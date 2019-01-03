#include <algorithm>
#include <thread>
#include "mainprocess.h"

using namespace std;
using namespace Json;

int main() {
    Array shopData;
    getShopData(shopData);

    Array::const_iterator ci = shopData.begin();

    do{
        Object shop = (*ci).getObject();
        ci = ++ci;
        updateShop(shop);
    }while(ci != shopData.end());
}

/**
 * @brief	Update a shop's articles
 * @parameter	shop : the shop's config date in json object
 */
void updateShop(Object shop){
    Array urls = shop["url"].getArray();
    Array::const_iterator ci = urls.begin();
    vector<Object> esData;
    vector<Object> srcData;
    vector<Object> matchedData;

    cout<<shop["shop"].getString()<<endl;
    getESData(esData, shop["tag"].getString());

    sort(esData.begin(), esData.end(), [](Object a, Object b) {
        return a[_esId].getString() < b[_esId].getString();
    });

    do{
        _srcId = shop["srcId"].getString();
        _srcPriceId = shop["priceId"].getString();
        getSrcData((*ci).getString(), shop, srcData);

        sort(srcData.begin(), srcData.end(), [](Object a, Object b) {
            return a[_srcId].getString() < b[_srcId].getString();
        });

        if(shop["tag"].getString() == "FLC-") 
            for(int i=0; i<srcData.size(); i++)
                if(!srcData.at(i)["CF_Availability"].getString().compare("No")) 
                    srcData.erase(srcData.begin() + i);

        matchingList(srcData, esData, matchedData);

        srcData.clear();
        ++ci;
    } while(ci != urls.end());

    cout<<matchedData.size()<<endl;
    writeOnFile(matchedData, shop["file"].getString());

    matchedData.clear();
    esData.clear();

    sendResultFile(shop["file"].getString());
    sendUpdateSignal(shop["tag"].getString());
}

/**
 * @brief	Get shops' data to update from config file
 * @description	The shops' data are written in json array named 'shops'
 * 		So, take the data in Json Array value
 */
void getShopData(Array& shopData){
    ifstream ifile(_shopDataFile.c_str());
    Object jo;
    string lineTemp;
    string result = "";

    while(getline(ifile, lineTemp))
        result += lineTemp + "\n";

    jo.addMember(result.c_str(), result.c_str() + result.size());
    shopData = jo["shops"].getArray();

    Array esfield = jo["esfield"].getArray();
    Array::const_iterator ci = esfield.begin();

    do{
        _esFields.push_back((*ci).getString());
        ci = ++ci;
    }while(ci != esfield.end());
}

/**
 * @brief	Get the shop's articles from elasticsearch 
 * @parameter	esData : vector to save and return data from elasticsearch
 * 		tag : the tag for config shop
 */
void getESData(vector<Object>& esData, string tag) {
    Array resultArray;
    ESHandler es;

    es.getArticlesByTag(tag, resultArray);
    es.getFieldList(resultArray, _esFields, esData);

    resultArray.clear();
}

/**
 * @brief	Get the shop's articles from source file
 * @parameter	url : Link to download file
 * 		shopData : the shop's data need to download source file
 * 		srcData : vector to save and return data from elasticsearch
 */
void getSrcData( string url, Object shopData, vector<Object>& srcData ) {
    SrcDataFile *ch;
    vector<string> headers;
    string filename = shopData["file"].getString();
    Array srcColumn = shopData["srcColumn"].getArray();
    int type = shopData["type"].getInt();

    Array::const_iterator columnIndex = srcColumn.begin();

    switch(type){
        case 0:
            //case 0 : normal download
            ch = new SrcDataFile(url, filename, ';');
            break;
        case 1:
            // case 1 : unzip function after download
            ch = new SrcDataFile(url, filename, ',', true);
            break;
        case 2:
            // case 2 : need to login to download file
            {
                string userId = shopData["userId"].getString();
                string userPw = shopData["userPw"].getString();
                ch = new SrcDataFile(url, filename, '	', userId, userPw); 
                break;
            }
        default :
            return;
    }

    do{
        headers.push_back((*columnIndex).getString());
        columnIndex = ++columnIndex;
    }while(columnIndex != srcColumn.end());

    ch->readColumn(headers, srcData);

    delete ch;

    headers.clear();
}

/**
 * @brief	Make json object about activated article
 * @description	To update price and inventory,
 * 		program should get variantid and price to match at database
 */
void setUpdatedData(Object& updatedData, Object& srcData, string price) {
    updatedData.addMemberByKey("variantid", srcData["variantid"].getString());
    updatedData.addMemberByKey("price", price);
}

/**
 * @brief	Match srcData and esData
 * @description	The article that is exist at srcData and esData both,
 * 		it should be appended at result
 * 		Two data are sorted by same function, so the matching calculation
 * 		goes (srcData.size() + esData.size()) times.
 */
void matchingList(vector<Object>& srcData, vector<Object>& esData, vector<Object>& result) {
    int esDataIndex = 0;
    int srcDataSize = srcData.size();
    int esDataSize = esData.size();

    for(int i=0; i<srcDataSize && esDataIndex < esDataSize; i++){
        //srcId : artnumber, esId : ordernumber except tag
        string srcId = srcData.at(i)[_srcId].getString();
        string esId = esData.at(esDataIndex)[_esId].getString();

        Object updatedData;

        if(srcId>esId){
            esDataIndex++;
            i--;
        }
        else if(srcId == esId){
            //If two ids are same, push at result vector
            string price = srcData.at(i)[_srcPriceId].getString();

            //But if the price is 0, do not push it
            if(atoi(price.c_str()) > 0) {
                setUpdatedData(updatedData, esData.at(esDataIndex), price);
                result.push_back(updatedData);
            }
            esDataIndex++;
        }
    }
}

/**
 * @brief	Write result file
 * @parameter	matchedData : data of articles up to update
 * 		filename : result file's full path
 * @description Write the article's variantid and price at result file
 * 		format : [variantid],[price]
 */
void writeOnFile(vector<Object>& matchedData, string filename) {
    int matchedDataSize = matchedData.size();

    ofstream resultFile(_resultDataDir + filename);

    for(int i=0; i < matchedDataSize; i++){
        Object matchedObject = matchedData.at(i);

        resultFile<<matchedObject["variantid"].getString()<<",";
        resultFile<<matchedObject["price"].getString()<<endl;
    }

    resultFile.close();
}

void sendResultFile(string filename) {
    Connection c;

    c.sendFile(filename);
}

void sendUpdateSignal(string tag){
    Connection c;
    string message = tag.substr(0, tag.length()-1);

    c.sendMessages(message);
}
