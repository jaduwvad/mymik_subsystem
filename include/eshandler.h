#pragma once
#include <iostream>
#include <vector>
#include "elasticsearch.h"

class ESHandler {
public:
    //Constructor : set elasticsearch connection data
    ESHandler();
    ~ESHandler();

    //Find articles by tag in artnumber. get all articles in Json::Array
    void getArticlesByTag(std::string tag, Json::Array& result);
    //Get specific field in elasicsearch response. use vector
    void getFieldList(Json::Array& esResponse, std::vector<std::string> fieldsName, std::vector<Json::Object>& result);

    //Unused in main process
    bool isSuccess(std::string esResponse);

private:
    std::string _ess;
    std::string _essIndex;
    std::string _essIndexType;
};
