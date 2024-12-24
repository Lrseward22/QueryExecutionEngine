#include <iostream>
#include <string>
#include <variant>
#include <stdexcept>
using namespace std;

using DataType = variant<int, double, string>;

class Attribute {
    public:
        string name;
        DataType value;

        Attribute(string _name, string _type, string _value) {
            name = _name;
            if(_type == "Double") {
                value = stod(_value);
            } else if(_type == "Integer") {
                value = stoi(_value);
            } else if(_type == "String") {
                value = _value;
            }
            else {
                throw runtime_error(_type + " is not supported in Attributes");
            }
        }

        string getType() {
            int index = value.index();
            switch (index) {
                case 0:
                    return "Integer";
                case 1:
                    return "Double";
                default:
                    return "String";
            }
        }

        string toString() {
            int index = value.index();
            switch (index) {
                case 0:
                    return to_string(get<int>(value));
                case 1: {
                    string str = to_string(get<double>(value));
                    while (str.back() == '0') {
                        str.pop_back();
                        if (str.back() == '.') {
                            str.pop_back();
                            break;
                        }
                    }
                    return str;
                        }
                default:
                    return get<string>(value);
            }
        }

        friend ostream& operator<< (ostream& os, const Attribute& a) {
            int index = a.value.index();
            if(index == 0) {
                os << get<int>(a.value);
            } else if(index == 1) {
                os << get<double>(a.value);
            } else {
                os << get<string>(a.value);
            }
            return os;
        }

        bool operator<(string val) {
            DataType newVal = toDataType(val);
            return value < newVal;
        }

        bool operator>(string val) {
            DataType newVal = toDataType(val);
            return value > newVal;
        }

        bool operator==(string val) {
            DataType newVal = toDataType(val);
            return value == newVal;
        }

        bool operator==(Attribute a) {
            return value == a.value;
        }

        bool operator!=(string val) {
            DataType newVal = toDataType(val);
            return value != newVal;
        }

        bool operator<=(string val) {
            DataType newVal = toDataType(val);
            return value <= newVal;
        }

        bool operator>=(string val) {
            DataType newVal = toDataType(val);
            return value >= newVal;
        }

    private:

        DataType toDataType(string val) {
            string type = getType();
            try {
                if(type == "Integer") {
                    return stoi(val);
                } else if(type == "Double") {
                    return stod(val);
                } else {
                    return val;
                }
            } catch (const exception& e) {
                cerr << "Error: " << e.what() << endl;
                return {};
            }
        }
};
