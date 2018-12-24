#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <cstdio>
#include <curl/curl.h>
#include "json.h"

namespace ThreatShopData {
    class SrcDataFile {
    public:
        struct MemoryStruct {
            char *memory;
            size_t size;
        };

        //Default constructor : download file by url in filename, open filestream
        SrcDataFile(std::string url, std::string filename, const char delimiter);
        //Constructor for urls requiring login
        SrcDataFile(std::string url, std::string filename, const char delimiter, std::string userId, std::string userPw);
        //Add unzip code in the end of constructor
        SrcDataFile(std::string url, std::string filename, const char delimiter, bool zipped);

        //Destructor : close filestream, delete downloaded file
        ~SrcDataFile();

        //read specific column`s value in csv file
        void readColumn(std::vector<std::string> columnHeaders, std::vector<Json::Object>& result);
        
    private:
        //open filestream in ifstream _srcFile, and get header data in vector _header
        void setFileStream();

        //Default file download function by url
        void setCurl(const std::string url);
        //file download by url with login - uncompleted
        void setCurl(const std::string url, std::string userId, std::string userPw);

        //get encoded string by base64. used in login
        std::string base64_encode(unsigned char const* bytes_to_encode, unsigned int in_len);
        //separate data by _delimiter. delete '"'
        void myExplode(std::string s, std::vector<std::string>& result);
        bool checkQuotes(std::string s);
        //function for download file
        static size_t writeData(void *ptr, size_t size, size_t nmemb, FILE *stream){
            return fwrite(ptr, size, nmemb, stream);
        }

        //filename for downloaded data
        std::string _filename;
        //file stream at downloaded data
        std::ifstream _srcFile;
        char _delimiter;
        std::vector<std::string> _header;
        int _filePointer;
    };
}
