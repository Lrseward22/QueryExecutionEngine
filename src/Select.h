#ifndef SELECT_H
#define SELECT_H

#include <vector>
#include <stdexcept>
#include <variant>
using namespace std;
#include "Record.h"
#include "Operator.h"
#include "FileReader.h"

using DataType = variant<int, double, string>;

class Select : public Operator {
    private:
        Operator *op;
        bool ownsOp;
        string attr_name;
        string operations;
        string values;

        vector<Record> inPage;
        int index = 0;

    public:
        Select(string fileName, string p_attr, string p_op, string p_val) {
            FileReader* fileReader = new FileReader(fileName);
            op = fileReader;
            ownsOp = true;
            attr_name = p_attr;
            operations = p_op;
            values = p_val;
        }
        Select(Operator *op, string p_attr, string p_op, string p_val) {
            this->op = op;
            ownsOp = false;
            attr_name = p_attr;
            operations = p_op;
            values = p_val;
        }

        void open() {
            op->open();
        }

        vector<Record> next() {
            vector<Record> outPage;
            fillOutPage(&outPage);
            return outPage;
        }

        void close() {
            op->close();
        }

        string getType() {
            return "Select";
        }

        ~Select() {
            if(ownsOp)
                delete op;
        }

    private:

        bool full(vector<Record> page) {
            return page.size() >= pageSize;
        }

        bool viableRecord(Record record) {
            if(operations == "<") {
                return record.getAttributeByName(attr_name) < values;
            } else if(operations == "<=") {
                return record.getAttributeByName(attr_name) <= values;
            } else if(operations == ">") {
                return record.getAttributeByName(attr_name) > values;
            } else if(operations == ">=") {
                return record.getAttributeByName(attr_name) >= values;
            } else if(operations == "=") {
                return record.getAttributeByName(attr_name) == values;
            } else if(operations == "!=") {
                return record.getAttributeByName(attr_name) != values;
            } else {
                throw runtime_error(operations + " is not a supported operation");
            }
        }

        void fillOutPage(vector<Record>* outPage) {
            while (!full(*outPage)) {
                if (inPage.empty())
                    inPage = op->next();
                if (inPage.empty())
                    return;
                if (viableRecord(inPage[index]))
                    outPage->push_back(inPage[index]);
                incrementIndex();
            }
        }

        void incrementIndex() {
            index++;
            if (index == pageSize || index == inPage.size()) {
                inPage = op->next();
                index = 0;
            }
        }
};

#endif
