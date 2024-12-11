#include <iostream>
#include <fstream>
#include <algorithm>


using namespace std;

class distanceSkeleton{
    int numRows, numCols, minVal, maxVal, newMinVal, newMaxVal, distanceChoice;
    public:
    int **ZFary;
    int **skeletonAry;
    distanceSkeleton(int numRows, int numCols, int minVal, int maxVal, int distanceChoice): 
    numRows(numRows), numCols(numCols), minVal(minVal), maxVal(maxVal), distanceChoice(distanceChoice){

        ZFary = new int*[numRows + 2];
        for (int i = 0; i < numRows + 2; i++){
            ZFary[i] = new int[numCols + 2]();
        }

        skeletonAry = new int*[numRows + 2];
            for (int i = 0; i < numRows + 2; i++){
                skeletonAry[i] = new int[numCols + 2]();
            }
        }

        void setZero(int **inAry){
            for (int i = 0; i < numRows + 2; i++){
                for (int j = 0; j < numCols + 2; j++){
                    inAry[i][j] = 0;
                }
            }
        }

        void loadImg(ifstream &inFile, int **Ary){
            for (int i = 1; i < numRows + 1; i++){
                for (int j = 1; j < numCols + 1; j++){
                    inFile >> Ary[i][j];
                }
            }
        }

        void loadSkeleton(ifstream &inFile, int **Ary){
            int numRowsFile, numColsFile, minValFile, maxValFile;
            inFile.ignore(numeric_limits<streamsize>::max(), '\n');//to ignore first header
            inFile.ignore(numeric_limits<streamsize>::max(), '\n');//to ignore second header
            int row, col, value;
            while (inFile >> row >> col >> value) {
                Ary[row][col] = value;
            }
        }   
    

        void distanceTransform(int **inAry, int distanceChoice, ofstream& prettyPrintFile, ofstream &logFile){
            logFile << "Entering distanceTransform() \n";
            distancePass1(inAry, distanceChoice, logFile);
            prettyPrint(inAry, prettyPrintFile, logFile, "1st pass distance transform with choice = " + to_string(distanceChoice));
            distancePass2(inAry, distanceChoice, logFile);
            prettyPrint(inAry, prettyPrintFile, logFile, "2nd pass distance transform with choice = " + to_string(distanceChoice));

            logFile << "Leaving distanceTransform() \n";
        }

        void distancePass1(int **inAry, int distanceChoice, ofstream &logFile){
            newMinVal = INT_MAX;
            newMaxVal = INT_MIN;
            logFile << "Entering distancePass1() \n";
            if (distanceChoice == 4){
                for (int i = 1; i < numRows + 1; i++){
                    for (int j = 1; j < numCols + 1; j++){
                        if (inAry[i][j] > 0){
                            int minVal = min({inAry[i - 1][j - 1] + 2,
                                              inAry[i - 1][j] + 1,
                                              inAry[i - 1][j + 1] + 2,
                                              inAry[i][j - 1] + 1});

                            inAry[i][j] = minVal;
                        }
                        if (inAry[i][j] < newMinVal) newMinVal = inAry[i][j];
                        if (inAry[i][j] > newMaxVal) newMaxVal = inAry[i][j];
                    }
                }
                minVal = newMinVal;
                maxVal = newMaxVal;
            } else if (distanceChoice == 8){
                for (int i = 1; i < numRows + 1; i++){
                    for (int j = 1; j < numCols + 1; j++){
                        if (inAry[i][j] > 0){
                            int minVal = min({inAry[i - 1][j - 1] + 1,
                                              inAry[i - 1][j] + 1,
                                              inAry[i - 1][j + 1] + 1,
                                              inAry[i][j - 1] + 1});
                            inAry[i][j] = minVal;
                        }
                        if (inAry[i][j] < newMinVal) newMinVal = inAry[i][j];
                        if (inAry[i][j] > newMaxVal) newMaxVal = inAry[i][j];
                    }
                }
                minVal = newMinVal;
                maxVal = newMaxVal;
            } else {
                            cout << "Wrong choice, choose either 4 or 8 to process\n";
                            exit(1);
                        }
            logFile << "Leaving distancePass1() \n";
        }

