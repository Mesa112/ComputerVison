#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <sstream>
#include <algorithm>

using namespace std;

struct CCproperty {
    int label;
    int numPixels;
    int minRow, minCol, maxRow, maxCol;
};

struct point {
    int row, col;
};

class chainCode {
public:
    int numCC;
    CCproperty CC;
    int numRows, numCols, minVal, maxVal;

    int** imgAry;
    int** boundryAry;
    int** CCAry;

    point coordOffset[8];
    int zeroTable[8];
    point startP;
    point currentP;
    point nextP;
    int lastQ;
    int nextDir;
    int PchainDir;

    chainCode(int numRows, int numCols, int minVal, int maxVal) :
        numRows(numRows), numCols(numCols), minVal(minVal), maxVal(maxVal),
        numCC(0), lastQ(0), nextDir(0), PchainDir(0) {
        initialize();

        coordOffset[0] = {0, 1};
        coordOffset[1] = {-1, 1};
        coordOffset[2] = {-1, 0};
        coordOffset[3] = {-1, -1};
        coordOffset[4] = {0, -1};
        coordOffset[5] = {1, -1};
        coordOffset[6] = {1, 0};
        coordOffset[7] = {1, 1};

        zeroTable[0] = 6;
        zeroTable[1] = 0;
        zeroTable[2] = 0;
        zeroTable[3] = 2;
        zeroTable[4] = 2;
        zeroTable[5] = 4;
        zeroTable[6] = 4;
        zeroTable[7] = 6;
    }

    void initialize() {
        imgAry = new int*[numRows + 2];
        for (int i = 0; i < numRows + 2; i++) {
            imgAry[i] = new int[numCols + 2];
        }

        boundryAry = new int*[numRows + 2];
        for (int i = 0; i < numRows + 2; i++) {
            boundryAry[i] = new int[numCols + 2];
        }

        CCAry = new int*[numRows + 2];
        for (int i = 0; i < numRows + 2; i++) {
            CCAry[i] = new int[numCols + 2];
        }
    }

    void zeroFramed(int** imgAry, int numRows, int numCols) {
        for (int i = 0; i < numRows + 2; i++) {
            for (int j = 0; j < numCols + 2; j++) {
                imgAry[i][j] = 0;
            }
        }
    }

    void loadImage(ifstream& file, int** imgAry, int numRows, int numCols) {
        for (int i = 1; i < numRows + 1; i++) {
            for (int j = 1; j < numCols + 1; j++) {
                file >> imgAry[i][j];
            }
        }
    }

    void clearCCAry(int** CCAry, int numRows, int numCols) {
        for (int i = 0; i < numRows + 2; i++) {
            for (int j = 0; j < numCols + 2; j++) {
                CCAry[i][j] = 0;
            }
        }
    }

    void loadCCAry(int ccLabel, int** CCAry) {
        for (int i = 1; i < numRows + 1; i++) {
            for (int j = 1; j < numCols + 1; j++) {
                if (imgAry[i][j] == ccLabel) {
                    CCAry[i][j] = imgAry[i][j];
                } else {
                    CCAry[i][j] = 0;
                }
            }
        }
    }

    void getChainCode(CCproperty CC, int** CCAry, ofstream& chainCodeFile, ofstream& deBugFile) {
        deBugFile << "Entering getChainCode method" << endl;

        int nextQ;
        bool foundStart = false;

        for (int iRow = 1; iRow < numRows + 1 && !foundStart; iRow++) {
            for (int jCol = 1; jCol < numCols + 1 && !foundStart; jCol++) {
                if (CCAry[iRow][jCol] == CC.label) {
                    chainCodeFile << iRow << " " << jCol << " " << CC.label << endl;
                    startP.row = iRow;
                    startP.col = jCol;
                    currentP.row = iRow;
                    currentP.col = jCol;
                    lastQ = 4;
                    foundStart = true;
                }
            }
        }
        bool cut = true;
        while (cut) {
            nextQ = (lastQ + 1) % 8;
            PchainDir = findNextP(currentP, nextQ, deBugFile);
            chainCodeFile << PchainDir << " ";
            nextP.row = currentP.row + coordOffset[PchainDir].row;
            nextP.col = currentP.col + coordOffset[PchainDir].col;
            currentP = nextP;

            if (PchainDir == 0) {
                lastQ = zeroTable[7];
            } else {
                lastQ = zeroTable[PchainDir - 1];
            }

            deBugFile << "lastQ = " << lastQ << "; nextQ = " << nextQ
                      << "; currentP.row = " << currentP.row << "; currentP.col = " << currentP.col
                      << "; nextP.row = " << nextP.row << "; nextP.col = " << nextP.col << endl;

            if (nextP.row == startP.row && nextP.col == startP.col) {
                cut = false;
            }
        }

        chainCodeFile << endl;
        deBugFile << "Leaving getChainCode" << endl;
    }

    int findNextP(point currentP, int lastQ, ofstream& deBugFile) {
        deBugFile << "Entering findNextP method" << endl;

        int label = imgAry[currentP.row][currentP.col];
        int index = lastQ;
        bool found = false;
        int chainDir = 0;

        while (!found) {
            int iRow = currentP.row + coordOffset[index].row;
            int jCol = currentP.col + coordOffset[index].col;

            if (imgAry[iRow][jCol] == label) {
                chainDir = index;
                found = true;
            } else {
                index = (index + 1) % 8;
            }
        }

        deBugFile << "Leaving findNextP method" << endl;
        deBugFile << "chainDir = " << chainDir << endl;

        return chainDir;
    }

