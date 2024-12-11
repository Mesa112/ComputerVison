#include <iostream>
#include <sstream>
#include <fstream>
#include <cmath>
#include <algorithm>

using namespace std;

class Enhancement
{
public:
    int numRows, numCols, minVal, maxVal, maskRows, maskCols, maskMin, maskMax, newMin, newMax, thrVal, maskWeight;
    int **mirrorFmAry;
    int **medianAry;
    int **gaussAry;
    int **thrAry;
    int **avgAry;
    int **mask2DAry;
    int *mask1DAry;
    int *neighbor1DAry;

    Enhancement(int numRows, int numCols, int minVal, int maxVal, int thrVal, int maskRows, int maskCols, int maskMin, int maskMax) : numRows(numRows), numCols(numCols), minVal(minVal), maxVal(maxVal), thrVal(thrVal), maskRows(maskRows), maskCols(maskCols), maskMin(maskMin), maskMax(maskMax)
    {

        mirrorFmAry = new int *[numRows + 2];
        for (int i = 0; i < numRows + 2; i++)
        {
            mirrorFmAry[i] = new int[numCols + 2]();
        }
        medianAry = new int *[numRows + 2];
        for (int i = 0; i < numRows + 2; i++)
        {
            medianAry[i] = new int[numCols + 2]();
        }
        gaussAry = new int *[numRows + 2];
        for (int i = 0; i < numRows + 2; i++)
        {
            gaussAry[i] = new int[numCols + 2]();
        }
        thrAry = new int *[numRows + 2];
        for (int i = 0; i < numRows + 2; i++)
        {
            thrAry[i] = new int[numCols + 2]();
        }
        avgAry = new int *[numRows + 2];
        for (int i = 0; i < numRows + 2; i++)
        {
            avgAry[i] = new int[numCols + 2]();
        }
        mask2DAry = new int *[numRows + 2];
        for (int i = 0; i < numRows + 2; i++)
        {
            mask2DAry[i] = new int[numCols + 2]();
        }
        mask1DAry = new int[9]();
        neighbor1DAry = new int[9]();
    }

    void loadImage(ifstream &inFile, int **outMtx, ofstream &logFile)
    {
        logFile << "Entering loadImage()\n";
        for (int i = 1; i <= numRows; i++)
        {
            for (int j = 1; j <= numCols; j++)
            {
                inFile >> outMtx[i][j];
            }
        }
        logFile << "Leaving loadImage()\n";
    }

    void binThreashold(int **inMtx1, int **outMtx, int thrVal, ofstream &logFile)
    {
        logFile << "Entering binThreshold()";
        for (int i = 1; i < numRows + 1; i++)
        {
            for (int j = 1; j < numCols + 1; j++)
            {
                if (inMtx1[i][j] >= thrVal)
                {
                    outMtx[i][j] = 1;
                }
                else
                {
                    outMtx[i][j] = 0;
                }
            }
        }
        logFile << "Leaving binThreshold()\n";
    }

    void prettyPrint(int **inputAry, int numRows, int numCols, int minVal, int maxVal, ofstream &logFile, bool printFullArray, bool isMask) {
    logFile << "Enter PrettyPrint()\n";
    if (isMask) {
        logFile << numRows << " " << numCols << " " << minVal << " " << maxVal << "\n";
    }
    else if (printFullArray) {
        logFile << numRows + 2 << " " << numCols + 2 << " " << minVal << " " << maxVal << "\n";
    }
    else {
        logFile << numRows << " " << numCols << " " << minVal << " " << maxVal << "\n";
    }
    string str = to_string(maxVal);
    int width = str.length();
    if (isMask) {
        for (int i = 0; i < numRows; i++) {
            for (int j = 0; j < numCols; j++) {
                logFile << inputAry[i][j];
                str = to_string(inputAry[i][j]);
                int ww = str.length();
                while (ww <= width) {
                    logFile << " ";
                    ww++;
                }
            }
            logFile << "\n";
        }
    }
    else {
        int startRow = printFullArray ? 0 : 1;
        int startCol = printFullArray ? 0 : 1;
        int endRow = printFullArray ? numRows + 2 : numRows + 1;
        int endCol = printFullArray ? numCols + 2 : numCols + 1;
        for (int i = startRow; i < endRow; i++) {
            for (int j = startCol; j < endCol; j++) {
                logFile << inputAry[i][j];
                str = to_string(inputAry[i][j]);
                int ww = str.length();
                while (ww <= width) {
                    logFile << " ";
                    ww++;
                }
            }
            logFile << "\n";
        }
    }
    logFile << "Leaving PrettyPrint()\n";
}


