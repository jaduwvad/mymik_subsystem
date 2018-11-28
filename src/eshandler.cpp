
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

    void ESHandler::getArticles(string spn, string tag, Json::Object& jo) {
        ElasticSearch es(_ess);
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
    }

    void ESHandler::updatePrice(string variantID, string price) {
        ElasticSearch es(_ess);
        Json::Object jo;

        jo.addMemberByKey("price", price);

        es.update(_essIndex, _essIndexType, variantID, jo);
    }

    void ESHandler::getArticlesByTag(string tag, Json::Array& result ){
        ElasticSearch es(_ess);
        string scrollId;
        string query = "{ \
            \"query\":{ \
                \"match\":{ \
                    \"artNumber\":\"" + tag + "\" \
                } \
            } \
        }";

        es.initScroll(result, scrollId, _essIndex, _essIndexType, query, 5000);
        size_t cs=0, ns;
        while(es.scrollNext(scrollId, result)){
            ns = result.size();
            if(cs == ns)
                break;
            cs = ns;
        }
    }

    void ESHandler::getFieldList(Json::Array& esResponse, vector<string> fieldsName, vector<Json::Object>& result){
        Json::Array::const_iterator ci = esResponse.begin();

        do{
            Json::Object jo((*ci).getObject()["_source"].getObject());
            Json::Object temp;

            for(string fieldName:fieldsName){
                temp.addMemberByKey(fieldName, jo[fieldName].getString());


            }
            result.push_back(temp);
            ci = ++ci;
        }while(ci != esResponse.end());
    }

/*
    bool ESHandler::isSuccess(string esResponse){

    }
*/
}




