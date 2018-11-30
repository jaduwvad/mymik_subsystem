#pragma once
#include <iostream>
#include <vector>
#include "elasticsearch.h"

namespace ThreatShopData {
    class ESHandler {
    public:
        //Constructor : set elasticsearch connection data
        ESHandler();
        ~ESHandler();

        //find one article by tag and suppliernumber. unused in main process
        void getArticles(std::string spn, std::string tag, Json::Object& jo);
        //find articles by tag in artnumber. get all articles in Json::Array
        void getArticlesByTag(std::string tag, Json::Array& result);
        //get specific field in elasicsearch response. use vector
        void getFieldList(Json::Array& esResponse, std::vector<std::string> fieldsName, std::vector<Json::Object>& result);

        //update one article's price in elasticsearch index by variantID
        void updatePrice(std::string variantID, std::string price);
        //unused in main process
        bool isSuccess(std::string esResponse);

    private:
        std::string _ess;
        std::string _essIndex;
        std::string _essIndexType;



    };
}
