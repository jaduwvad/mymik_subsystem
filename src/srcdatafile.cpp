#include <algorithm>
#include <stdio.h>
#include "srcdatafile.h"
#include "json.h"

using namespace std;

namespace ThreatShopData {
    SrcDataFile::SrcDataFile(string url, string filename, const char delimiter) {
        this->_filename = filename;
        this->_delimiter = delimiter;
        setCurl(url);
        setFileStream();
    }

    SrcDataFile::SrcDataFile(string url, string filename, const char delimiter, string userId, string userPw){
        this->_filename = filename;
        this->_delimiter = delimiter;
        setCurl(url, userId, userPw);
        setFileStream();
    }

    SrcDataFile::SrcDataFile(string url, string filename, const char delimiter, bool zipped){ 
        this->_filename = filename + ".gz";
        this->_delimiter = delimiter;
        setCurl(url);

        string cmd = "gzip -df " + this->_filename;

        system(cmd.c_str());
        this->_filename = filename;
        setFileStream();
    }

    SrcDataFile::~SrcDataFile(){
        _srcFile.close();
        remove(_filename.c_str());
    }

    void SrcDataFile::readColumn(vector<string> columnHeaders, vector<Json::Object>& result){
        vector<int> headerList;
        string strTemp;
        vector<string> listTemp;

        for(string header:columnHeaders){
            auto it = find(_header.begin(), _header.end(), header);

            if(_header.end() == it)
                return;

            headerList.push_back( distance( _header.begin(), it ) );
        }

        while(getline(_srcFile, strTemp)){
            myExplode(strTemp, listTemp);
            Json::Object jo;

            if(listTemp.size() != _header.size())
                continue;

            for(int i=0; i<columnHeaders.size(); i++)
                jo.addMemberByKey(columnHeaders.at(i), listTemp.at(headerList.at(i)));

            result.push_back(jo);
            listTemp.clear();
        }

        _srcFile.clear();
        _srcFile.seekg(_filePointer, ios::beg);
    }

    void SrcDataFile::setFileStream(){
        string headerStr;
        _srcFile.open(_filename.c_str());
        getline(_srcFile, headerStr);
        myExplode(headerStr, _header);
        _filePointer = _srcFile.tellg();
    }

    void SrcDataFile::setCurl(const string url){
        FILE *fp = fopen(_filename.c_str(), "w");
        CURL *curl;
        curl = curl_easy_init();
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeData);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
        curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        fclose(fp);
    }

    void SrcDataFile::setCurl(const string url, string userId, string userPw) {
        CURL *curl;
        CURLcode res;
        FILE *fp = fopen(_filename.c_str(), "w");

        curl_global_init(CURL_GLOBAL_ALL);
        curl = curl_easy_init();

        if(curl) {
            struct curl_slist *headers = NULL;
            headers = curl_slist_append(headers, "Accept: text/plain");

            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_HTTPAUTH, (long)CURLAUTH_BASIC);
            curl_easy_setopt(curl, CURLOPT_USERNAME, userId.c_str());
            curl_easy_setopt(curl, CURLOPT_PASSWORD, userPw.c_str());
            curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);

            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeData);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
            curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
            res = curl_easy_perform(curl);
            
            curl_slist_free_all(headers);
            curl_easy_cleanup(curl);
            curl_global_cleanup();
        }

        return;
    }

    void SrcDataFile::myExplode(string s, vector<string>& result) {
        string buff("");
        bool isQuoteUsed = checkQuotes(s);

        if( isQuoteUsed ){
            bool inQuote = false;
            for( int i=0; i<s.length(); i++) {
                char n = s.at(i);

                if(n == '"' && !inQuote){
                    inQuote = true;
                }
                else if(n == '"' && inQuote) {
                    if(i == s.length() -1){
                        inQuote = false;
                        result.push_back(buff);
                        buff = "";
                        i++;
                    }
                    else if(s.at(i+1) == '"') {
                        buff += "\"\"";
                        i++;
                    }
                    else {
                        inQuote = false;
                        result.push_back(buff);
                        buff = "";
                        i++;
                    }
                }
                else if(n == _delimiter && !inQuote){
                    result.push_back(buff);
                    buff = "";
                }
                else
                    buff += n;
            }
            if(buff != "") result.push_back(buff);
        }
        else{
            for(char n:s){
                if(n == _delimiter){
                    result.push_back(buff);
                    buff = "";
                }
                else //if( n != '"')
                    buff += n;
            }
            if(buff != "" || s.at(s.length()-1) == _delimiter) result.push_back(buff);
        }
    }


    bool SrcDataFile::checkQuotes(string s){
        for( int i = 1; i < s.length(); i++) {
            char n = s.at(i);
            if(n == '"' && s.at(i-1) == _delimiter){
                for(int j = i+1; j < s.length(); j++){
                    if(s.at(j) == '"') {
                        if(j == s.length() - 1)
                            return true;
                        else if(s.at(j+1) == _delimiter)
                            return true;
                    }
                }
            }
        }
        return false;
    }

    string SrcDataFile::base64_encode(unsigned char const* bytes_to_encode, unsigned int in_len){
        string base64_chars =
             "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
             "abcdefghijklmnopqrstuvwxyz"
             "0123456789+/";
        string ret;
        int i = 0;
        int j = 0;
        unsigned char char_array_3[3];
        unsigned char char_array_4[4];

        while (in_len--) {
            char_array_3[i++] = *(bytes_to_encode++);
            if (i == 3) {
                char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
                char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
                char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
                char_array_4[3] = char_array_3[2] & 0x3f;

                for(i = 0; (i <4) ; i++)
                    ret += base64_chars[char_array_4[i]];
                i = 0;
            }
        }

        if (i) {
            for(j = i; j < 3; j++)
                char_array_3[j] = '\0';

            char_array_4[0] = ( char_array_3[0] & 0xfc) >> 2;
            char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
            char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);

            for (j = 0; (j < i + 1); j++)
                ret += base64_chars[char_array_4[j]];

            while((i++ < 3))
                ret += '=';
        }
        return ret;
    }
}
