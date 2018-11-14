
#include "eshandler.h"

namespace ThreatShopData {
    ESHandler::ESHandler():
    _ess("localhost:9200"),
    _essIndex("articles_price"),
    _essFuncUpdate("_update"),
    _essFuncSearch("_search"),
    _essIndexType("product") {

    }

    ESHandler::~ESHandler() {

    }


}