        void distancePass2(int **inAry, int distanceChoice, ofstream &logFile){
            newMinVal = INT_MAX;
            newMaxVal = INT_MIN;
            logFile << "Entering distancePass2()\n";
            if (distanceChoice == 4){
                for (int i = numRows; i > 0; i--){
                    for (int j = numCols; j > 0; j--){
                        int minVal = min({inAry[i][j],
                                          inAry[i][j + 1] + 2,
                                          inAry[i + 1][j - 1] + 1,
                                          inAry[i + 1][j] + 2,
                                          inAry[i + 1][j + 1] + 1});
                        inAry[i][j] = minVal;

                        if (inAry[i][j] < newMinVal) newMinVal = inAry[i][j];
                        if (inAry[i][j] > newMaxVal) newMaxVal = inAry[i][j];
                    }
                }
                minVal = newMinVal;
                maxVal = newMaxVal;
            }
            else if (distanceChoice == 8){
                for (int i = numRows; i > 0; i--){
                    for (int j = numCols; j > 0; j--){
                        int minVal = min({inAry[i][j],
                                          inAry[i][j + 1] + 1,
                                          inAry[i + 1][j - 1] + 1,
                                          inAry[i + 1][j] + 1,
                                          inAry[i + 1][j + 1] + 1});
                        inAry[i][j] = minVal;

                        if (inAry[i][j] < newMinVal) newMinVal = inAry[i][j];
                        if (inAry[i][j] > newMaxVal) newMaxVal = inAry[i][j];
                    }
                }
                minVal = newMinVal;
                maxVal = newMaxVal;
            }
            else{
                cout << "Wrong choice, choose either 4 or 8 to process\n";
                exit(1);
            }
            logFile << "Leaving distancePass2() \n";

        }

        void compression(int** inAry, int** SkeletonAry, int distanceChoice, ofstream& logFile, ofstream& prettyPrintFile, ofstream& skeletonFile){
            logFile << "Entering Compression() method\n";
            computeLocalMaxima(inAry, SkeletonAry, distanceChoice, logFile, prettyPrintFile);
            prettyPrint(SkeletonAry, prettyPrintFile, logFile, "Local maxima, skeletonAry with choice = " + to_string(distanceChoice));
            extractSkeleton(skeletonFile, SkeletonAry, logFile);
            prettyPrint(SkeletonAry, prettyPrintFile, logFile, "In compression() Below is skeleton Array with choice = " + to_string(distanceChoice));
            logFile << "Leaving Compression() method\n";
        }

        bool islocalMaxima(int** inAry, int i, int j, int distanceChoice) {
            if (distanceChoice == 4) {
                int neighbors[] = {
                                    inAry[i - 1][j],   
                    inAry[i][j - 1],                inAry[i][j + 1],   
                                    inAry[i + 1][j]    
                };
                for (int k : neighbors) {
                    if (inAry[i][j] < k) {
                        return false;
                    }
                }
                return true;
            } 
            else if (distanceChoice == 8) {
                int neighbors[] = {
                    inAry[i - 1][j - 1], inAry[i - 1][j], inAry[i - 1][j + 1],  
                    inAry[i][j - 1],                          inAry[i][j + 1],    
                    inAry[i + 1][j - 1], inAry[i + 1][j], inAry[i + 1][j + 1]  
                };
                for (int k : neighbors) {
                    if (inAry[i][j] < k) {
                        return false;
                    }
                }
                return true;
            } 
            else {
                cout << "Wrong choice, choose either 4 or 8 to process\n";
                return false; 
            }
        }

        void computeLocalMaxima(int** inAry, int** skeletonAry, int distanceChoice, ofstream& logFile, ofstream& prettyPrintFile){
            logFile << "Entering computeLocalMaxima() method\n";
            for(int i = 1; i < numRows + 1; i++){
                for(int j = 1; j < numCols + 1; j++){
                    if(islocalMaxima(inAry, i, j, distanceChoice)){
                        skeletonAry[i][j] = inAry[i][j];
                    }else{
                        skeletonAry[i][j] = 0;
                    }
                }
            }
            logFile << "Leaving computeLocalMaxima() method\n";
        }

        void extractSkeleton(ofstream& skeletonFile, int** inAry, ofstream& logFile){
            logFile << "Entering extractSkeleton() method\n";
            skeletonFile << numRows << " " << numCols << " " << minVal << " " << maxVal << "\n";
            int maxRowWidth = max(3, static_cast<int>(to_string(numRows).length())); 
            int maxColWidth = max(6, static_cast<int>(to_string(numCols).length())); 
            int maxValWidth = max(5, static_cast<int>(to_string(maxVal).length())); 

            skeletonFile << setw(maxRowWidth) << "Row " 
                        << setw(maxColWidth) << "Column " 
                        << setw(maxValWidth) << "Value" << "\n";
            
            for(int i = 1; i < numRows + 1; i++){
                for(int j = 1; j < numCols + 1; j++){
                    if(inAry[i][j] > 0){
                        skeletonFile << setw(maxRowWidth) << i 
                                    << setw(maxColWidth) << j 
                                    << setw(maxValWidth) << inAry[i][j] << "\n";
                    }
                }
            }
            logFile << "Leaving extractSkeleton() method\n";
        }

