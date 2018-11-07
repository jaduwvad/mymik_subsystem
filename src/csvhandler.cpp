#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include "csvhandler.h"

using namespace std;

namespace ThreatShopData {
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
