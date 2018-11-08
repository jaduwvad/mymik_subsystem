#include <iostream>
#include <cstring>
#include <sstream>
#include <algorithm>
#include "json.h"

using namespace std;

namespace ThreatShopData {

    JsonValue::JsonValue(){}

    JsonValue::JsonValue(JsonValue* v){
        if(v->getJsonObject()){
            this->_type = ValueType::objectType;
            this->_object = new JsonObject(v->getJsonObject());
        }
        else if(v->getJsonArray()){
            this->_type=  ValueType::arrayType;
            this->_array = new JsonArray(v->getJsonArray());
        }
        else{
            this->_type = ValueType::stringType;
            this->_value = v->getValue();
        }
    }


    JsonValue::~JsonValue(){
        if(_object)
            delete _object;

        if(_array)
            delete _array;
    }

    JsonValue::ValueType JsonValue::getType(){ return this->_type; }
    string JsonValue::getValue(){ return this->_value; }
    JsonObject* JsonValue::getJsonObject(){ return _object; }
    JsonArray* JsonValue::getJsonArray(){ return _array; }

    void JsonValue::setValue(string s){
        _type = ValueType::stringType;
        _value = s;
        _object = 0;
        _array = 0;
    }

    void JsonValue::setArray(JsonArray* arr){
        _type = ValueType::arrayType;
        _array = new JsonArray(arr);
        _object = 0;
        
    }

    void JsonValue::setObject( JsonObject* obj){
        _type = ValueType::objectType;
        _object = new JsonObject(obj);
        _array = 0;

    }

    string JsonValue::escapeJsonString(const string& input) {
        stringstream output;

        for (auto iter = input.cbegin(); iter != input.cend(); iter++) {
            switch (*iter) {
                case '\\': output << "\\\\"; break;
                case '"': output << "\\\""; break;
                case '\b': output << "\\b"; break;
                case '\f': output << "\\f"; break;
                case '\n': output << "\\n"; break;
                case '\r': output << "\\r"; break;
                case '\t': output << "\\t"; break;
                default: output << *iter; break;
            }
        }

        return output.str();
    }





    JsonObject::JsonObject(){ }

    JsonObject::JsonObject(JsonObject* obj){ 
        this->_jsonKeys = obj->_jsonKeys;
        this->_jsonValues = obj->_jsonValues;
    }

    JsonObject::~JsonObject(){
        _jsonKeys.clear();
        _jsonValues.clear();
    }

    void JsonObject::addJsonMember(const string k, const string v){
        JsonValue jv;
        jv.setValue(v);
        _jsonKeys.push_back(k);
        _jsonValues.push_back(jv);
    }

    void JsonObject::addJsonMember(string k, JsonValue* v){
        JsonValue jv(v);
        _jsonKeys.push_back(k);
        _jsonValues.push_back(jv);
        cout<<jv.getJsonArray()->toString()<<endl;;
    }

    void JsonObject::addJsonMember(string k, JsonArray* v){
        JsonValue jv;
        jv.setArray(v);
        _jsonKeys.push_back(k);
        cout<<jv.getJsonArray()->toString()<<endl;
        _jsonValues.push_back(jv);
    }

    void JsonObject::addJsonMember(string k, JsonObject* v){
        JsonValue jv;
        jv.setObject(v);
        _jsonKeys.push_back(k);
        _jsonValues.push_back(jv);
    }

    JsonValue* JsonObject::operator[](const string key){
        auto it = find(_jsonKeys.begin(), _jsonKeys.end(), key);
        if(_jsonKeys.end() == it)
            return 0;

        int valuePos = distance(_jsonKeys.begin(), it);

        return &_jsonValues.at(valuePos);
    }

    string JsonObject::toString(){
        string re = "{\n";

        for(int i=0; i<_jsonKeys.size(); i++){
            re += "\"" + _jsonKeys.at(i) + "\":";
            switch(_jsonValues.at(i).getType()){
                case 0:
                    cout<<_jsonValues.at(i).getJsonObject()->toString()<<endl;
                    re += _jsonValues.at(i).getJsonObject()->toString();
                    break;
                case 1:
                    cout<<_jsonValues.at(i).getJsonArray()->toString()<<endl;
                    re += _jsonValues.at(i).getJsonArray()->toString();
                    break;
                case 2:
                    cout<<_jsonValues.at(i).getValue()<<endl;
                    re += "\"" + _jsonValues.at(i).getValue() + "\"";
                    break;
            }
            re += ",\n";
        }
        return re+"}";
    }





    JsonArray::JsonArray(){

    }

    JsonArray::JsonArray(JsonArray* arr){
        this->_jsonValueList = arr->_jsonValueList;
    }

    JsonArray::~JsonArray(){
        this->_jsonValueList.clear();
    }

    void JsonArray::addJsonList(JsonValue v) {
        _jsonValueList.push_back(v);
    }

    void JsonArray::addJsonList(JsonObject* v) {
        JsonValue jv;
        jv.setObject(v);
        _jsonValueList.push_back(jv);
    }

    void JsonArray::addJsonList(string v) {
        JsonValue jv;
        jv.setValue(JsonValue::escapeJsonString(v));
        _jsonValueList.push_back(jv);
    }

    JsonValue* JsonArray::operator[](const int i){
        return &_jsonValueList.at(i);
    }

    string JsonArray::toString(){
        string re = "[\n";
        for(int i=0; i<_jsonValueList.size(); i++){
            switch(_jsonValueList.at(i).getType()){
                case 0:
                    re += _jsonValueList.at(i).getJsonObject()->toString();
                    break;
                case 1:
                    re += _jsonValueList.at(i).getJsonArray()->toString();
                    break;
                case 2:
                    re += "\"" + _jsonValueList.at(i).getValue() + "\"";
                    break;
            }
            re += ",\n";
        }

        return re+"]";
    }
}
