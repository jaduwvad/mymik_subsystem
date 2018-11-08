#pragma once
#include <map>
#include <string>
#include <vector>

namespace ThreatShopData {

    class JsonArray;
    class JsonObject;

    class JsonValue {
    public:
        enum ValueType{ objectType, arrayType, stringType };

        JsonValue();
        JsonValue(JsonValue* v);
        ~JsonValue();

        ValueType getType();
        std::string getValue();
        JsonObject* getJsonObject();
        JsonArray* getJsonArray();

        static std::string escapeJsonString(const std::string& input);
        void setValue(std::string s);
        void setArray(JsonArray* arr);
        void setObject(JsonObject* obj);
    private:
        std::string _value;
        ValueType _type;
        JsonObject *_object;
        JsonArray *_array;
        
    };

    class JsonArray {
    public:
        JsonArray();
        JsonArray(JsonArray* arr);
        ~JsonArray();

        void addJsonList(JsonValue v);
        void addJsonList(JsonObject* v);
        void addJsonList(std::string v);

        JsonValue* operator[](const int i);
        std::string toString();
    private:
        std::vector<JsonValue> _jsonValueList;
    };

    class JsonObject {
    public:
        JsonObject(JsonObject* obj);
        JsonObject();
        ~JsonObject();

        void addJsonMember(const std::string k, const std::string v);
        void addJsonMember(std::string k, JsonValue* v);
        void addJsonMember(std::string k, JsonArray* v);
        void addJsonMember(std::string k, JsonObject* v);

        JsonValue* operator[](const std::string key);
        std::string toString();
    private:
        std::vector<std::string> _jsonKeys;
        std::vector<JsonValue> _jsonValues;
    };
}
