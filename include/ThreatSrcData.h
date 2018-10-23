#pragma once
#include <iostream>
#include <curl/curl.h>
#include <fstream>
#include <list>

namespace ThreatSrcData{
    class CurlHandler{
    public:
        CurlHandler(std::string url);
        CurlHandler(std::string url, std::string userId, std::string userPw);
        CurlHandler(std::string url, bool zipped);
        ~CurlHandler();

    private:
        void setCurl(const std::string url);
        void setCurl(const std::string url, std::string userId, std::string userPw);
        std::string base64_encode(unsigned char const* bytes_to_encode, unsigned int in_len);
        void unzipFile(std::string fn);

        static size_t writeData(void *ptr, size_t size, size_t nmemb, FILE *stream){
            return fwrite(ptr, size, nmemb, stream);
        }

        CURL *curl;
        CURLcode res;
        std::string url;
        std::string filename;
    };

    class CSVHandler{
    public:
        CSVHandler(std::string filename, const char* delimiter, bool mode);
        ~CSVHandler();

        void readLine(std::list<std::string>& result);
        void writeLine(std::list<std::string>& data);
    private:
        std::string myReplace(char *str, const std::string& oldStr, const std::string& newStr);

        std::ifstream srcFile;
        std::ofstream dstFile;
        const char *delimiter;
    };
}

