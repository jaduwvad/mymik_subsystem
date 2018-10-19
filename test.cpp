#include <iostream>
#include <curl/curl.h>
#include <typeinfo>
#include <ThreatSrcData.h>
#include <list>
#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include <fstream>

using namespace std;
using namespace ThreatSrcData;


int main(void) {
    list<string> l;
    namespace bio = boost::iostreams;
    string url = "https://productdata.awin.com/datafeed/download/apikey/2cfea05a4ea7a32e4edfc8faef6d2e28/fid/23591/format/csv/language/de/delimiter/%2C/compression/gzip/columns/data_feed_id%2Cmerchant_id%2Cmerchant_name%2Caw_product_id%2Caw_deep_link%2Caw_image_url%2Caw_thumb_url%2Ccategory_id%2Ccategory_name%2Cbrand_id%2Cbrand_name%2Cmerchant_product_id%2Cmerchant_category%2Cean%2Cproduct_name%2Cdescription%2Cspecifications%2Cmerchant_deep_link%2Cmerchant_thumb_url%2Cmerchant_image_url%2Cdelivery_time%2Csearch_price%2Cdelivery_cost%2Cstock_quantity%2Ccolour%2Ccustom_1%2Clarge_image%2Cproduct_short_description%2Cstock_status/";
    CurlHandler ch(url, true);
/*
    ifstream file("result", ios_base::in | ios_base::binary);
    ofstream outfile("result.txt");
    filtering_streambuf<input> in;
    in.push(gzip_decompressor());
    in.push(file);
    copy(in, outfile);
*/
    CSVHandler csvh("result", ";", true);
    CSVHandler csvout("result.csv", ";", false);

    //csvh.readLine(l);
    //csvout.writeLine(r);
    return 0;
}

