#pragma once
#include <iostream>
#include <vector>
#include "elasticsearch.h"

namespace ThreatShopData {
    class ESHandler {
    public:
        ESHandler();
        ~ESHandler();

        void getArticles(std::string spn, std::string tag, Json::Object& jo);
        void updatePrice(std::string variantID, std::string price);
        void getArticlesByTag(std::string tag, Json::Array& result);
        void getFieldList(Json::Array& esResponse, std::vector<std::string> fieldsName, std::vector<Json::Object>& result);
        bool isSuccess(std::string esResponse);

    private:
        std::string _ess;
        std::string _essIndex;
        std::string _essIndexType;



    };
}
