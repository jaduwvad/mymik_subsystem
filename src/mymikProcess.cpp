#include <ctime>
#include "mymikprocess.h"

using namespace std;

void MymikProcess::getConfig(Json::Array& configData){
    ifstream ifile("/tmp/mymik/data/shop_data.json");
    string temp;
    string result = "";

    while(getline(ifile, temp))
        result += temp+"\n";

    Json::Object jo;
    jo.addMember(result.c_str(), result.c_str()+result.size());
    configData = jo["shops"].getArray();
}

string MymikgetSupplierNumber(string spn){
    string result = spn;

    if(result[0] == 'P')
        result = result.substr(1, result.length()-1);

    while(result[0] == '0')
        result = result.substr(1, result.length()-1);

    return result;
}

bool myCompare(Json::Object a, Json::Object b){
    return a["supplierNumber"].getString() < b["supplierNumber"].getString();
}

bool myCompare2(Json::Object a, Json::Object b){
    string spna = a["ArtNumber"].getString();
    string spnb = b["ArtNumber"].getString();
    return getSupplierNumber(spna) < getSupplierNumber(spnb);
}

void configShop(Json::Object shop){
    Json::Array urls = shop["url"].getArray();
    Json::Array::const_iterator ci = urls.begin();
    vector<Json::Object> esData;

    cout<<shop["shop"].getString()<<endl;

    getESData(esData, shop["tag"].getString());

    do{
        SrcDataFile ch((*ci).getString(), shop["file"].getString(), ';');

        vector<Json::Object> an;
        vector<Json::Object> result;

        vector<string> headers;
        headers.push_back("ArtNumber");
        headers.push_back("Price");

        ch.readColumn(headers, an);

        sort(an.begin(), an.end(), myCompare2);
        sort(esData.begin(), esData.end(), myCompare);

        cout<<an.at(0)["ArtNumber"].getString()<<endl;
        cout<<esData.at(0)["supplierNumber"].getString()<<endl;
        matchingList(an, esData, result);
        cout<<result.size()<<endl;

        an.clear();
        result.clear();

        ++ci;
    } while(ci != urls.end());

    cout<<esData.at(1).pretty()<<endl;
    esData.clear();
}

void matchingList(vector<Json::Object>& src, vector<Json::Object>& dst, vector<Json::Object>& result){
    int dstIndex = 0;

    for(int i=0; i<src.size() && dstIndex<dst.size(); i++){
        string srcNumber = src.at(i)["ArtNumber"].getString();
        string dstNumber = dst.at(dstIndex)["supplierNumber"].getString();

        srcNumber = getSupplierNumber(srcNumber);
        if(srcNumber>dstNumber){
            dstIndex++;
            i--;
        }
        else if(srcNumber == dstNumber){
            result.push_back(dst.at(dstIndex));
            dstIndex++;
        }
    }
}

void setPriceInven(vector<Json::Object>& src, vector<Json::Object>& dst){
    int dstIndex = 0;

    for(int i=0; i<src.size(); i++){
        if(src.at(i)["artNumber"].getString() == dst.at(dstIndex)["artNumber"].getString()){
            if(src.at(i)["price"].getString() == dst.at(dstIndex)["price"].getString()){

            }
            dstIndex++;
        } else {

        }
    }
}


MymikProcess::showTime(){
    time_t curr_time = time(NULL);
    struct tm *curr_tm = localtime(&curr_time);
    cout << curr_tm->tm_hour << "시 " << curr_tm->tm_min << "분 " << curr_tm->tm_sec << "초" << endl;
}
