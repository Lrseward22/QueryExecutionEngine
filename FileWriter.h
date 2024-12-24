
#ifndef FILEWRITER_H
#define FILEWRITER_H

#include <vector>
#include <sstream>
#include <fstream>
using namespace std;
#include "Record.h"
#include "Operator.h"

class FileWriter: public Operator {
    private:
        ofstream dataFile;
        string filename;
        bool headerWritten;

    public:
        FileWriter(string fileName) {
            filename = fileName;
            headerWritten = false;
        }
        FileWriter() {
            headerWritten = false;
        }

        void open() {
            dataFile.open(filename);
        }

        vector<Record> next() {
            vector<Record> outPage;
            return outPage;
        }

        void close() {
            dataFile.close();
        }

        string getType() {
            return "FileWriter";
        }

        void write(vector<Record> records) {
            if (!headerWritten && !records.empty())
                writeHeader(records[0]);
            for (auto& record : records) {
                dataFile << record.toString() + '\n';
            }
        }

    private:
        void writeHeader(Record record) {
            for (auto& str : record.getSchema()) {
                dataFile << str + '\n';
            }
            headerWritten = true;
        }

};

#endif
