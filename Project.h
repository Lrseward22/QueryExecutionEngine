#ifndef PROJECT_H
#define PROJECT_H

#include <vector>
using namespace std;
#include "Record.h"
#include "Operator.h"
#include "FileReader.h"

class Project : public Operator {
    private:
        Operator *op;
        vector<string> keptAttributes;
        bool ownsOp;

    public:
        Project(string fileName, vector<string> fieldNames) {
            FileReader* fileReader = new FileReader(fileName);
            op = fileReader;
            keptAttributes = fieldNames;
            ownsOp = true;
        }
        Project(Operator *op, vector<string> fieldNames) {
            this->op = op;
            keptAttributes = fieldNames;
            ownsOp = false;
        }

        void open() {
            op->open();
        }

        vector<Record> next() {
            vector<Record> outPage;
            vector<Record> inPage = op->next();
            for(auto& record : inPage) {
                vector<Attribute> attributesToKeep = cullRecord(record);
                outPage.emplace_back(Record(attributesToKeep));
            }
            return outPage;
        }

        void close() {
            op->close();
        }

        string getType() {
            return "Project";
        }

        ~Project() {
            if(ownsOp)
                delete op;
        }

    private:
        vector<Attribute> cullRecord(Record record) {
            vector<Attribute> attributesToKeep;
            for(auto keptAttribute : keptAttributes) {
                attributesToKeep.emplace_back(record.getAttributeByName(keptAttribute));
            } 
            return attributesToKeep;
        }
};

#endif
