#include <iostream>
#include <curl/curl.h>
#include <typeinfo>
#include <ThreatSrcData.h>
#include <list>
#include <fstream>

using namespace std;
using namespace ThreatSrcData;


int main(void) {
    list<string> l;
    string url2 = "http://productdata-download.affili.net/affilinet_products_6091_690740.CSV?auth=QLjXj6kj2bC09U9Cmf10&type=CSV";
    string url = "http://export.wave-computer.de/redirect.jsp?exportFormat=CSV&exportEncoding=utf-8&compressed=0&cat=-9969929";
    CurlHandler ch(url, "26085804", "geld7914");
    //CurlHandler ch(url2);

    //CSVHandler csvh("result", ";", true);
    //CSVHandler csvout("result.csv", ";", false);

    //csvh.readLine(l);
    //csvout.writeLine(r);
    return 0;
}

