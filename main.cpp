#include <ctime>
#include <algorithm>
#include <cstring>
#include "srcdatafile.h"
#include "mymikprocess.h"
#include "connection.h"

using namespace std;
using namespace ThreatShopData;

MymikProcess mp;
void configShop(Json::Object shop);

void sendUpdateSignal(string tag, string filename){
    Connection c;
    string message = tag.substr(0, tag.length()-1);

    c.sendFile("price_"+filename);
    c.sendFile("inven_"+filename);
    c.sendMessages(message);
}

int main() {
    /*
    Connection c;

    c.sendFile("asdf");
    */
    Json::Array shopData;
    mp.getShopData(shopData);

    Json::Array::const_iterator ci = shopData.begin();

    do{
        Json::Object shop = (*ci).getObject();
        ci = ++ci;
        configShop(shop);
    }while(ci != shopData.end());
}

void configShop(Json::Object shop){
    Json::Array urls = shop["url"].getArray();
    Json::Array::const_iterator ci = urls.begin();
    vector<Json::Object> esData;
    vector<Json::Object> srcData;
    vector<Json::Object> matchedData;

    cout<<shop["shop"].getString()<<endl;
    mp.getESData(esData, shop["tag"].getString());

    sort(esData.begin(), esData.end(), [](Json::Object a, Json::Object b) {
        return a["ordernumber"].getString() < b["ordernumber"].getString();
    });

    do{
        mp.getSrcData(ci, shop["file"].getString(), srcData);

        sort(srcData.begin(), srcData.end(), [](Json::Object a, Json::Object b) {
            return a["ArtNumber"].getString() < b["ArtNumber"].getString();
        });

        mp.matchingList(srcData, esData, matchedData);

        srcData.clear();
        ++ci;
    } while(ci != urls.end());

    cout<<matchedData.size()<<endl;

    mp.setPriceInven(matchedData, shop["file"].getString());

    matchedData.clear();
    esData.clear();

    sendUpdateSignal(shop["tag"].getString(), shop["file"].getString());
}

