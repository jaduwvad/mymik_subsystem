#include "eshandler.h"

using namespace std;

/**
 * @brief	Constructor : set elasticsearch index connecting data
 */
ESHandler::ESHandler():
    _ess("46.4.23.60:9200"),
    _essIndex("articles_price"),
    _essIndexType("product") { }

ESHandler::~ESHandler() {

}

/**
 * @brief	Get the shop's all articles 
 * @parameter	tag : Specific shop's tag. The same shop, have same tag at artnumber
 * 		result : array of articles to return
 * @description	Get all articles that is in the shop matched by tag
 */
void ESHandler::getArticlesByTag(string tag, Json::Array& result ){
    ElasticSearch es(_ess);
    string scrollId;
    string query = "{ \
        \"query\":{ \
            \"match\":{ \
                \"artnumber\":\"" + tag + "\" \
            } \
        } \
    }";

    //use Elasticsearch Scroll to get more than 10000 articles
    es.initScroll(result, scrollId, _essIndex, _essIndexType, query, 5000);
    size_t cs=0, ns;
    while(es.scrollNext(scrollId, result)){
        ns = result.size();
        if(cs == ns)
            break;
        cs = ns;
    }
}

/**
 * @brief	get specific data from es response
 * @parameter	esResponse : Json Array. response about elasticsearch request
 * 		fieldsName : specific data, which is need to process
 * 		result	   : vector to save and return the specific data
 */
void ESHandler::getFieldList(Json::Array& esResponse, vector<string> fieldsName, vector<Json::Object>& result){
    Json::Array::const_iterator ci = esResponse.begin();

    do{
        Json::Object jo((*ci).getObject()["_source"].getObject());
        Json::Object temp;

        for(string fieldName:fieldsName)
            temp.addMemberByKey(fieldName, jo[fieldName].getString());
        
        result.push_back(temp);
        ci = ++ci;
    }while(ci != esResponse.end());
}


