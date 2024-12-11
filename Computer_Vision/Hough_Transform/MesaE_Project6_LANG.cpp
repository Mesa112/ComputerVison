    #include <iostream>
    #include <cmath>
    #include <fstream>
    #include <sstream>
    #include <vector>
    #include <climits>

    using namespace std;

    long double const pi = 3.141592653589793238462643383279502884197;

    class HoughTransform
{
public:
    int numRows, numCols, minVal, maxVal, angleInDegree;
    int houghDist;
    int houghAngle;
    double angleInRadians;
    int **imgAry;
    int **cartesianAry;
    int **polarAry;
    double offset; 

    HoughTransform(int numRows, int numCols, int minVal, int maxVal) : numRows(numRows), numCols(numCols), minVal(minVal), maxVal(maxVal) {
        offset = (int)sqrt(pow((double)numRows, 2) + pow((double)numCols, 2)); 
        houghDist = (int)(sqrt((pow((double)numRows, 2) + (pow((double)numCols, 2)))) * 2);
        houghAngle = 180;
    }
        void initalize()
    {
        imgAry = new int*[numRows];
        for (int i = 0; i < numRows; i++)
        {
            imgAry[i] = new int[numCols];
        }

        cartesianAry = new int*[houghDist];
        for (int i = 0; i < houghDist; i++)
        {
            cartesianAry[i] = new int[houghAngle];
        }

        polarAry = new int*[houghDist];
        for (int i = 0; i < houghDist; i++)
        {
            polarAry[i] = new int[houghAngle];
        }
    }
        void loadImg(ifstream &file, int **imgAry)
        {
            for (int i = 0; i < numRows; i++)
            {
                for (int j = 0; j < numCols; j++)
                {
                    file >> imgAry[i][j];
                }
            }
        }

        void buildHoughSpace(int **imgAry, ofstream &deBugFile)
    {
        deBugFile << "Entering buildHoughSpace" << endl;

        for (int i = 0; i < numRows; i++)
        {
            for (int j = 0; j < numCols; j++)
            {
                if (imgAry[i][j] > 0)
                {
                    computeSinusoid(i, j, deBugFile); 
                }
            }
        }

        deBugFile << "Leaving buildHoughSpace" << endl;
    }

        void computeSinusoid(int i, int j, ofstream &deBugFile)
    {
        deBugFile << "Entering computeSinusoid()" << endl;
        angleInDegree = 0;

        while (angleInDegree <= 179)
        {
            angleInRadians = (double)(angleInDegree * (pi/180));
            double cartesianDist = CartesianDist(i, j, angleInRadians);
            int cartesianDistInt = (int)(cartesianDist);
            cartesianAry[cartesianDistInt][angleInDegree]++;

            double polarDist = PolarDist(i, j, angleInRadians);
            int polarDistInt = (int)(polarDist);
            polarAry[polarDistInt][angleInDegree]++;

            angleInDegree++;
            
        }
        deBugFile << "Leaving computeSinusoid()" << endl;
    }

       double CartesianDist(int i, int j, double angleInRadians)
{
    double convertI = static_cast<double>(i);
    double convertJ = static_cast<double>(j);
    double t = angleInRadians - (atan2(convertJ, convertI)) - (pi / 2);
    double d = ((sqrt(pow(convertI, 2) + pow(convertJ, 2))) * cos(t)) + offset;
    return d;
}

double PolarDist(int i, int j, double angleInRadians)
{
    double convertI = static_cast<double>(i);
    double convertJ = static_cast<double>(j);
    double d = (convertI * cos(angleInRadians) + convertJ * sin(angleInRadians)) + offset;
    return d;
}


        void reformatPrettyPrint(int **imgAry, ofstream &outFile1, int numRows, int numCols, const string& caption) {
        outFile1 << caption << endl;
        outFile1 << numRows << " " << numCols << " " << minVal << " " << maxVal << "\n";
            for (int i = 0; i < numRows; i++) {
                for (int j = 0; j < numCols; j++) {
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

        void imgReformat(int **imgAry, ofstream &outFile1, int numRows, int numCols, const string& caption) {
        outFile1 << caption << endl;
        outFile1 << numRows << " " << numCols << " " << minVal << " " << maxVal << "\n";
            for (int i = 0; i < numRows; i++) {
                for (int j = 0; j < numCols; j++) {
                    if (imgAry[i][j] > 0) { 
                        outFile1 << imgAry[i][j];
                    } else {
                        outFile1 << "0";
                    }
                    outFile1 << " ";
                }
                outFile1 << "\n";
            }
        }
    };

    int main(int argc, char *argv[])
    {
        string inFile = argv[1];
        string outFile1 = argv[2];
        string deBugFile = argv[3];

        ifstream input(inFile);
        ofstream outFiles1(outFile1);
        ofstream deBugFiles(deBugFile);

        int numRows, numCols, minVal, maxVal;
        input >> numRows >> numCols >> minVal >> maxVal;

        HoughTransform line(numRows, numCols, minVal, maxVal);
        line.initalize();
        line.loadImg(input, line.imgAry);
        line.buildHoughSpace(line.imgAry, deBugFiles);

        line.reformatPrettyPrint(line.imgAry, outFiles1, line.numRows, line.numCols, "Input Image:");
        line.reformatPrettyPrint(line.cartesianAry, outFiles1, line.houghDist, line.houghAngle, "Cartesian Hough Space:");
        line.reformatPrettyPrint(line.polarAry, outFiles1, line.houghDist, line.houghAngle, "Polar Hough Space:");

        input.close();
        outFiles1.close();
        deBugFiles.close();

        return 0;
    }