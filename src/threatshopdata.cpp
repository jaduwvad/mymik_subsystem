#include <iostream>
#include <fstream>
#include <curl/curl.h>
#include <cstring>
#include <stdlib.h>
#include <vector>
#include <algorithm>
#include "csvhandler.h"
#include "curlhandler.h"

using namespace std;

namespace ThreatShopData{
    //일반 파일 다운로드용 생성자
    CurlHandler::CurlHandler(string url, string filename) {
        this->filename = filename;
        setCurl(url);
    }

    //로그인이 필요한 파일 다운로드용 생성자
    CurlHandler::CurlHandler(string url, string filename, string userId, string userPw) {
        this->filename = filename;
        setCurl(url, userId, userPw);
    }

    //gzip으로 압축된 파일을 다운로드 후 압축 해제를 위한 생성자
    CurlHandler::CurlHandler(string url, string filename, bool zipped) {
        this->filename = filename+".gz";
        setCurl(url);

        //system("gzip -d " + this->filename);
    }

    //다운로드한 파일 삭제
    CurlHandler::~CurlHandler() {
        //remove(filename.c_str());
    }

    void CurlHandler::setCurl(const string url){
        FILE *fp = fopen(filename.c_str(), "w");
        curl = curl_easy_init();
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeData);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
        curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        fclose(fp);
    }

    void CurlHandler::setCurl(const string url, string userId, string userPw){
        FILE *fp = fopen(filename.c_str(), "w");
        struct curl_slist* headers = NULL;
        string s = userId+":"+userPw;
        string data = "Authorization: Basic " + base64_encode(reinterpret_cast<const unsigned char*>(s.c_str()), s.length());

        curl = curl_easy_init();
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        headers = curl_slist_append(headers, data.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        //curl_easy_setopt(curl, CURLOPT_USERPWD, data.c_str());
        //curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
        //curl_easy_setopt(curl, CURLOPT_HTTPAUTH, (long)CURLAUTH_ANY);
        //curl_easy_setopt(curl, CURLOPT_USERPWD, data.c_str());
        //curl_easy_setopt(curl, CURLOPT_PASSWORD, userPw.c_str());
        
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, CurlHandler::writeData);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);

        curl_easy_perform(curl);
        cout<<data<<endl;
        curl_easy_cleanup(curl);
        fclose(fp);
    }

    void CurlHandler::unzipFile(string fn){
        
    }

    string CurlHandler::base64_encode(unsigned char const* bytes_to_encode, unsigned int in_len) {
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

    //mode가 true면 read, false면 write
    CSVHandler::CSVHandler(string filename, char delimiter, bool mode){
        this->delimiter = delimiter;

        if(mode){
            srcFile.open(filename.c_str());
            string headerStr;
            getline(srcFile, headerStr);
            myExplode(headerStr, header);
        }
        else
            dstFile.open(filename.c_str());
    }

    CSVHandler::~CSVHandler(){
        srcFile.close();
        dstFile.close();
    }

    void CSVHandler::readColumn(string columnHeader, vector<string>& result){
        string strTemp;
        vector<string> listTemp;
        auto it = find(header.begin(), header.end(), columnHeader);

        if(header.end() == it) 
            return;

        int columnPos = distance(header.begin(), it);

        while(getline(srcFile, strTemp)){
            myExplode(strTemp, listTemp);
            result.push_back(listTemp.at(columnPos));
            listTemp.clear();
        }
    }

    void CSVHandler::writeRow(vector<string>& data){
        string result = "";

        while(!data.empty()){
            result = result+data.back();
            data.pop_back();
            result = result+",";
        }
        result.pop_back();
        dstFile<<result<<endl;
    }

    void CSVHandler::myExplode(string s, vector<string>& result) {
        string buff("");

        for(char n:s) { 
            if(n != delimiter && n != '"') 
                buff += n;
            else if( n == delimiter ) {
                result.push_back(buff); 
                buff="";
            }
        }
        if(buff != "") result.push_back(buff);
    }

}

