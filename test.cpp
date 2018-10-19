#include <iostream>
#include <curl/curl.h>
#include <typeinfo>
#include <ThreatSrcData.h>
#include <list>

using namespace std;
using namespace ThreatSrcData;


int main(void) {
    list<string> l;
    string url = "http://productdata-download.affili.net/affilinet_products_5308_690740.CSV?auth=QLjXj6kj2bC09U9Cmf10&type=CSV";
    CurlHandler ch(url);
    CSVHandler csvh("result", ";", true);
    CSVHandler csvout("result.csv", ";", false);

    csvh.readLine(l);
    list<string> r;
    r.push_back(l.front());
    l.pop_front();
    r.push_back(l.front());
    l.pop_front();
    r.push_back(l.front());
    l.pop_front();
    csvout.writeLine(r);
    return 0;
}

