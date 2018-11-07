#pragma once
#include <iostream>

namespace ThreatShopData{
    class ESHandler {
    public:
        ESHandler();
        ~ESHandler();

    private:
        std::string ess;
        std::string essIndex;
        std::string essFuncUpdate;
        std::string essFuncSearch;
        std::string essIndexType;
    };
}
