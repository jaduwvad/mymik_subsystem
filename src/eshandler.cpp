
#include "eshandler.h"

using namespace std;

namespace ThreatShopData {
    ESHandler::ESHandler():
    _ess("46.4.23.60:9200"),
    _essIndex("articles_price"),
    _essIndexType("product") {

    }

    ESHandler::~ESHandler() {

    }

    void ESHandler::getArticles(string spn, string tag) {
        ElasticSearch es(_ess);
        Json::Object jo;
        string query = "{ \
            \"query\":{ \
                \"bool\":{ \
                    \"must\":[ \
                        {\"match\":{\"supplierNumber\":{\"query\":\"" + spn + "\", \"operator\":\"and\"}}}, \
                        {\"match\":{\"artNumber\":\"" + tag + "\"}} \
                    ] \
                }\
            }\
        }";

        es.search(_essIndex, _essIndexType, query, jo);
        cout<<jo.pretty()<<endl;
    }

    void ESHandler::updatePrice(string variantID, string price) {
        ElasticSearch es(_ess);
        Json::Object jo;

        jo.addMemberByKey("price", price);

        es.update(_essIndex, _essIndexType, variantID, jo);
    }

    void ESHandler::getFieldList(string esResponse, string fieldName){
        
    }

    bool ESHandler::isSuccess(string esResponse){

    }

}




