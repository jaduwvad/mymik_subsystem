#include <algorithm>
#include <stdio.h>
#include "srcdatafile.h"
#include "json.h"

using namespace std;

/**
 * @brief	Main constructor.
 * @description	Get filename to download and handle.
 * 		Get delimiter for csv file.
 * 		Download file by url in filename.
 */
SrcDataFile::SrcDataFile(string url, string filename, const char delimiter) {
    this->_filename = filename;
    this->_delimiter = delimiter;
    setCurl(url);
    setFileStream();
}

/**
 * @brief	Constructor with login data.
 * @description	To download file by url, it needs to login.
 * 		So, it use another download function.
 */
SrcDataFile::SrcDataFile(string url, string filename, const char delimiter, string userId, string userPw){
    this->_filename = filename;
    this->_delimiter = delimiter;
    setCurl(url, userId, userPw);
    setFileStream();
}

/**
 * @brief	Constructor with unzip function
 * @description	Downloaded file is zipped by gzip.
 * 		So, it use unzip function after download.
 */
SrcDataFile::SrcDataFile(string url, string filename, const char delimiter, bool zipped){ 
    this->_filename = filename + ".gz";
    this->_delimiter = delimiter;
    setCurl(url);

    string cmd = "gzip -df " + this->_filename;

    system(cmd.c_str());
    this->_filename = filename;
    setFileStream();
}

/**
 * @brief	Destructor
 * @description	Close filestream
 * 		Delete downloaded file
 */
SrcDataFile::~SrcDataFile(){
    _srcFile.close();
    remove(_filename.c_str());
}

/**
 * @brief	Open file at filestream
 * @description	Make filestream and open downloaded file
 * 		Get first line that is header
 * 		and save header data after explode header
 */
void SrcDataFile::setFileStream(){
    string headerStr;
    _srcFile.open(_filename.c_str());
    getline(_srcFile, headerStr);
    myExplode(headerStr, _header);
    _filePointer = _srcFile.tellg();
}

/**
 * @brief	Get data from downloaded file
 * @parameter	columnHeaders : column's header name by parameter
 * 		result : specific data from downloaded file
 * @description	Get index of header's location at list
 * 		Explode a row from filestream
 *		Append the data from row at result vector
 */
void SrcDataFile::readColumn(vector<string> columnHeaders, vector<Json::Object>& result){
    vector<int> headerList;
    string strTemp;
    vector<string> listTemp;

    for(string header:columnHeaders){
        auto it = find(_header.begin(), _header.end(), header);

        if(_header.end() == it)
            return;

        headerList.push_back( distance( _header.begin(), it ) );
    }

    while(getline(_srcFile, strTemp)){
        myExplode(strTemp, listTemp);
        Json::Object jo;

        if(listTemp.size() != _header.size()) {
            listTemp.clear();
            continue;
        }

        for(int i=0; i<columnHeaders.size(); i++)
            jo.addMemberByKey(columnHeaders.at(i), listTemp.at(headerList.at(i)));

        result.push_back(jo);
        listTemp.clear();
    }

    _srcFile.clear();
    _srcFile.seekg(_filePointer, ios::beg);
}

/**
 * @brief	Download file by url
 * @description	Set curl option
 * 		Make file and open to write data
 *		Download file
 */
void SrcDataFile::setCurl(const string url){
    FILE *fp = fopen(_filename.c_str(), "w");
    CURL *curl;
    curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeData);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
    curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    fclose(fp);
}

/**
 * @brief	download file with login
 * @description	Set curl option
 * 		Add login data in option
 * 		and set CURLOPT_FOLLOWLOCATION to 1
 */
void SrcDataFile::setCurl(const string url, string userId, string userPw) {
    CURL *curl;
    CURLcode res;
    FILE *fp = fopen(_filename.c_str(), "w");

    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();

    if(curl) {
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Accept: text/plain");

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPAUTH, (long)CURLAUTH_BASIC);
        curl_easy_setopt(curl, CURLOPT_USERNAME, userId.c_str());
        curl_easy_setopt(curl, CURLOPT_PASSWORD, userPw.c_str());
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeData);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        res = curl_easy_perform(curl);
            
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
        curl_global_cleanup();
    }

    return;
}

/**
 * @brief	Separate exploding process
 * @description	If csv file use '"' for data more than one, return true
 * 		else return false
 */
bool SrcDataFile::checkQuotes(string s){
    for( int i = 1; i < s.length(); i++) {
        char n = s.at(i);
        if(n == '"' && s.at(i-1) == _delimiter){
            for(int j = i+1; j < s.length(); j++){
                if(s.at(j) == '"') {
                    if(j == s.length() - 1)
                        return true;
                    else if(s.at(j+1) == _delimiter)
                        return true;
                }
            }
        }
    }
    return false;
}

/**
 * @brief	Explode string by delimiter
 * @parameter	s : downloaded file's a row
 * 		result : vector to save and return exploded data
 * @description	Check the string if it use '"' or not.
 * 		Separate the string by delimiter
 */
void SrcDataFile::myExplode(string s, vector<string>& result) {
    string buff("");
    bool isQuoteUsed = checkQuotes(s);

    if( isQuoteUsed ){
        bool inQuote = false;
        for( int i=0; i<s.length(); i++) {
            char n = s.at(i);

            if(n == '"' && !inQuote){
                inQuote = true;
            }
            else if(n == '"' && inQuote) {
                if(i == s.length() -1){
                    inQuote = false;
                    result.push_back(buff);
                    buff = "";
                    i++;
                }
                else if(s.at(i+1) == '"') {
                    buff += "\"\"";
                    i++;
                }
                else {
                    inQuote = false;
                    result.push_back(buff);
                    buff = "";
                    i++;
                }
            }
            else if(n == _delimiter && !inQuote){
                result.push_back(buff);
                buff = "";
            }
            else
                buff += n;
        }
        if(buff != "") result.push_back(buff);
    }
    else{
        for(char n:s){
            if(n == _delimiter){
                result.push_back(buff);
                buff = "";
            }
            else 
                buff += n;
        }
        if(buff != "" || s.at(s.length()-1) == _delimiter) result.push_back(buff);
    }
}
