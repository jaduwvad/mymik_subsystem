#include <algorithm>
#include <thread>

#include "srcdatafile.h"
#include "mymikprocess.h"
#include "connection.h"

using namespace std;
using namespace ThreatShopData;

MymikProcess mp;
string esId = "ordernumber";
string srcId;

void configShop(Json::Object shop);
void sendPriceResultFile(string filename);
void sendInvenResultFile(string filename);

void sendUpdateSignal(string tag){
    Connection c;
    string message = tag.substr(0, tag.length()-1);

    c.sendMessages(message);
}


int main() {
    string url = "https://productdata.awin.com/datafeed/download/apikey/2cfea05a4ea7a32e4edfc8faef6d2e28/fid/23591/format/csv/language/de/delimiter/%2C/compression/gzip/columns/data_feed_id%2Cmerchant_id%2Cmerchant_name%2Caw_product_id%2Caw_deep_link%2Caw_image_url%2Caw_thumb_url%2Ccategory_id%2Ccategory_name%2Cbrand_id%2Cbrand_name%2Cmerchant_product_id%2Cmerchant_category%2Cean%2Cproduct_name%2Cdescription%2Cspecifications%2Cmerchant_deep_link%2Cmerchant_thumb_url%2Cmerchant_image_url%2Cdelivery_time%2Csearch_price%2Cdelivery_cost%2Cstock_quantity%2Ccolour%2Ccustom_1%2Clarge_image%2Cproduct_short_description%2Cstock_status/";
    SrcDataFile sdf(url, "wave.csv", ';', true);

    /*
    Connection c;

    c.sendFile("asdf");
    Json::Array shopData;
    mp.getShopData(shopData);

    Json::Array::const_iterator ci = shopData.begin();

    do{
        Json::Object shop = (*ci).getObject();
        ci = ++ci;
        configShop(shop);
    }while(ci != shopData.end());
    */
}

void configShop(Json::Object shop){
    Json::Array urls = shop["url"].getArray();
    Json::Array::const_iterator ci = urls.begin();
    vector<Json::Object> esData;
    vector<Json::Object> srcData;
    vector<Json::Object> matchedData;

    cout<<shop["shop"].getString()<<endl;
    mp.getESData(esData, shop["tag"].getString(), shop["esfield"].getArray());

    sort(esData.begin(), esData.end(), [](Json::Object a, Json::Object b) {
        return a[esId].getString() < b[esId].getString();
    });

    do{
        srcId = shop["srcId"].getString();
        mp.getSrcData((*ci).getString(), shop["file"].getString(), srcData, shop["srcColumn"].getArray());

        sort(srcData.begin(), srcData.end(), [](Json::Object a, Json::Object b) {
            return a[srcId].getString() < b[srcId].getString();
        });

        mp.matchingList(srcData, esData, matchedData);

        srcData.clear();
        ++ci;
    } while(ci != urls.end());

    cout<<matchedData.size()<<endl;

    mp.setPriceInven(matchedData, shop["file"].getString());

    matchedData.clear();
    esData.clear();

    //sendPriceResultFile(shop["file"].getString());
    //sleep(1);
    //sendInvenResultFile(shop["file"].getString());

    //sendUpdateSignal(shop["tag"].getString());
}

void sendPriceResultFile(string filename) {
    Connection c;

    c.sendFile("price_" + filename);
}

void sendInvenResultFile(string filename) {
    Connection c;

    c.sendFile("inven_" + filename);
}
