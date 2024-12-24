#ifndef RECORD_H
#define RECORD_H

#include <iostream>
#include <vector>
#include <stdexcept>
#include "Attribute.h"
using namespace std;

class Record {
    public:
        vector<Attribute> attributes;

        Record(vector<string> names, vector<string> types, vector<string> values) {
            int numAttributes = size(names);
            for(int i=0; i<numAttributes; ++i) {
                attributes.emplace_back(names[i], types[i], values[i]);
            }
        }

        Record(vector<Attribute> _attributes) {
            attributes = _attributes;
        }

        Attribute getAttributeByName(string attrName) {
            for(auto attribute : attributes) {
                if(attrName == attribute.name)
                    return attribute;
            }
            throw runtime_error("Attribute not found: " + attrName);
        }

        friend ostream& operator<< (ostream& os, const Record& r) {
            for(const auto& attribute: r.attributes) {
                os << attribute << " ";
            }
            return os;
        }

        Record operator+(const Record& record) {
            vector<Attribute> newAttributes;
            newAttributes.insert(newAttributes.end(), this->attributes.begin(), this->attributes.end());
            newAttributes.insert(newAttributes.end(), record.attributes.begin(), record.attributes.end());
            return Record(newAttributes);
        }

        string toString() {
            string str = "";
            for (auto& attribute : attributes) {
                str = str + attribute.toString() + ",";
            }
            str.pop_back();
            return str;
        }

        vector<string> getSchema() {
            vector<string> schema;
            string attrNames = "";
            string attrTypes = "";
            string junk = "----------------------";
            for (auto& attr : attributes) {
                attrNames = attrNames + attr.name + ',';
                attrTypes = attrTypes + attr.getType() + ',';
            }
            attrNames.pop_back();
            attrTypes.pop_back();
            schema.push_back(attrNames);
            schema.push_back(attrTypes);
            schema.push_back(junk);
            return schema;
        }

};

#endif
