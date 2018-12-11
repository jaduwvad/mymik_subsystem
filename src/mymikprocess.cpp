#include <ctime>
#include <fstream>
#include <algorithm>
#include "mymikprocess.h"
#include "srcdatafile.h"

using namespace std;

MymikProcess::MymikProcess():
    _shopDataFile("/tmp/mymik/data/shop_data.json"),
    _resultDataDir("/tmp/mymik/results/"),
    _configMainTag("shops") { }

MymikProcess::~MymikProcess() { }

void MymikProcess::getShopData(Json::Array& shopData){
    ifstream ifile(_shopDataFile);
    Json::Object jo;
    string lineTemp;
    string result = "";

    while(getline(ifile, lineTemp))
        result += lineTemp+"\n";

    jo.addMember(result.c_str(), result.c_str()+result.size());
    shopData = jo[_configMainTag].getArray();
}

void MymikProcess::getESData(vector<Json::Object>& esData, string tag, Json::Array esfield){
    Json::Array resultArray;
    vector<string> fields;
    ThreatShopData::ESHandler es;

    Json::Array::const_iterator ci = esfield.begin();

    do{
        fields.push_back((*ci).getString());
        ci = ++ci;
    }while(ci != esfield.end());

    es.getArticlesByTag(tag, resultArray);
    es.getFieldList(resultArray, fields, esData);

    resultArray.clear();
}

void MymikProcess::getSrcData(string url, string filename, vector<Json::Object>& srcData, Json::Array srcColumn) {
    ThreatShopData::SrcDataFile ch(url, filename, ';');
    vector<string> headers;
    Json::Array::const_iterator ci = srcColumn.begin();

    do{
        headers.push_back((*ci).getString());
        ci = ++ci;
    }while(ci != srcColumn.end());

    ch.readColumn(headers, srcData);
    headers.clear();
}

void MymikProcess::setUpdatedData(Json::Object& updatedData, Json::Object& srcData, string price, bool priceUpdate) {
    updatedData.addMemberByKey("variantid", srcData["variantid"].getString());
    updatedData.addMemberByKey("ordernumber", srcData["ordernumber"].getString());
    updatedData.addMemberByKey("price", price);
    updatedData.addMemberByKey("priceUpdate", priceUpdate);
}

void MymikProcess::matchingList(vector<Json::Object>& srcData, vector<Json::Object>& esData, vector<Json::Object>& result) {
    int esDataIndex = 0;
    int srcDataSize = srcData.size();
    int esDataSize = esData.size();

    for(int i=0; i<srcDataSize && esDataIndex < esDataSize; i++){
        string srcArtNumber = srcData.at(i)["ArtNumber"].getString();
        string esDataSpn = esData.at(esDataIndex)["ordernumber"].getString();

        Json::Object updatedData;

        if(srcArtNumber>esDataSpn){
            esDataIndex++;
            i--;
        }
        else if(srcArtNumber == esDataSpn){
            string price = srcData.at(i)["Price"].getString();

            if(srcData.at(i)["Price"].getString() != esData.at(esDataIndex)["price"].getString())
                setUpdatedData(updatedData, esData.at(esDataIndex), price, true);
            else
                setUpdatedData(updatedData, esData.at(esDataIndex), price, false);
            result.push_back(updatedData);
            esDataIndex++;
        }
    }
}

void MymikProcess::setPriceInven(vector<Json::Object>& matchedData, string filename) {
    int matchedDataSize = matchedData.size();

    ofstream priceFile(_resultDataDir + "price_" + filename);
    ofstream invenFile(_resultDataDir + "inven_" + filename);

    string priceData;
    string invenData;

    for(int i=0; i < matchedDataSize; i++){
        Json::Object matchedObject = matchedData.at(i);

        invenFile<<matchedObject["variantid"].getString()<<endl;

        if(matchedObject["priceUpdate"].getBoolean()){
            priceFile<<matchedObject["variantid"].getString()<<",";
            priceFile<<matchedObject["price"].getString()<<endl;
        }
    }

    priceFile.close();
    invenFile.close();
}

void MymikProcess::showTime(){
    time_t curr_time = time(NULL);
    struct tm *curr_tm = localtime(&curr_time);
    cout << curr_tm->tm_hour << "시 " << curr_tm->tm_min << "분 " << curr_tm->tm_sec << "초" << endl;
}

string MymikProcess::formSupplierNumber(string spn){
    string result = spn;

    if(result[0] == 'P')
        result = result.substr(1, result.length()-1);

    while(result[0] == '0')
        result = result.substr(1, result.length()-1);

    return result;
}





