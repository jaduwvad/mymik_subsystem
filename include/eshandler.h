#pragma once
#include <iostream>
#include "elasticsearch.h"

namespace ThreatShopData {
    class ESHandler {
    public:
        ESHandler();
        ~ESHandler();

        void getArticles(std::string spn, std::string tag);
        void updatePrice(std::string variantID, std::string price);
        void getFieldList(std::string esResponse, std::string fieldName);
        bool isSuccess(std::string esResponse);

    private:
        std::string _ess;
        std::string _essIndex;
        std::string _essFuncUpdate;
        std::string _essFuncSearch;
        std::string _essIndexType;



    };
}
