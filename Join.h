#ifndef JOIN_H
#define JOIN_H

#include <vector>
#include <cstdio>
#include "Operator.h"
#include "Record.h"
#include "FileReader.h"
#include "FileWriter.h"

class Join : public Operator {
    private:
        Operator *leftOp;
        Operator *rightOp;
        bool ownRightOp = false;
        bool ownLeftOp = false;
        string leftAttr;
        string rightAttr;

        vector<Record> leftInPage;
        vector<Record> rightInPage;
        int leftIndex = 0;
        int rightIndex = 0;

        FileWriter *writer = nullptr;
        string tempFile;

    public:
        Join(Operator *op1, Operator *op2, string pAttr1, string pAttr2) {
            this->leftOp = op1;
            this->rightOp = op2;
            leftAttr = pAttr1;
            rightAttr = pAttr2;
            tempFile = "tempFile_" + to_string(tempNum++) + ".data";
            writer = new FileWriter(tempFile);

        }
        Join(Operator *op, string fileName, string pAttr1, string pAttr2) {
            this->leftOp = op;
            this->rightOp = new FileReader(fileName);
            ownRightOp = true;
            leftAttr = pAttr1;
            rightAttr = pAttr2;
        }
        Join(string fileName1, string fileName2, string pAttr1, string pAttr2) {
            this->leftOp = new FileReader(fileName1);
            this->rightOp = new FileReader(fileName2);
            ownLeftOp = true;
            ownRightOp = true;
            leftAttr = pAttr1;
            rightAttr = pAttr2;
        }

        void open() {
            leftOp->open();
            rightOp->open();
            if (writer) {
                writer->open();
            }
        }

        vector<Record> next() {
            vector<Record> outPage;
            fillOutPage(&outPage);
            return outPage;
        }

        void close() {
            // FIXME:: Delete the temp file
            if (!tempFile.empty())
                remove(tempFile.c_str());
            leftOp->close();
            rightOp->close();
            if (writer)
                writer->close();
        }

        string getType() {
            return "Join";
        }

        ~Join() {
            if (ownLeftOp)
                delete leftOp;
            if (ownRightOp)
                delete rightOp;
            if (writer)
                delete writer;
        }

    private:
        bool full(vector<Record> *page) {
            return page->size() >= pageSize;
        }

        void fillOutPage(vector<Record>* outPage) {
            while (!full(outPage)) {
                handleEmptyPages();
                if (leftInPage.empty())
                    return;
                if (compareRecords()) {
                    outPage->emplace_back((leftInPage[leftIndex] + rightInPage[rightIndex]));
                }
                advanceIndex();
            }
        }

        void advanceLeftPage() {
            leftInPage = leftOp->next();
        }

        void advanceRightPage() {
            rightInPage = rightOp->next();
            materializeData(rightInPage);
        }

        bool compareRecords() {
            Attribute leftAttribute = leftInPage[leftIndex].getAttributeByName(leftAttr);
            Attribute rightAttribute = rightInPage[rightIndex].getAttributeByName(rightAttr);
            if (leftAttribute == rightAttribute) {
                return true;
            } else {
                return false;
            }
        }

        void advanceIndex() {
            rightIndex++;
            if (rightIndex == pageSize || rightIndex == rightInPage.size()) {
                leftIndex++;
                getNextPage();
            }
        }

        void getNextPage() {
            if (leftIndex == pageSize || leftIndex == leftInPage.size()) {
                advanceRightPage();
                if (rightInPage.empty()) {
                    rightOp->close();
                    switchOperator();
                    rightOp->open();
                    advanceLeftPage();
                    advanceRightPage();
                }
                leftIndex = 0;
                rightIndex = 0;
            } else {
                rightIndex = 0;
            }
        }

        void handleEmptyPages() {
            if (leftInPage.empty())
                advanceLeftPage();
            if (rightInPage.empty())
                advanceRightPage();
        }

        void materializeData(vector<Record> records) {
            if (!writer) {
                return;
            }
            writer->write(records);
        }

        void switchOperator() {
            if (rightOp->getType() != "FileReader") {
                rightOp = new FileReader(tempFile);
                ownRightOp = true;
                delete writer;
                writer = nullptr;
            }
        }
};


#endif