        void deCompression(int** inAry, int distanceChoice, ofstream& prettyPrintFile, ofstream& logFile){
            logFile << "Entering deCompression() method \n";
                expansionPass1(inAry, distanceChoice, logFile);
                prettyPrint(inAry, prettyPrintFile, logFile, "1st pass Expansion with choice = " + to_string(distanceChoice));
                expansionPass2(inAry, distanceChoice, logFile);
                prettyPrint(inAry, prettyPrintFile, logFile, "2nd pass Expansion with choice = " + to_string(distanceChoice));
            logFile << "Leaving deCompression() method \n";
        }

        void expansionPass1(int** inAry, int distanceChoice, ofstream& logFile){
            logFile << "Entering expansionPass1() Method \n";
            newMinVal = INT_MAX;
            newMaxVal = INT_MIN;
            if(distanceChoice == 4){
                for(int i = 1; i < numRows + 1; i++){
                    for(int j = 1; j < numCols + 1; j++){
                        if(inAry[i][j] == 0){
                            inAry[i][j] = max({inAry[i - 1][j - 1] - 2, inAry[i - 1][j] - 1, inAry[i - 1][j + 1] - 2,  
                                        inAry[i][j - 1] - 1,          inAry[i][j],           inAry[i][j + 1] - 1,    
                                        inAry[i + 1][j - 1] - 2, inAry[i + 1][j] - 1, inAry[i + 1][j + 1] - 2});
                        }
                        if (inAry[i][j] < newMinVal) newMinVal = inAry[i][j];
                        if (inAry[i][j] > newMaxVal) newMaxVal = inAry[i][j];
                    }
                }
                minVal = newMinVal;
                maxVal = newMaxVal;
            }else if(distanceChoice == 8){
                for(int i = 1; i < numRows + 1; i++){
                    for(int j = 1; j < numCols + 1; j++){
                        if(inAry[i][j] == 0){
                            inAry[i][j] = max({inAry[i - 1][j - 1] - 1, inAry[i - 1][j] - 1, inAry[i - 1][j + 1] - 1,  
                                            inAry[i][j - 1] - 1,           inAry[i][j],      inAry[i][j + 1] - 1,    
                                            inAry[i + 1][j - 1] - 1, inAry[i + 1][j] - 1, inAry[i + 1][j + 1] - 1});
                                            
                        }
                        if (inAry[i][j] < newMinVal) newMinVal = inAry[i][j];
                        if (inAry[i][j] > newMaxVal) newMaxVal = inAry[i][j];
                    }
                }
                minVal = newMinVal;
                maxVal = newMaxVal;
            }else{
                cout << "Wrong choice, choose either 4 or 8 to process\n";
                exit(1);
            }
            logFile << "Leaving expansionPass1() Method \n";

        }

        void expansionPass2(int** inAry, int distanceChoice, ofstream& logFile){
            logFile << "Entering expansionPass2() Method \n";
            newMinVal = INT_MAX;
            newMaxVal = INT_MIN;
            if(distanceChoice == 4){
                for(int i = numRows; i > 0; i--){
                    for(int j = numCols; j > 0; j--){
                            inAry[i][j] = max({inAry[i - 1][j - 1] - 2, inAry[i - 1][j] - 1, inAry[i - 1][j + 1] - 2,  
                                        inAry[i][j - 1] - 1,          inAry[i][j],           inAry[i][j + 1] - 1,    
                                        inAry[i + 1][j - 1] - 2, inAry[i + 1][j] - 1, inAry[i + 1][j + 1] - 2});
                            if (inAry[i][j] < newMinVal) newMinVal = inAry[i][j];
                            if (inAry[i][j] > newMaxVal) newMaxVal = inAry[i][j];
                    }
                }
                minVal = newMinVal;
                maxVal = newMaxVal;
            }else if(distanceChoice == 8){
                for(int i = numRows; i > 0; i--){
                    for(int j = numCols; j > 0; j--){
                        inAry[i][j] = max({inAry[i - 1][j - 1] - 1, inAry[i - 1][j] - 1, inAry[i - 1][j + 1] - 1,  
                                        inAry[i][j - 1] - 1,          inAry[i][j],           inAry[i][j + 1] - 1,    
                                        inAry[i + 1][j - 1] - 1, inAry[i + 1][j] - 1, inAry[i + 1][j + 1] - 1});
                        if (inAry[i][j] < newMinVal) newMinVal = inAry[i][j];
                        if (inAry[i][j] > newMaxVal) newMaxVal = inAry[i][j];
                    }
                }
                minVal = newMinVal;
                maxVal = newMaxVal;
            }else{
                cout << "Wrong choice, choose either 4 or 8 to process\n";
                exit(1);
            }
            logFile << "Leaving expansionPass2() Method \n";

        }

