#pragma once
#include <iostream>
#include <vector>
#include "eshandler.h"

class MymikProcess {
public:
    MymikProcess();
    ~MymikProcess();

    void matchingList(std::vector<Json::Object>& srcData, std::vector<Json::Object>& esData, std::vector<Json::Object>& result);
    void getShopData(Json::Array& shopData);
    void getESData(std::vector<Json::Object>& esData, std::string tag);
    void showTime();
    std::string formSupplierNumber(std::string spn);

private:
    const std::string shopDataFile;
    const std::string esSpnColumn;
    const std::string srcArtNumberColumn;
    const std::string configMainTag;
};


