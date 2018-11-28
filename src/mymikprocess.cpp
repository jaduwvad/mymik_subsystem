#include <ctime>
#include <fstream>
#include "mymikprocess.h"

using namespace std;

MymikProcess::MymikProcess():
    shopDataFile("/tmp/mymik/data/shop_data.json"),
    esSpnColumn("supplierNumber"),
    srcArtNumberColumn("ArtNumber"),
    configMainTag("shops") { }

MymikProcess::~MymikProcess() { }

void MymikProcess::getShopData(Json::Array& shopData){
    ifstream ifile(shopDataFile);
    Json::Object jo;
    string lineTemp;
    string result = "";

    while(getline(ifile, lineTemp))
        result += lineTemp+"\n";

    jo.addMember(result.c_str(), result.c_str()+result.size());
    shopData = jo[configMainTag].getArray();
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

void MymikProcess::getESData(vector<Json::Object>& esData, string tag){
    Json::Array resultArray;
    vector<string> fields;
    ThreatShopData::ESHandler es;

    fields.push_back("artNumber");
    fields.push_back("supplierNumber");
    fields.push_back("price");
    fields.push_back("variantID");

    es.getArticlesByTag(tag, resultArray);
    es.getFieldList(resultArray, fields, esData);

    resultArray.clear();
}

void MymikProcess::matchingList(vector<Json::Object>& srcData, vector<Json::Object>& esData, vector<Json::Object>& result){
    int esDataIndex = 0;

    for(int i=0; i<srcData.size() && esDataIndex < esData.size(); i++){
        string srcArtNumber = srcData.at(i)["ArtNumber"].getString();
        string esDataSpn = esData.at(esDataIndex)["supplierNumber"].getString();

        srcArtNumber = formSupplierNumber(srcArtNumber);
        if(srcArtNumber>esDataSpn){
            esDataIndex++;
            i--;
        }
        else if(srcArtNumber == esDataSpn){
            result.push_back(esData.at(esDataIndex));
            esDataIndex++;
        }
    }
}








