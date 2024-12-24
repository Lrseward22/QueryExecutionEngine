#ifndef FILEREADER_H
#define FILEREADER_H

#include <vector>
#include <sstream>
#include <fstream>
using namespace std;
#include "Record.h"
#include "Operator.h"


// This function takes in a string with items delimited by commas
// and returns a vector of those items.
// Example - input: "str1,str2,str3,str4"
//          output: {"str1", "str2", "str3", "str4"}
vector<string> split(string line) {
    vector<string> v;
    string s;
    istringstream is(line);
    while (getline(is, s, ','))
        v.push_back(s);
    return v;
}


class FileReader : public Operator {
    private:
        string filename;
        ifstream dataFile;
        vector<string> names;
        vector<string> types;
        bool opened;

    public:
        FileReader(string fileName) {
            dataFile.open(fileName);
            filename = fileName;
            opened = true;
            if(!dataFile)
                cout << "There was an issue opening the file." << endl;
        }

        void open() {
            //Read first two lines to get names and types (Schema)
            if (!opened) {
                dataFile.open(filename);
                opened = true;
            }
            string line;
            getline(dataFile, line);
            names = split(line);
            getline(dataFile, line);
            types = split(line);

            // Get rid of junk line
            getline(dataFile, line);
        }

        vector<Record> next() {
            vector<Record> outPage;
            int i = 0;
            string line;
            while(i<pageSize && getline(dataFile, line)) {
                // Get next line, values for the next Record
                vector<string> values = split(line);
                outPage.emplace_back(names, types, values);
                ++i;
            }
            // Return an empty vector if no more pages exist
            return outPage;
        }

        void close() {
            if (opened) {
                dataFile.close();
                dataFile.clear();
                opened = false;
            }
        }

        void toFile(string fileName) {
            if (opened)
                dataFile.close();
            filename = fileName;
        }

        string getType() {
            return "FileReader";
        }
};

#endif
