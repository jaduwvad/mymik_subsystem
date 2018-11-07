#include <iostream>
#include <curl/curl.h>
#include <typeinfo>
#include <vector>
#include <fstream>
#include "curlhandler.h"
#include "csvhandler.h"
#include "eshandler.h"

using namespace std;
using namespace ThreatShopData;


int main(void) {
    vector<string> l;
    char delimiter = ';';
    string filename = "text";
    string url = "http://productdata-download.affili.net/affilinet_products_990_853709.CSV?auth=gzTDyE8pfzsYbbUgPHQU&type=CSV";
    CurlHandler ch(url, filename);
    //CurlHandler ch(url2);

    //CSVHandler csvh("result", ";", true);
    //CSVHandler csvout("result.csv", ";", false);

    //csvh.readLine(l);
    //csvout.writeLine(r);
    return 0;
}

