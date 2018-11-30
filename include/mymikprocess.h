#pragma once
#include <iostream>
#include <vector>
#include "eshandler.h"

class MymikProcess {
public:
    MymikProcess();
    ~MymikProcess();

    void getSrcData(Json::Array::const_iterator ci, std::string filename, std::vector<Json::Object>& srcData);
    void getShopData(Json::Array& shopData);
    void getESData(std::vector<Json::Object>& esData, std::string tag);

    void matchingList(std::vector<Json::Object>& srcData, std::vector<Json::Object>& esData, std::vector<Json::Object>& result);
    void setPriceInven(std::vector<Json::Object>& matchedData, std::string filename);
    void setUpdatedData(Json::Object& updatedData, Json::Object& srcData, std::string price, bool priceUpdate);

    std::string formSupplierNumber(std::string spn);
    void showTime();

private:
    const std::string _shopDataFile;
    const std::string _priceDataDir;
    const std::string _invenDataAcDir;
    const std::string _invenDataInacDir;

    const std::string _configMainTag;
};