    void mirrorFraming(int **inputAry, ofstream &logFile) {
    logFile << "Entering mirrorFraming()\n";
    
    // Copy the top and bottom edges
    for (int col = 1; col < numCols + 1; col++) {
        inputAry[0][col] = inputAry[1][col];                     // Top edge
        inputAry[numRows + 1][col] = inputAry[numRows][col];     // Bottom edge
        logFile << "Copied row 1 to row 0 at column " << col << "\n";
        logFile << "Copied row " << numRows << " to row " << (numRows + 1) << " at column " << col << "\n";
    }

    // Copy the left and right edges
    for (int row = 1; row < numRows + 1; row++) {
        inputAry[row][0] = inputAry[row][1];                     // Left edge
        inputAry[row][numCols + 1] = inputAry[row][numCols];     // Right edge
        logFile << "Copied column 1 to column 0 at row " << row << "\n";
        logFile << "Copied column " << numCols << " to column " << (numCols + 1) << " at row " << row << "\n";
    }

    // Handle the four corners
    inputAry[0][0] = inputAry[1][1];                             // Top-left corner
    inputAry[0][numCols + 1] = inputAry[1][numCols];             // Top-right corner
    inputAry[numRows + 1][0] = inputAry[numRows][1];             // Bottom-left corner
    inputAry[numRows + 1][numCols + 1] = inputAry[numRows][numCols]; // Bottom-right corner
    logFile << "Copied value for the four corners\n";

    logFile << "Exiting mirrorFraming()\n";
}


    int loadMask(ifstream &inFile, ofstream &logFile)
    {
        logFile << "Entering loadMask()\n";
        int weight = 0;
        for (int i = 0; i < maskRows; i++)
        {
            for (int j = 0; j < maskCols; j++)
            {
                inFile >> mask2DAry[i][j];
                weight += mask2DAry[i][j];
            }
        }
        logFile << "Leaving loadMask()\n";
        return weight;
    }

    void loadMask2Dto1D(ofstream &logFile)
    {
        logFile << "Entering loadMask2Dto1D()\n";
        int index = 0;
        for (int i = 0; i < maskRows; i++)
        {
            for (int j = 0; j < numCols; j++)
            {
                mask1DAry[index] = mask2DAry[i][j];
                index++;
            }
        }
        logFile << "Leaving loadMask2Dto1D()\n";
    }

    void loadNeighbor2Dto1D(int i, int j, int **input2D, int *input1D, ofstream &logFile)
    {
        logFile << "Entering loadNeighbor2Dto1D() for pixel (" << i << ", " << j << ")\n";
        int index = 0;
        // Loop over the 3x3 neighborhood around pixel (i, j)
        for (int row = i - 1; row <= i + 1; row++)
        {
            for (int col = j - 1; col <= j + 1; col++)
            {
                input1D[index] = input2D[row][col];
                logFile << "input1D[" << index << "] = input2D[" << row << "][" << col << "] = " << input2D[row][col] << "\n";
                index++;
            }
        }

        logFile << "Leaving loadNeighbor2Dto1D() for pixel (" << i << ", " << j << ")\n";
    }

    void computeMedian(int **mirrorFmAry, int **medianAry, ofstream &logFile)
    {
        logFile << "Entering computeMedian()\n";

        int newMin = 9999; // large min
        int newMax = 0;    // large max
        for (int i = 1; i < numRows + 1; i++)
        {
            for (int j = 1; j < numCols + 1; j++)
            {
                loadNeighbor2Dto1D(i, j, mirrorFmAry, neighbor1DAry, logFile);
                logFile << "** Below is conversion of mirrorFmAry[" << i << "][" << j << "] to 1D array:\n";
                print1DAry(neighbor1DAry, logFile);
                sort(neighbor1DAry, neighbor1DAry + 9);
                logFile << "** Below is the sorted neighbor array for pixel (" << i << ", " << j << "):\n";
                print1DAry(neighbor1DAry, logFile);
                medianAry[i][j] = neighbor1DAry[4];
                if (newMin > medianAry[i][j])
                    newMin = medianAry[i][j];
                if (newMax < medianAry[i][j])
                    newMax = medianAry[i][j];
            }
        }
        logFile << "** Leaving computeMedian method ***\n";
        logFile << "newMin = " << newMin << "\n";
        logFile << "newMax = " << newMax << "\n";
    }

