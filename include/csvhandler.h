#pragma once
#include <iostream>
#include <fstream>
#include <vector>

namespace ThreatShopData{
    class CSVHandler {
    public:
        CSVHandler(std::string filename, const char delimiter, bool mode);
        ~CSVHandler();

        void readColumn(std::string columnHeader, std::vector<std::string>& result);
        void writeRow(std::vector<std::string>& data);

    private:
        void myExplode(std::string s, std::vector<std::string>& result);

        std::ifstream _srcFile;
        std::ofstream _dstFile;
        char _delimiter;
        std::vector<std::string> _header;
    };
}

