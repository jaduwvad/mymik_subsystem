#pragma once
#include <iostream>
#include <vector>
#include "json.h"

class MymikProcess {
public:
    MymikProcess();
    ~MymikProcess();
    void getESData(std::vector<Json::Object>& esData, std::string tag);
    void configShop(Json::Object shop);
    void matchingList(std::vector<Json::Object>& src, std::vector<Json::Object>& dst, std::vector<Json::Object>& result);
    void setPriceInven(std::vector<Json::Object>& src, std::vector<Json::Object>& dst);

private:
    void getConfig(Json::Array& configData);
    std::string getSupplierNumber(std::string spn);
    bool myCompare(Json::Object a, Json::Object b);
    bool myCompare2(Json::Object a, Json::Object b);
}


