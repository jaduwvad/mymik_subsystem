#include <iostream>
#include <fstream>
#include <curl/curl.h>
#include <cstring>
#include <stdlib.h>
#include <list>
#include "ThreatSrcData.h"

using namespace std;

namespace ThreatSrcData{
    //일반 파일 다운로드용 생성자
    CurlHandler::CurlHandler(string url)
    : filename("./result"){
        setCurl(url);
    }

    //로그인이 필요한 파일 다운로드용 생성자
    CurlHandler::CurlHandler(string url, string userId, string userPw)
    : filename("./result"){
        setCurl(url, userId, userPw);
    }

    //gzip으로 압축된 파일을 다운로드 후 압축 해제를 위한 생성자
    CurlHandler::CurlHandler(string url, bool zipped)
    : filename("./result.gz"){
        setCurl(url);

        system("gzip -d result.gz");
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
    CSVHandler::CSVHandler(string filename, const char* delimiter, bool mode){
        if(mode)
            srcFile.open(filename.c_str());
        else
            dstFile.open(filename.c_str());

        CSVHandler::delimiter = delimiter;
    }

    CSVHandler::~CSVHandler(){
        srcFile.close();
        dstFile.close();
    }

    void CSVHandler::readLine(list<string>& result){
        char temp[1024];
        char *splitted;

        srcFile.getline(temp, 1024);
        splitted = strtok(temp, delimiter);
        while(splitted != NULL){
            string temp = myReplace(splitted, "\"", "");
            result.push_back(temp);
            splitted = strtok(NULL, delimiter);
        }
    }
    void CSVHandler::writeLine(list<string>& data){
        string result = "";
        cout<<data.size()<<endl;
        while(true){
            result = result+data.front();
            data.pop_front();
            if(!data.empty())
                result = result+",";
            else
                break;
            
        }
        dstFile<<result<<endl;
    }

    string CSVHandler::myReplace(char *str, const string& oldStr, const string& newStr){
        string result(str);
        string::size_type pos = 0u;
        while((pos = result.find(oldStr, pos)) != string::npos){
            result.replace(pos, oldStr.length(), newStr);
            pos += newStr.length();
        }
        return result;
    }
}

