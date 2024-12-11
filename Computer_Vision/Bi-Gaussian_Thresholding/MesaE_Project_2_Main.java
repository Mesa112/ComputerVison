import java.io.*;
import java.util.*;

class BiMeans {

    int numRows, numCols, minVal, maxVal, BiGaussThrVal, histHeight, maxHeight;
    int[] histAry;
    int[] GaussAry;
    int[] bestFitGaussAry;
    char[][] GaussGraph;
    char[][] gapGraph;

    public BiMeans(int numRows, int numCols, int minVal, int maxVal, int histHeight) {
        this.numRows = numRows;
        this.numCols = numCols;
        this.minVal = minVal;
        this.maxVal = maxVal;
        this.histHeight = histHeight;
        this.maxHeight = 0;

        histAry = new int[maxVal + 1];
        GaussAry = new int[maxVal + 1];
        bestFitGaussAry = new int[maxVal + 1];
    }

    public int loadHist(BufferedReader inFile) throws IOException {
        //logFile.write("Entering loadHist()\n");
        histAry = new int[maxVal + 1];
        String line;
        int histHeight = 0;

        while ((line = inFile.readLine()) != null) {
            line = line.trim();
            // emptylines
            if (line.isEmpty()) {
                continue;
            }
            String[] parts = line.split("\\s+");
            int i = Integer.parseInt(parts[0]);
            int val = Integer.parseInt(parts[1]);

            if (i > maxVal) {
                /*logFile.write("Warning: Data index " + i + " exceeds the maxVal " + maxVal
                        + " specified in the header. I will ignore this value.\n");*/
                continue;
            }

            histAry[i] = val;
            if (val > histHeight) {
                histHeight = val;
            }
        }
        this.histHeight = histHeight;
        GaussGraph = new char[maxVal + 1][histHeight + 1];
        gapGraph = new char[maxVal + 1][histHeight + 1];
        //ogFile.write("leaving loadHist()\n");
        return histHeight;
    }

    public void printHist(BufferedWriter histFile) throws IOException {
        //logFile.write("Now entering printHist()\n");
        histFile.write(numRows + " " + numCols + " " + minVal + " " + maxVal + "\n");
        for (int i = 0; i <= maxVal; i++) {
            int count = histAry[i];
            histFile.write(i + " " + count + "\n");
        }
        //logFile.write("Now leaving printHist()\n");
    }

    public void displayHist(BufferedWriter histFile) throws IOException {
        //logFile.write("Now Entering displayHist()\n");
        histFile.write(numRows + " " + numCols + " " + minVal + " " + maxVal + "\n");
        for (int i = 0; i <= maxVal; i++) {
            int count = histAry[i];
            histFile.write(i + " (" + count + "):");
            for (int j = 0; j < count; j++) {
                histFile.write("+");
            }
            histFile.write("\n");
        }
        //logFile.write("Now leaving diplayHist()\n");
    }

    public void copyArys(int[] ary1, int[] ary2) {
        for (int i = 0; i < maxVal + 1; i++) {
            ary2[i] = ary1[i];
        }
    }

    public void setZero() {
        for (int i = 0; i < GaussAry.length; i++) {
            GaussAry[i] = 0;
        }
    }

    public void setBlanks(char[][] graph) {
        for (int i = 0; i < graph.length; i++) {
            for (int j = 0; j < graph[i].length; j++) {
                graph[i][j] = ' ';
            }
        }
    }

    public int biGaussian() throws IOException {
        //logFile.write("Now entering biGaussian method()\n");
        double sum1, sum2, total, minSumDiff;
        int offset = (int) (maxVal - minVal) / 10;
        int dividePt = offset;
        int bestThr = dividePt;
        minSumDiff = 99999;
        while (dividePt < (maxVal - offset)) {
            setZero();
            sum1 = fitGauss(0, dividePt);
            sum2 = fitGauss(dividePt, maxVal);
            total = sum1 + sum2;
            if (total < minSumDiff) {
                minSumDiff = total;
                bestThr = dividePt;
                copyArys(GaussAry, bestFitGaussAry);
            }
            /*logFile.write("In biGaussian ():\ndividePt = " + dividePt +
                    ", sum1 = " + sum1 +
                    ", sum2 = " + sum2 +
                    ", total = " + total +
                    ", minSumDiff = " + minSumDiff +
                    ", bestThr = " + bestThr + "\n");*/

            dividePt++;
        }
        //logFile.write("Now leaving biGaussian method(), minSumDiff = " + minSumDiff + " bestThr is " + bestThr + "\n");
        return bestThr;
    }

    public double fitGauss(int leftIndex, int rightIndex) throws IOException {
        //logFile.write("Entering fitGauss()\n");
        double mean, var, sum, Gval;
        sum = 0.0;
        mean = computeMean(leftIndex, rightIndex);
        var = computeVar(leftIndex, rightIndex, mean);
        int index = leftIndex;
        while (index <= rightIndex) {
            Gval = modifiedGauss(index, mean, var);
            sum += Math.abs(Gval - (double) histAry[index]);
            GaussAry[index] = (int) Gval;
            index++;
        }
        //logFile.write("leaving fitGauss() method, sum is " + sum + "\n");
        return sum;
    }

