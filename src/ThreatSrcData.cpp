#include <iostream>
#include <fstream>
#include <curl/curl.h>
#include <cstring>
#include <list>
#include "ThreatSrcData.h"

using namespace std;

namespace ThreatSrcData{
    //일반 파일 다운로드용 생성자
    CurlHandler::CurlHandler(string url)
    : filename("./result"){
        FILE *fp = fopen(filename.c_str(), "wb");
        curl = curl_easy_init();
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeData);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
        curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        fclose(fp);
    }

    //로그인이 필요한 파일 다운로드용 생성자
    CurlHandler::CurlHandler(string url, string userId, string userPw)
    : filename("./result"){
        FILE *fp = fopen(filename.c_str(), "wb");
        curl = curl_easy_init();
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

        curl_easy_setopt(curl, CURLOPT_HTTPAUTH, (long)CURLAUTH_BASIC);
        curl_easy_setopt(curl, CURLOPT_USERNAME, userId.c_str());
        curl_easy_setopt(curl, CURLOPT_PASSWORD, userPw.c_str());

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, CurlHandler::writeData);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
        curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        fclose(fp);
    }

    //gzip으로 압축된 파일을 다운로드 후 압축 해제를 위한 생성자
    CurlHandler::CurlHandler(string url, bool zipped)
    : filename("./result"){
        FILE *fp = fopen("temp.gz", "wb");
        //setCurl(fp);
        fclose(fp);

        unzipFile("temp.gz");
    }

    //다운로드한 파일 삭제
    CurlHandler::~CurlHandler(){
        remove(filename.c_str());
    }

    void CurlHandler::unzipFile(string fn){
        
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

