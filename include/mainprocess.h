#pragma once

#include "eshandler.h"
#include "srcdatafile.h"
#include "connection.h"

//Id and price key name in elasticsearch data
std::string _esId = "ordernumber";
std::string _esPriceId = "price";

//Id and price key name in src data. get name from config file
std::string _srcId;
std::string _srcPriceId;

//Config file full name
std::string _shopDataFile = "/tmp/mymik/data/shop_data.json";
//Directory for saving result file
std::string _resultDataDir = "/tmp/mymik/results/";

//Key names of data needs from elasticsearch
std::vector<std::string> _esFields;


void updateShop(Json::Object shop);
//Get data from config file
void getShopData(Json::Array& shopData);
//Get data from elasticsearch
void getESData(std::vector<Json::Object>& esData, std::string tag);
//Get data from src file
void getSrcData(std::string url, Json::Object shopData, std::vector<Json::Object>& srcData);

//Get matched data. match es data and src data by esid and srcid
void matchingList(std::vector<Json::Object>& srcData, std::vector<Json::Object>& esData, std::vector<Json::Object>& result);
//Make json object by matched data
void setUpdatedData(Json::Object& updatedData, Json::Object& srcData, std::string price);
//Write variantid and price about matched data
void writeOnFile(std::vector<Json::Object>& matchedData, std::string filename);

void sendResultFile(std::string filename);
void sendUpdateSignal(std::string tag);