    void constructBoundary(int** boundryAry, ifstream& chainCodeFile) {
        int numRows2, numCols2, minVal2, maxVal2, numCC2;
        chainCodeFile >> numRows2 >> numCols2 >> minVal2 >> maxVal2;
        chainCodeFile >> numCC2;

        for (int i = 0; i < numRows2 + 2; i++) {
            for (int j = 0; j < numCols2 + 2; j++) {
                boundryAry[i][j] = 0;
            }
        }

        int label, startRow, startCol;
        while (chainCodeFile >> startRow >> startCol >> label) {
            int currentRow = startRow;
            int currentCol = startCol;

            boundryAry[currentRow][currentCol] = label;

            int chainDir2;
            while (chainCodeFile >> chainDir2) {
                currentRow += coordOffset[chainDir2].row;
                currentCol += coordOffset[chainDir2].col;

                boundryAry[currentRow][currentCol] = label;

                if (currentRow == startRow && currentCol == startCol) {
                    break;
                }
            }
        }
    }

    void reformatPrettyPrint(int** imgAry, ofstream& outFile1, int numRows, int numCols) {
        outFile1 << numRows << " " << numCols << " " << minVal << " " << maxVal << "\n";
        for (int i = 1; i < numRows + 1; i++) {
            for (int j = 1; j < numCols + 1; j++) {
                if (imgAry[i][j] > 0) {
                    outFile1 << imgAry[i][j] << " ";
                } else {
                    outFile1 << ". ";
                }
            }
            outFile1 << endl;
        }
        outFile1 << endl;
    }

    void imgReformat(int** imgAry, ofstream& boundryFile, int numRows, int numCols) {
        boundryFile << numRows << " " << numCols << " " << minVal << " " << maxVal << "\n";
        for (int i = 1; i < numRows + 1; i++) {
            for (int j = 1; j < numCols + 1; j++) {
                if (imgAry[i][j] > 0) {
                    boundryFile << imgAry[i][j];
                } else {
                    boundryFile << "0";
                }
                boundryFile << " ";
            }
            boundryFile << "\n";
        }
    }

    ~chainCode() {
        for (int i = 0; i < numRows + 2; i++) {
            delete[] imgAry[i];
            delete[] boundryAry[i];
            delete[] CCAry[i];
        }
        delete[] imgAry;
        delete[] boundryAry;
        delete[] CCAry;
    }
};

int main(int argc, char* argv[]) {
    // Step 0: Open files via argv[]
    string labelFile = argv[1];
    string propFile = argv[2];
    string outFile1 = argv[3];
    string deBugFile = argv[4];
    string chainCodeFile = argv[5];
    string boundryFile = argv[6];
    
    ifstream labelFiles(labelFile);
    ifstream propFiles(propFile);
    ofstream outFiles1(outFile1);
    ofstream deBugFiles(deBugFile);
    ofstream chainCodeFiles(chainCodeFile);
    ofstream boundryFiles(boundryFile);

    // we read numRows, numCols, minVal, maxVal from labelFile
    int numRows, numCols, minVal, maxVal;
    labelFiles >> numRows >> numCols >> minVal >> maxVal;

    // we read numRows, numCols, minVal, maxVal from propFile
    propFiles >> numRows >> numCols >> minVal >> maxVal;

    // we read numCC from propFile
    int numCC;
    propFiles >> numCC;

    // then we create an instance of the chainCode class
    chainCode cc(numRows, numCols, minVal, maxVal);
    cc.zeroFramed(cc.imgAry, numRows, numCols);
    cc.loadImage(labelFiles, cc.imgAry, numRows, numCols);
    outFiles1 << "Input Image" << endl;
    cc.reformatPrettyPrint(cc.imgAry, outFiles1, numRows, numCols);
    // Step 1: Write image header to chainCodeFile
    chainCodeFiles << numRows << " " << numCols << " " << minVal << " " << maxVal << endl;
    chainCodeFiles << numCC << endl;
    for (int i = 0; i < numCC; i++) {
        // Step 2: Read CC from propFile
        propFiles >> cc.CC.label >> cc.CC.numPixels >> cc.CC.minRow >> cc.CC.minCol >> cc.CC.maxRow >> cc.CC.maxCol;

        // Step 3: Clear CCAry
        cc.clearCCAry(cc.CCAry, numRows, numCols);

        // Step 4: Load CCAry with the current CC
        cc.loadCCAry(cc.CC.label, cc.CCAry);
        deBugFiles << "Current:" << endl;
        cc.reformatPrettyPrint(cc.CCAry, deBugFiles, numRows, numCols);

        // Step 5: Get chain code for the current CC
        cc.getChainCode(cc.CC, cc.CCAry, chainCodeFiles, deBugFiles);
    }

    // Step 7: Close chainCodeFile
    chainCodeFiles.close();

    // Step 8: Reopen chainCodeFile
    ifstream chainCodeFileIn(chainCodeFile);

    // Step 9: Construct boundary from chainCodeFile
    cc.constructBoundary(cc.boundryAry, chainCodeFileIn);

    // Reformat and pretty print boundryAry to outFile1
    outFiles1 << "Boundary Image" << endl;
    cc.reformatPrettyPrint(cc.boundryAry, outFiles1, numRows, numCols);

    // Reformat boundryAry to boundryFile
    cc.imgReformat(cc.boundryAry, boundryFiles, numRows, numCols);

    // Step 10: Close all files
    labelFiles.close();
    propFiles.close();
    outFiles1.close();
    deBugFiles.close();
    chainCodeFileIn.close();
    boundryFiles.close();

    cout << "DONE!" << endl;
    return 0;
}
