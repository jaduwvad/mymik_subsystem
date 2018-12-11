#pragma once
#include <iostream>
#include <vector>
#include "eshandler.h"

class MymikProcess {
public:
    //Constructor : set data file name, result file's directory, config index name
    MymikProcess();
    ~MymikProcess();

    //get shop data in json format
    void getShopData(Json::Array& shopData);
    //get article's data by tag and get specific field in Object vector
    void getESData(std::vector<Json::Object>& esData, std::string tag, Json::Array esfield);
    //get article's specific column data in src file from affilinet
    void getSrcData(std::string url, std::string filename, std::vector<Json::Object>& srcData, Json::Array srcColumn);

    //match two article list. One from affilinet, one from elasticsearch. Check whether price updated
    void matchingList(std::vector<Json::Object>& srcData, std::vector<Json::Object>& esData, std::vector<Json::Object>& result);
    //Write matched articles in result file. Write article's price data if price's updated
    void setPriceInven(std::vector<Json::Object>& matchedData, std::string filename);

    //make Json Object by matched article data.
    void setUpdatedData(Json::Object& updatedData, Json::Object& srcData, std::string price, bool priceUpdate);
    //parse supplier number : delete 'P' in VTS's ArtNumber and delete '0' in front of ArtNumber
    std::string formSupplierNumber(std::string spn);
    void showTime();

private:
    const std::string _shopDataFile;
    const std::string _resultDataDir;

    const std::string _configMainTag;
};


