#pragma once

#include "eshandler.h"
#include "srcdatafile.h"
#include "connection.h"

std::string _esId = "ordernumber";
std::string _esPriceId = "price";

std::string _srcId;
std::string _srcPriceId;

std::string _shopDataFile = "/tmp/mymik/data/shop_data.json";
std::string _resultDataDir = "/tmp/mymik/results/";

std::vector<std::string> _esFields;

void configShop(Json::Object shop);
void getShopData(Json::Array& shopData);
void getESData(std::vector<Json::Object>& esData, std::string tag);
void getSrcData(std::string url, Json::Object shopData, std::vector<Json::Object>& srcData);
void setUpdatedData(Json::Object& updatedData, Json::Object& srcData, std::string price);
void matchingList(std::vector<Json::Object>& srcData, std::vector<Json::Object>& esData, std::vector<Json::Object>& result);
void setPriceInven(std::vector<Json::Object>& matchedData, std::string filename);

void sendResultFile(std::string filename);
void sendUpdateSignal(std::string tag);