    public double computeMean(int leftIndex, int rightIndex) throws IOException {
        //logFile.write("Entering computeMean method\n");
        maxHeight = 0;
        double sum = 0.0;
        int numPixels = 0;
        int index = leftIndex;
        while (index < rightIndex) {
            sum += histAry[index] * index;
            numPixels += histAry[index];
            if (histAry[index] >= maxHeight) {
                maxHeight = histAry[index];
            }
            index++;
        }
        double result = (numPixels > 0) ? (sum / (double) numPixels) : 0.0;
        //logFile.write("output Leaving computeMean method maxHeight = " + maxHeight + ", result = " + result + "\n");
        return result;
    }

    public double computeVar(int leftIndex, int rightIndex, double mean) throws IOException {
        //logFile.write("Entering computeVar() method\n");
        double sum = 0.0;
        int numPixels = 0;
        int index = leftIndex;
        while (index < rightIndex) {
            sum += (double) histAry[index] * Math.pow(((double) index - mean), 2);
            numPixels += histAry[index];
            index++;
        }
        double result = (numPixels > 0) ? (sum / (double) numPixels) : 0.0;
        //logFile.write("Leaving computeVar() method, variance = " + result + "\n");
        return result;
    }

    public void plotGaussGraph() throws IOException {
        //logFile.write("Entering plotGaussGraph() method\n");
        setBlanks(GaussGraph);

        for (int index = 0; index <= maxVal; index++) {
            if (bestFitGaussAry[index] > 0) {
                for (int i = 0; i < bestFitGaussAry[index]; i++) {
                    GaussGraph[index][i] = '*';
                }
            }
        }
        //logFile.write("Leaving plotGaussGraph() method\n");
    }

    public void dispGraph(BufferedWriter outputFile, char[][] graph) throws IOException {
        //logFile.write("Entering dispGraph() method\n");

        outputFile.write(numRows + " " + numCols + " " + minVal + " " + maxVal + "\n");
        for (int i = histHeight; i >= 0; i--) {
            for (int j = 0; j <= maxVal; j++) {
                outputFile.write(graph[j][i]);
            }
            outputFile.write("\n");
        }

        //logFile.write("Leaving dispGraph() method\n");
    }

    public void plotGapGraph() throws IOException {
        //logFile.write("Entering plotGapGraph() method\n");
        setBlanks(gapGraph);
        int index = 0;
        while (index <= maxVal) {
            int end1, end2;
            if (bestFitGaussAry[index] <= histAry[index]) {
                end1 = bestFitGaussAry[index];
                end2 = histAry[index];
            } else {
                end1 = histAry[index];
                end2 = bestFitGaussAry[index];
            }
            for (int i = end1; i <= end2; i++) {
                gapGraph[index][i] = '@';
            }
            index++;
        }
        //logFile.write("Leaving plotGapGraph() method\n");
    }

    public double modifiedGauss(int x, double mean, double var) {
        // Compute the Gaussian value
        return (double) (maxHeight * Math.exp(-(Math.pow((double) (x - mean), 2) / (2 * var))));
    }

    public void printBestFitGauss(BufferedWriter gaussFile) throws IOException {
        //logFile.write("Entering printBestFitGauss() method\n");

        gaussFile.write(numRows + " " + numCols + " " + minVal + " " + maxVal + "\n");

        for (int i = 0; i <= maxVal; i++) {
            int count = bestFitGaussAry[i];
            gaussFile.write(i + " " + count + "\n");
        }

        //logFile.write("Leaving printBestFitGauss() method\n");
    }

}

public class MesaE_Project_2_Main {
    public static void main(String[] args) {

        if (args.length < 3) {
            System.err.println("Error in the number of arguments");
            return;
        }
        System.out.println("Correct number of arguments provided.");

        try {

            BufferedReader inFile1 = new BufferedReader(new FileReader(args[0]));
            BufferedWriter histFile = new BufferedWriter(new FileWriter(args[1]));
            BufferedWriter gaussFile = new BufferedWriter(new FileWriter(args[2]));
            // BufferedWriter logFile = new BufferedWriter(new FileWriter(args[3]));

            String headerLine = inFile1.readLine();
            String[] header = headerLine.trim().split("\\s+");
            int numRows = Integer.parseInt(header[0]);
            int numCols = Integer.parseInt(header[1]);
            int minVal = Integer.parseInt(header[2]);
            int maxVal = Integer.parseInt(header[3]);

            BiMeans img = new BiMeans(numRows, numCols, minVal, maxVal, 0);

            img.histHeight = img.loadHist(inFile1);

            histFile.write("** Below is the input histogram **\n");
            img.printHist(histFile);

            histFile.write("** Below is the graphic display of the input histogram **\n");
            img.displayHist(histFile);

            img.BiGaussThrVal = img.biGaussian();

            gaussFile.write("** The selected threshold value is " + img.BiGaussThrVal + " **\n");
            gaussFile.write("** Below is the best Fitted Gaussians **\n");
            img.printBestFitGauss(gaussFile);

            img.plotGaussGraph();
            gaussFile.write("** Below is the graphic display of BestFitGaussAry **\n");
            img.dispGraph(gaussFile, img.GaussGraph);

            img.plotGapGraph();
            gaussFile.write("** Below displays the gaps between the histogram and the best fitted Gaussians **\n");
            img.dispGraph(gaussFile, img.gapGraph);

            inFile1.close();
            histFile.close();
            gaussFile.close();
            //logFile.close();
        } catch (FileNotFoundException e) {
            System.err.println("Error: File not found - " + args[0]);
        } catch (IOException e) {
            System.err.println("Error reading the file.");
        }
    }
}