        void binaryTheshold(int** inAry, ofstream& decompressedFile, ofstream& logFile){
            logFile << "Entering binaryThreshold() Method \n";
            for(int i = 1; i < numRows + 1; i++){
                for(int j = 1; j < numCols + 1; j++){
                    if(inAry[i][j] >= 1){
                        decompressedFile << "1 ";
                    }else{
                        decompressedFile << "0 ";
                    }
                }
                decompressedFile << "\n";
            }
            logFile << "Leaving binaryThreshold() Method \n";
        }

        void prettyPrint(int **inAry, ofstream& outFile, ofstream& logFile, string caption = ""){
            logFile << "Entering PrettyPrint() Method\n";
            if (!caption.empty()) {
                outFile << caption << "\n";
            }
            outFile << numRows << " " << numCols << " " << minVal << " " << maxVal << "\n";
            string str = to_string(maxVal);
            int Width = str.length();
            int i = 1;
            while (i < (numRows + 1)) {
                int j = 1;
                while (j < (numCols + 1)) {
                    str = to_string(inAry[i][j]);
                    outFile.write(str.c_str(), str.length());
                    int WW = str.length();
                    while (WW <= Width) {
                        outFile.write(" ", 1);
                        WW++;
                    }
                    j++;
                }
                i++;
                outFile.write("\n", 1);
            }
            logFile << "Leaving PrettyPrint() Method\n";
        }   
    };

    int main(int argc, char **argv){
      if (argc < 7) {
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

      // Input distanceChoice
      int distanceChoice = stoi(argv[2]);

      // Open PrettyPrintFile
      ofstream prettyPrintFile(argv[3]);
      if (!prettyPrintFile.is_open()) {
          cerr << "Error: Cannot open PrettyPrintFile file." << endl;
          return 1;
      }

      // Open skeletonFile
      ofstream skeletonFile(argv[4]);
      if (!skeletonFile.is_open()) {
          cerr << "Error: Cannot open SkeletonFile file." << endl;
          return 1;
      }

      // Open deCompressedFile
      ofstream deCompressedFile(argv[5]);
      if (!deCompressedFile.is_open()) {
          cerr << "Error: Cannot open deCompressedFile file." << endl;
          return 1;
      }

      // Open logFile to log the progress of the program
      ofstream logFile(argv[6]);
      if (!logFile.is_open()) {
          cerr << "Error: Cannot open logFile." << endl;
          return 1;
      }

      int numRows, numCols, minVal, maxVal;

      inFile >> numRows >> numCols >> minVal >> maxVal;
      distanceSkeleton img(numRows, numCols, minVal, maxVal, distanceChoice);

      img.setZero(img.ZFary);
      img.setZero(img.skeletonAry);
      img.loadImg(inFile, img.ZFary);
      img.prettyPrint(img.ZFary, prettyPrintFile, logFile, "** Below is input image **");
      img.distanceTransform(img.ZFary, distanceChoice, prettyPrintFile, logFile);
      img.compression(img.ZFary, img.skeletonAry, distanceChoice, logFile, prettyPrintFile, skeletonFile);
      skeletonFile.close();

      // Reopen skeletonFile as an input stream  within a different scope
        
            ifstream skeletonInFile(argv[4]);
            if (!skeletonInFile.is_open()) {
                cerr << "Error: Could not reopen SkeletonFile." << endl;
                return 1;
            }
        
        img.setZero(img.ZFary);
        img.loadSkeleton(skeletonInFile, img.ZFary);
        img.prettyPrint(img.ZFary, prettyPrintFile, logFile, "** Below is the loaded skeleton with choice= " + to_string(distanceChoice));
        img.deCompression(img.ZFary, distanceChoice, prettyPrintFile, logFile);
        deCompressedFile << numRows << " " << numCols << " " << minVal << " " << maxVal << "\n";
        img.binaryTheshold(img.ZFary, deCompressedFile, logFile);

        skeletonFile.close();
        skeletonInFile.close();
        inFile.close();
        prettyPrintFile.close();
        deCompressedFile.close();
        logFile.close();

        cout << "Processing Complete!\n";

    }