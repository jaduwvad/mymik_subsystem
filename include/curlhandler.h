#pragma once
#include <iostream>
#include <curl/curl.h>

namespace ThreatShopData {
    class CurlHandler {
    public:
        CurlHandler(std::string url, std::string filename);
        CurlHandler(std::string url, std::string filename, std::string userId, std::string userPw);
        CurlHandler(std::string url, std::string filename, bool zipped);
        ~CurlHandler();

    private:
        void setCurl(const std::string url);
        void setCurl(const std::string url, std::string userId, std::string userPw);
        std::string base64_encode(unsigned char const* bytes_to_encode, unsigned int in_len);
        void unzipFile(std::string fn);

        static size_t writeData(void *ptr, size_t size, size_t nmemb, FILE *stream){
            return fwrite(ptr, size, nmemb, stream);
        }

        std::string _filename;
    };
}
