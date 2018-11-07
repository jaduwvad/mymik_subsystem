#include <map>
#include <list>
#include <string>

namespace Json {
    typedef std::string Key;

    class Object;
    class Array;

    class Value {
    public:
        enum ValueType{ objectType, arrayType, stringType, booleanType, numberType, nullType };

        Value();
        Value(const Value& val);
        ~Value();

        const char* showType() const;
        const char* read(const char* pStart, const char* pEnd);
        bool operator==(const Value& v) const;
        bool operator!=(const Value& other) const {
            return !operator==(other);
        }

        const std::string& getString() const;

        operator const std::string&() const;

        void show() const;
        bool empty() const;
        unsigned int getUnsignedInt() const;

        bool getBoolean() const;

        operator bool() const;

        int getInt() const;

        operator int() const;

        long int getLong() const;

        double getDouble() const;

        operator double() const;

        float getFloat() const;

        operator float() const;

        const Object& getObject() const;
        const Array& getArray() const;

        void setBoolean(bool b);

        void setString(const std::string& value);

        void setObject(const Json::Object& obj);

        void setArray(const Json::Array& array);

        void setDouble(double v);

        void setInt(unsigned int u);

        void setInt(int u);

        void setLong(long l);

        friend std::ostream& operator<<(std::ostream& os, const Value& value);

        const std::string& data() const {return _data;}

        static std::string escapeJsonString(const std::string& input);

        static bool weakEquality(const Json::Value& a, const Json::Value& b);

        inline bool isNull() const {return (_type == nullType);}
        inline bool isObject() const {return (_type == objectType);}
        inline bool isArray() const {return (_type == arrayType);}

        std::string pretty(int tab = 0) const;

    private:
        ValueType _type;
        std::string _data;
        Object* _object;
        Array* _array;
    };

    class Object {
    public:
        Object();
        Object(const Object& obj);

        const char* addMember(const char* startPtr, const char* endStr);

        void addMemberByKey(const std::string& key, const std::string& str);
        void addMemberByKey(const std::string& key, const Json::Array& array);
        void addMemberByKey(const std::string& key, const Json::Object& obj);
        void addMemberByKey(const std::string& key, const Json::Value& value);
        void addMemberByKey(const std::string& key, double v);
        void addMemberByKey(const std::string& key, bool v);
        void addMemberByKey(const std::string& key, const char* s);
        void addMemberByKey(const std::string& key, unsigned int u);
        void addMemberByKey(const std::string& key, int i);
        void addMemberByKey(const std::string& key, long i);
        void addMemberByKey(const std::string& key, unsigned long i);

        void clear() { _memberMap.clear(); }

        bool empty() const { return _memberMap.empty();}

        size_t size() const { return _memberMap.size();}

        bool member(const std::string& key) const;

        void append(const Object& obj);

        const Value& getValue(const std::string& key) const;

        const Value& operator[](const std::string& key) const noexcept;

        friend std::ostream& operator<<(std::ostream& os, const Object& obj);

        std::string str() const;

        std::string pretty(int tab = 0) const;

        bool contain(const Object& o) const;

        bool operator==(const Object& v) const;
        bool operator!=(const Object& other) const {
            return !operator==(other);
        }

        class const_iterator {
            std::map< Key, Value >::const_iterator _it;
        public:
            const_iterator(const std::map< Key, Value >::const_iterator& it) : _it(it) {}
            const_iterator(const const_iterator& it) : _it(it._it) {}
            const_iterator& operator++() {
                ++_it;
                return *this;
            }
            const Key& operator*() const {return _it->first;}
            const Key& key() const {return _it->first;}
            const Value& value() const {return _it->second;}
            bool operator!=(const const_iterator& rhs) {return _it != rhs._it;}
        };

        const const_iterator begin() const { return const_iterator(_memberMap.cbegin()); }
        const const_iterator end() const { return const_iterator(_memberMap.cend()); }

    private:
        std::map< Key, Value > _memberMap;
    };


    class Array {
    public:
        Array();

        const char* addElement(const char* startPtr, const char* endStr);

        void addElement(const Json::Value& val);

        void addElement(const Json::Object& obj);

        size_t size() const { return _elementList.size();}

        void clear() { return _elementList.clear();}

        bool empty() const { return _elementList.empty();}

        const Value& first() const { return _elementList.front(); }

        bool operator==(const Array& v) const;
        bool operator!=(const Array& other) const {
            return !operator==(other);
        }

        friend std::ostream& operator<<(std::ostream& os, const Array& array);

        std::string str() const;

        class const_iterator {
            std::list<Value>::const_iterator _it;
        public:
            const_iterator(const std::list<Value>::const_iterator& it) : _it(it) {}
            const_iterator(const const_iterator& it) : _it(it._it) {}
            const_iterator& operator++() {
                ++_it;
                return *this;
            }
            const Value& operator*() const { return *_it; }
            bool operator!=(const const_iterator& rhs) {return _it != rhs._it;}
        };

        const const_iterator begin() const { return const_iterator(_elementList.cbegin()); }
        const const_iterator end() const { return const_iterator(_elementList.cend()); }

        std::string pretty(int tab = 0) const;

    private:
        std::list<Value> _elementList;
    };
}
