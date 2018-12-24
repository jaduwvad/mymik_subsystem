#pragma once

#include "eshandler.h"
#include "srcdatafile.h"
#include "connection.h"

//id and price key name in elasticsearch data
std::string _esId = "ordernumber";
std::string _esPriceId = "price";

//id and price key name in src data. get name from config file
std::string _srcId;
std::string _srcPriceId;

//config file full name
std::string _shopDataFile = "/tmp/mymik/data/shop_data.json";
//directory for saving result file
std::string _resultDataDir = "/tmp/mymik/results/";

//key names of data needs from elasticsearch
std::vector<std::string> _esFields;


void configShop(Json::Object shop);
//get data from config file
void getShopData(Json::Array& shopData);
//get data from elasticsearch
void getESData(std::vector<Json::Object>& esData, std::string tag);
//get data from src file
void getSrcData(std::string url, Json::Object shopData, std::vector<Json::Object>& srcData);

//get matched data. match es data and src data by esid and srcid
void matchingList(std::vector<Json::Object>& srcData, std::vector<Json::Object>& esData, std::vector<Json::Object>& result);
//make json object by matched data
void setUpdatedData(Json::Object& updatedData, Json::Object& srcData, std::string price);
//write variantid and price about matched data
void writeOnFile(std::vector<Json::Object>& matchedData, std::string filename);

void sendResultFile(std::string filename);
void sendUpdateSignal(std::string tag);


