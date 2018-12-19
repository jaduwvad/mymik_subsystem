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
        configShop(shop);
    }while(ci != shopData.end());
}

void configShop(Object shop){
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

        matchingList(srcData, esData, matchedData);

        srcData.clear();
        ++ci;
    } while(ci != urls.end());

    cout<<matchedData.size()<<endl;
    setPriceInven(matchedData, shop["file"].getString());

    matchedData.clear();
    esData.clear();

    sendResultFile(shop["file"].getString());

    sendUpdateSignal(shop["tag"].getString());

}

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

void getESData(vector<Object>& esData, string tag) {
    Array resultArray;
    ThreatShopData::ESHandler es;

    es.getArticlesByTag(tag, resultArray);
    es.getFieldList(resultArray, _esFields, esData);

    resultArray.clear();
}

void getSrcData( string url, Object shopData, vector<Object>& srcData ) {
    ThreatShopData::SrcDataFile *ch;
    vector<string> headers;
    string filename = shopData["file"].getString();
    Array srcColumn = shopData["srcColumn"].getArray();
    int type = shopData["type"].getInt();

    Array::const_iterator ci = srcColumn.begin();

    switch(type){
        case 0:
            ch = new ThreatShopData::SrcDataFile(url, filename, ';');
            break;
        case 1:
            ch = new ThreatShopData::SrcDataFile(url, filename, ',', true);
            break;
        case 2:
            {
                string userId = shopData["userId"].getString();
                string userPw = shopData["userPw"].getString();
                ch = new ThreatShopData::SrcDataFile(url, filename, '	', userId, userPw); 
                break;
            }
        default :
            return;
    }

    do{
        headers.push_back((*ci).getString());
        ci = ++ci;
    }while(ci != srcColumn.end());

    ch->readColumn(headers, srcData);

    delete ch;

    headers.clear();
}

void setUpdatedData(Object& updatedData, Object& srcData, string price) {
    updatedData.addMemberByKey("variantid", srcData["variantid"].getString());
    updatedData.addMemberByKey("price", price);
}

void matchingList(vector<Object>& srcData, vector<Object>& esData, vector<Object>& result) {
    int esDataIndex = 0;
    int srcDataSize = srcData.size();
    int esDataSize = esData.size();

    for(int i=0; i<srcDataSize && esDataIndex < esDataSize; i++){
        string srcId = srcData.at(i)[_srcId].getString();
        string esId = esData.at(esDataIndex)[_esId].getString();

        Object updatedData;

        if(srcId>esId){
            esDataIndex++;
            i--;
        }
        else if(srcId == esId){
            string price = srcData.at(i)[_srcPriceId].getString();

            setUpdatedData(updatedData, esData.at(esDataIndex), price);
            result.push_back(updatedData);
            esDataIndex++;
        }
    }
}

void setPriceInven(vector<Object>& matchedData, string filename) {
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
