#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class image{

    int numRows, numCols, minVal, maxVal, thrVal;
    int* histAry;
    int** imgAry;

    public:

    image(int numRows, int numCols, int minVal, int maxVal, int thrVal):
        numRows(numRows), numCols(numCols), minVal(minVal), maxVal(maxVal), thrVal(thrVal){
        
        histAry = new int[maxVal + 1]();
        imgAry = new int*[numRows];
            for(int i = 0; i < numRows; i++){
                imgAry[i] = new int[numCols]();
            }
        }

    void loadImage(ifstream& inFile){
        for(int i = 0; i < numRows; i++){
            for(int j = 0; j < numCols; j++){
                inFile >> imgAry[i][j];
            }
        }
    }

    void computeHist(ofstream& logFile){
        logFile << "Enerting computeHist()\n";
        for(int i = 0; i < numRows; i++){
            for(int j = 0; j < numCols; j++){
                int val = imgAry[i][j];
                if(val < minVal || val > maxVal){
                    logFile << "imgAry[" << i << "][" << j << "] value " << val << " is not within minVal and maxVal\n";
                    exit(1);
                }
                histAry[val]++;
            }
        }
        logFile << "leaving computeHist()\n";
    }

    void printHist(ofstream& histCountFile, ofstream& logFile){
        logFile << "Entering printHist()\n";
        histCountFile << numRows << " " << numCols << " " << minVal << " " << maxVal << "\n";

        for(int i = 0; i <= maxVal; i++){
            int count = histAry[i];
            histCountFile << i << " " << count << "\n";
        }

        logFile << "Leaving printHist()\n";
    }

    void displayHist(ofstream& histGraphFile, ofstream& logFile){
        logFile << "Entering displayHist()\n";
        histGraphFile << numRows << " " << numCols << " " << minVal << " " << maxVal << "\n";

        for(int i = 0; i <= maxVal; i++){
            int count = histAry[i];
            histGraphFile << i << " (" << count << "):";
            for(int j = 0; j < count; j++){
                    histGraphFile << "+";
            }
            histGraphFile << "\n";
        }
        logFile << "Leaving displayHist()\n";
    }

    void binaryThreshold(ofstream& binThrFile, ofstream& logFile){
        logFile << "Entering binaryThreshold()\n";
        logFile << "The result of the binary thresholding using " << thrVal << " as threshold value\n";
        binThrFile << numRows << " " << numCols << " " << "0 1\n";
        logFile << numRows << " " << numCols << " " << "0 1\n";
        for(int i = 0; i < numRows; i++){
            for(int j = 0; j < numCols; j++){
                if(imgAry[i][j] >= thrVal){
                    binThrFile << "1 ";
                    logFile << "1 ";
                }else{
                    binThrFile << "0 ";
                    logFile << "0 ";
                }
            }
            binThrFile << "\n";
            logFile << "\n";
        }
        logFile << "leaving binaryThreshold";
    } 

    void prettyPrint(ofstream& logFile){
        logFile << "Enter PrettyPrint()\n";
        logFile << numRows << " " << numCols << " " << minVal << " " << maxVal << "\n";
        string str = to_string(maxVal);
        int width = str.length();
        for(int i = 0; i < numRows; i++){
            for(int j = 0; j < numCols; j++){
                logFile << imgAry[i][j];
                str = to_string(imgAry[i][j]);
                int ww = str.length();
                while(ww <= width){
                logFile << " ";
                ww++;
                }
            }
            logFile << "\n";
        }
        logFile << "Leaving PrettyPrint()\n";
    }
};

int main(int argc, char** argv){
    if(argc < 7){
        cerr << "Error opening Files" << endl;
        return 1;
    }

    ifstream inFile(argv[1]);
    int thrVal = stoi(argv[2]);
    ofstream histCountFile(argv[3]);
    ofstream histGraphFile(argv[4]);
    ofstream binThrFile(argv[5]);
    ofstream logFile(argv[6]);

    if (!inFile.is_open()) {
        cerr << "Error: Cannot open input file." << endl;
        return 1;
    }
    if (!histCountFile.is_open()) {
        cerr << "Error: Cannot open histogram count file." << endl;
        return 1;
    }
    if (!histGraphFile.is_open()) {
        cerr << "Error: Cannot open histogram graph file." << endl;
        return 1;
    }
    if (!binThrFile.is_open()) {
        cerr << "Error: Cannot open binary threshold file." << endl;
        return 1;
    }
    if (!logFile.is_open()) {
        cerr << "Error: Cannot open log file." << endl;
        return 1;
    }

    int numRows, numCols, minVal, maxVal;
    inFile >> numRows >> numCols >> minVal >> maxVal;

    image img(numRows, numCols, minVal, maxVal, thrVal);

    img.loadImage(inFile);
    img.prettyPrint(logFile);
    img.computeHist(logFile);
    img.printHist(histCountFile, logFile);
    img.displayHist(histGraphFile, logFile);
    logFile << "The Threshold Value = " << thrVal << "\n";
    img.binaryThreshold(binThrFile, logFile);

    // Close all files
    inFile.close();
    histCountFile.close();
    histGraphFile.close();
    binThrFile.close();
    logFile.close();
}