    void computeAvg(int **mirrorFmAry, int **inputMtx, ofstream &logFile)
    {
        logFile << "** Entering computeAvg method ***\n";

        int newMin = 9999;
        int newMax = 0;
        for (int i = 1; i < numRows + 1; i++)
        {
            for (int j = 1; j < numCols + 1; j++)
            {
                loadNeighbor2Dto1D(i, j, mirrorFmAry, neighbor1DAry, logFile);
                int sum = 0;
                for (int k = 0; k < 9; k++)
                {
                    sum += neighbor1DAry[k];
                }
                int avg = sum / 9;
                avgAry[i][j] = avg;
                if (newMin > avg)
                    newMin = avg;
                if (newMax < avg)
                    newMax = avg;
            }
        }
        logFile << "** Leaving computeAvg method ***\n";
        logFile << "newMin = " << newMin << "\n";
        logFile << "newMax = " << newMax << "\n";
    }

    void computeGauss(int **mirrorFmAry, int **GaussAry, int *mask1DAry, int maskWeight, ofstream &logFile)
    {
        logFile << "Entering computeGauss method\n";
        int newMin = 9999; // large min
        int newMax = 0;    // large max
        for (int i = 1; i < numRows + 1; i++)
        {
            for (int j = 1; j < numCols + 1; j++)
            {
                loadNeighbor2Dto1D(i, j, mirrorFmAry, neighbor1DAry, logFile);

                logFile << "** Below is conversion of mirrorFmAry[" << i << "][" << j << "] to 1D array:\n";
                print1DAry(neighbor1DAry, logFile);

                logFile << "** Below is the Gaussian mask (mask1DAry):\n";
                print1DAry(mask1DAry, logFile);
                GaussAry[i][j] = convolution(neighbor1DAry, mask1DAry, maskWeight, logFile);

                if (newMin > GaussAry[i][j])
                    newMin = GaussAry[i][j];
                if (newMax < GaussAry[i][j])
                    newMax = GaussAry[i][j];
            }
        }

        logFile << "Leaving computeGauss method\n";
        logFile << "newMin = " << newMin << "\n";
        logFile << "newMax = " << newMax << "\n";
    }

    int convolution(int *neighbor1DAry, int *mask1DAry, int maskWeight, ofstream &logFile)
    {
        logFile << "Entering convolution method\n";
        int result = 0;
        for (int i = 0; i < 9; ++i)
        {
            result += neighbor1DAry[i] * mask1DAry[i];
        }
        result /= maskWeight;
        logFile << "Leaving convolution method, maskWeight = " << maskWeight << "; result/maskWeight = " << result << "\n";
        return result;
    }

    void print1DAry(int *neighbor1DAry, ofstream &logFile)
    {
        logFile << "Entering print1DAry()\n";
        logFile << "[";
        int size = 9;
        for (int i = 0; i < size; i++)
        {
            logFile << neighbor1DAry[i];
            if (i < size - 1)
            {
                logFile << ", ";
            }
        }
        logFile << "]\n";
        logFile << "Leaving print1DAry()\n";
    }
};

