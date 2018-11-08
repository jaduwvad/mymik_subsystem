#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include "csvhandler.h"

using namespace std;

namespace ThreatShopData {
    CSVHandler::CSVHandler(string filename, char delimiter, bool mode){
        this->_delimiter = _delimiter;

        if(mode){
            _srcFile.open(filename.c_str());
            string headerStr;
            getline(_srcFile, headerStr);
            myExplode(headerStr, _header);
        }
        else
            _dstFile.open(filename.c_str());
    }

    CSVHandler::~CSVHandler(){
        _srcFile.close();
        _dstFile.close();
    }

    void CSVHandler::readColumn(string columnHeader, vector<string>& result){
        string strTemp;
        vector<string> listTemp;
        auto it = find(_header.begin(), _header.end(), columnHeader);

        if(_header.end() == it)
            return;

        int columnPos = distance(_header.begin(), it);

        while(getline(_srcFile, strTemp)){
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
        _dstFile<<result<<endl;
    }

    void CSVHandler::myExplode(string s, vector<string>& result) {
        string buff("");

        for(char n:s) {
            if(n != _delimiter && n != '"')
                buff += n;
            else if( n == _delimiter ) {
                result.push_back(buff);
                buff="";
            }
        }
        if(buff != "") result.push_back(buff);
    }
}
