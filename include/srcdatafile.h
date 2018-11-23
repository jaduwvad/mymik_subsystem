#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <curl/curl.h>
#include "json.h"

namespace ThreatShopData {
    class SrcDataFile {
    public:
        //Default constructor : download file by url in filename, open filestream
        SrcDataFile(std::string url, std::string filename, const char delimiter);
        //Constructor for urls requiring login
        SrcDataFile(std::string url, std::string filename, const char delimiter, std::string userId, std::string userPw);
        //Add unzip code in the end of constructor
        SrcDataFile(std::string url, std::string filename, const char delimiter, bool zipped);
        //close filestream, delete file
        ~SrcDataFile();

        //read spacific column`s value in csv file
        void readColumn(std::vector<std::string> columnHeaders, std::vector<Json::Object>& result);
        
        
    private:
 
        void setFileStream();
        void setCurl(const std::string url);
        void setCurl(const std::string url, std::string userId, std::string userPw);
        std::string base64_encode(unsigned char const* bytes_to_encode, unsigned int in_len);
        void myExplode(std::string s, std::vector<std::string>& result);

        static size_t writeData(void *ptr, size_t size, size_t nmemb, FILE *stream){
            return fwrite(ptr, size, nmemb, stream);
        }

        std::string _filename;
        std::ifstream _srcFile;
        char _delimiter;
        std::vector<std::string> _header;
        int _filePointer;
    };
}