int main(int argc, char **argv)
{
      // Check if the correct number of arguments is provided
      if (argc < 8) {
        cerr << "Error: Insufficient number of arguments." << endl;
        return 1;
    }
    cout << "Correct number of arguments provided!" << endl;
    for (int i = 0; i < argc; i++) {
        cout << "Argument " << i << ": " << argv[i] << endl;
    }

      // Open input image file
      ifstream inFile(argv[1]);
      if (!inFile.is_open()) {
          cerr << "Error: Cannot open input file." << endl;
          return 1;
      }

      // Open mask file
      ifstream maskFile(argv[2]);
      if (!maskFile.is_open()) {
          cerr << "Error: Cannot open mask file." << endl;
          return 1;
      }

      // Convert the threshold value from string to integer
      int thrVal = stoi(argv[3]);

      // Open AvgFile to store the result of 3x3 averaging
      ofstream AvgFile(argv[4]);
      if (!AvgFile.is_open()) {
          cerr << "Error: Cannot open AvgFile file." << endl;
          return 1;
      }

      // Open MedianFile to store the result of 3x3 median filtering
      ofstream MedianFile(argv[5]);
      if (!MedianFile.is_open()) {
          cerr << "Error: Cannot open MedianFile file." << endl;
          return 1;
      }

      // Open GaussFile to store the result of 3x3 Gaussian filtering
      ofstream GaussFile(argv[6]);
      if (!GaussFile.is_open()) {
          cerr << "Error: Cannot open GaussFile file." << endl;
          return 1;
      }

      // Open logFile to log the progress of the program
      ofstream logFile(argv[7]);
      if (!logFile.is_open()) {
          cerr << "Error: Cannot open logFile." << endl;
          return 1;
      }

    /*ifstream inFile("/Users/estebanm/Desktop/img1.txt");
    ifstream maskFile("/Users/estebanm/Desktop/mask.txt");
    int thrVal = 22;
    ofstream AvgFile("/Users/estebanm/Desktop/AvgFile_run1.txt");
    ofstream MedianFile("/Users/estebanm/Desktop/MedianFile_run1.txt");
    ofstream GaussFile("/Users/estebanm/Desktop/GaussFile_run1.txt");
    ofstream logFile("/Users/estebanm/Desktop/logFile_run1.txt");*/

    int numRows, numCols, minVal, maxVal, maskRows, maskCols, maskMin, maskMax;

    inFile >> numRows >> numCols >> minVal >> maxVal;
    maskFile >> maskRows >> maskCols >> maskMin >> maskMax;

    Enhancement img(numRows, numCols, minVal, maxVal, thrVal, maskRows, maskCols, maskMin, maskMax);

    int maskWeight = img.loadMask(maskFile, logFile);
    img.loadMask2Dto1D(logFile);
    img.loadImage(inFile, img.mirrorFmAry, logFile);
    img.mirrorFraming(img.mirrorFmAry, logFile);

    // Step 6: Average Filtering
    AvgFile << "** Below is the mirror framed input image. ***\n";
    img.prettyPrint(img.mirrorFmAry, numRows, numCols, minVal, maxVal, AvgFile, true, false);
    img.computeAvg(img.mirrorFmAry, img.avgAry, logFile);
    AvgFile << "** Below is the 3x3 averaging of the input image. ***\n";
    img.prettyPrint(img.avgAry, numRows, numCols, minVal, maxVal, AvgFile, false, false);
    img.binThreashold(img.avgAry, img.thrAry, thrVal, logFile);
    AvgFile << "** Below is the result of the binary threshold of averaging image. ***\n";
    img.prettyPrint(img.thrAry, numRows, numCols, minVal, maxVal, AvgFile, false, false);

    // Step 7: Median Filtering
    MedianFile << "** Below is the mirror framed input image. ***\n";
    img.prettyPrint(img.mirrorFmAry, numRows, numCols, minVal, maxVal, MedianFile, true, false);
    img.computeMedian(img.mirrorFmAry, img.medianAry, logFile);
    MedianFile << "** Below is the 3x3 median filter of the input image. ***\n";
    img.prettyPrint(img.medianAry, numRows, numCols, minVal, maxVal, MedianFile, false, false);
    img.binThreashold(img.medianAry, img.thrAry, thrVal, logFile);
    MedianFile << "** Below is the result of the binary threshold of median filtered image. ***\n";
    img.prettyPrint(img.thrAry, numRows, numCols, minVal, maxVal, MedianFile, false, false);

    // Step 8: Gaussian Filtering
    GaussFile << "** Below is the mirror framed input image. ***\n";
    img.prettyPrint(img.mirrorFmAry, numRows, numCols, minVal, maxVal, GaussFile, true, false);
    GaussFile << "** Below is the mask for Gaussian filter. ***\n";
    img.prettyPrint(img.mask2DAry, maskRows, maskCols, maskMin, maskMax, GaussFile, true, true);
    img.computeGauss(img.mirrorFmAry, img.gaussAry, img.mask1DAry, maskWeight, logFile);
    GaussFile << "** Below is the 3x3 Gaussian filter of the input image. ***\n";
    img.prettyPrint(img.gaussAry, numRows, numCols, minVal, maxVal, GaussFile, false, false);
    img.binThreashold(img.gaussAry, img.thrAry, thrVal, logFile);
    GaussFile << "** Below is the result of the binary threshold of Gaussian filtered image. ***\n";
    img.prettyPrint(img.thrAry, numRows, numCols, minVal, maxVal, GaussFile, false, false);

    // Step 9: Close all files
    inFile.close();
    maskFile.close();
    AvgFile.close();
    MedianFile.close();
    GaussFile.close();
    logFile.close();

    cout << "Execution completed!" << endl;
}