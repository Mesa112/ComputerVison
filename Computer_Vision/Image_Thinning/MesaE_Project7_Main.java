import java.io.*;

class Thinning {
    int numRows, numCols, minVal, maxVal, changeCount, cycleCount;
    int[][] aryOne;
    int[][] aryTwo;

    public Thinning(int numRows, int numCols, int minVal, int maxVal) {
        this.numRows = numRows;
        this.numCols = numCols;
        this.minVal = minVal;
        this.maxVal = maxVal;

        aryOne = new int[numRows + 2][numCols + 2];
        aryTwo = new int[numRows + 2][numCols + 2];
    }

    public void loadImage(int[][] inAry, BufferedReader inFile) throws IOException {
        for (int i = 1; i < numRows + 1; i++) {
            String line = inFile.readLine();
            if (line != null) {
                String[] values = line.trim().split("\\s+");
                for (int j = 1; j < numCols + 1; j++) {
                    inAry[i][j] = Integer.parseInt(values[j - 1]);
                }
            }
        }
    }

    public int countNonZeroNeighbors(int[][] inAry, int i, int j) {
        int count = 0;
        int[] neighborhood = {
                inAry[i - 1][j - 1], inAry[i - 1][j], inAry[i - 1][j + 1],
                inAry[i][j - 1], inAry[i][j + 1],
                inAry[i + 1][j - 1], inAry[i + 1][j], inAry[i + 1][j + 1]
        };
        for (int x : neighborhood) {
            if (x > 0) {
                count++;
            }
        }
        return count;
    }

    public void copyArys(int[][] inAry, int[][] outAry) {
        for (int i = 1; i < numRows + 1; i++) {
            for (int j = 1; j < numCols + 1; j++) {
                outAry[i][j] = inAry[i][j];
            }
        }
    }

    public void zeroFramed(int[][] inAry) {
        for (int j = 0; j < numCols + 2; j++) {
            inAry[0][j] = 0;
            inAry[numRows + 1][j] = 0;
        }
        for (int i = 0; i < numRows + 2; i++) {
            inAry[i][0] = 0;
            inAry[i][numCols + 1] = 0;
        }
    }

    public void prettyDotPrint(int[][] inAry, BufferedWriter outFile) throws IOException {
        outFile.write(numRows + " " + numCols + " " + minVal + " " + maxVal + "\n");
        int fieldWidth = String.valueOf(maxVal).length();
        for (int i = 1; i < numRows + 1; i++) {
            for (int j = 1; j < numCols + 1; j++) {
                String value = inAry[i][j] == 0 ? "." : String.valueOf(inAry[i][j]);
                outFile.write(String.format("%-" + fieldWidth + "s", value));
            }
            outFile.newLine();
        }
        outFile.newLine();
    }

    public void prettyPrint(int[][] inAry, BufferedWriter outFile) throws IOException {
        outFile.write(numRows + " " + numCols + " " + minVal + " " + maxVal + "\n");
        int fieldWidth = String.valueOf(maxVal).length();
        for (int i = 1; i < numRows + 1; i++) {
            for (int j = 1; j < numCols + 1; j++) {
                String value = inAry[i][j] == 0 ? " " : String.valueOf(inAry[i][j]);
                outFile.write(String.format("%-" + fieldWidth + "s", value));
            }
            outFile.newLine();
        }
        outFile.newLine();
    }

    public void nothThinning(int[][] inAryOne, int[][] inAryTwo, BufferedWriter logFile) throws IOException {
        logFile.write("Entering northThinning();\n" + "cycleCount = " + cycleCount + ";\n" + "changeCount = "
                + changeCount + ";\n");
        for (int i = 1; i < numRows + 1; i++) {
            for (int j = 1; j < numCols + 1; j++) {
                if (inAryOne[i][j] > 0 && inAryOne[i - 1][j] == 0) {
                    int[][] currNeighborhood = getNeighborhood(inAryOne, i, j);
                    int nonZeroCount = countNonZeroNeighbors(inAryOne, i, j);
                    boolean Flag = checkConnector(currNeighborhood);
                    logFile.write("in northThinning, i = " + i + "; j = " + j + ";  nonZeroCount = " + nonZeroCount
                            + "; Flag = " + Flag + "\n");
                    if (nonZeroCount >= 4 && Flag == false) {
                        inAryTwo[i][j] = 0;
                        changeCount++;
                    } else {
                        inAryTwo[i][j] = inAryOne[i][j];
                    }
                } else {
                    inAryTwo[i][j] = inAryOne[i][j];
                }
            }
        }
        logFile.write("Leaving northThinning(); cycleCount =  " + cycleCount + "; changeCount = " + changeCount + "\n");
    }

    public void southThinning(int[][] inAryOne, int[][] inAryTwo, BufferedWriter logFile) throws IOException {
        logFile.write("Entering SouthThinning();\n" + "cycleCount = " + cycleCount + ";\n" + "changeCount = "
                + changeCount + ";\n");
        for (int i = 1; i < numRows + 1; i++) {
            for (int j = 1; j < numCols + 1; j++) {
                if (inAryOne[i][j] > 0 && inAryOne[i + 1][j] == 0) {
                    int[][] currNeighborhood = getNeighborhood(inAryOne, i, j);
                    int nonZeroCount = countNonZeroNeighbors(inAryOne, i, j);
                    boolean Flag = checkConnector(currNeighborhood);
                    logFile.write("in SouthThinning, i = " + i + "; j = " + j + ";  nonZeroCount = " + nonZeroCount
                            + "; Flag = " + Flag + "\n");
                    if (nonZeroCount >= 4 && !Flag) {
                        inAryTwo[i][j] = 0;
                        changeCount++;
                    } else {
                        inAryTwo[i][j] = inAryOne[i][j];
                    }
                } else {
                    inAryTwo[i][j] = inAryOne[i][j];
                }
            }
        }
        logFile.write("Leaving SouthThinning(); cycleCount =  " + cycleCount + "; changeCount = " + changeCount + "\n");
    }

    public void westThinning(int[][] inAryOne, int[][] inAryTwo, BufferedWriter logFile) throws IOException {
        logFile.write("Entering WestThinning();\n" + "cycleCount = " + cycleCount + ";\n" + "changeCount = "
                + changeCount + ";\n");
        for (int i = 1; i < numRows + 1; i++) {
            for (int j = 1; j < numCols + 1; j++) {
                if (inAryOne[i][j] > 0 && inAryOne[i][j - 1] == 0) {
                    int[][] currNeighborhood = getNeighborhood(inAryOne, i, j);
                    int nonZeroCount = countNonZeroNeighbors(inAryOne, i, j);
                    boolean Flag = checkConnector(currNeighborhood);
                    logFile.write("in WestThinnig, i = " + i + "; j = " + j + ";  nonZeroCount = " + nonZeroCount
                            + "; Flag = " + Flag + "\n");
                    if (nonZeroCount >= 4 && Flag == false) {
                        inAryTwo[i][j] = 0;
                        changeCount++;
                    } else {
                        inAryTwo[i][j] = inAryOne[i][j];
                    }
                } else {
                    inAryTwo[i][j] = inAryOne[i][j];
                }
            }
        }
        logFile.write("Leaving WestThinning(); cycleCount =  " + cycleCount + "; changeCount = " + changeCount + "\n");
    }

    public void eastThinning(int[][] inAryOne, int[][] inAryTwo, BufferedWriter logFile) throws IOException {
        logFile.write("Entering EastThinning();\n" + "cycleCount = " + cycleCount + ";\n" + "changeCount = "
                + changeCount + ";\n");
        for (int i = 1; i < numRows + 1; i++) {
            for (int j = 1; j < numCols + 1; j++) {
                if (inAryOne[i][j] > 0 && inAryOne[i][j + 1] == 0) {
                    int[][] currNeighborhood = getNeighborhood(inAryOne, i, j);
                    int nonZeroCount = countNonZeroNeighbors(inAryOne, i, j);
                    boolean Flag = checkConnector(currNeighborhood);
                    logFile.write("in EastThinning, i = " + i + "; j = " + j + ";  nonZeroCount = " + nonZeroCount
                            + "; Flag = " + Flag + "\n");
                    if (nonZeroCount >= 4 && Flag == false) {
                        inAryTwo[i][j] = 0;
                        changeCount++;
                    } else {
                        inAryTwo[i][j] = inAryOne[i][j];
                    }
                } else {
                    inAryTwo[i][j] = inAryOne[i][j];
                }

            }
        }
        logFile.write("Leaving EastThinning(); cycleCount =  " + cycleCount + "; changeCount = " + changeCount + "\n");
    }

    public int[][] getNeighborhood(int[][] inAry, int i, int j) {
        int[][] neighborhood = new int[3][3];
        for (int x = -1; x <= 1; x++) {
            for (int y = -1; y <= 1; y++) {
                neighborhood[x + 1][y + 1] = inAry[i + x][j + y];
            }
        }
        return neighborhood;
    }

    public boolean checkConnector(int[][] neighborhood) {
        // Configuration 1:
        // x 0 x
        // x x x
        // x 0 x
        if (neighborhood[0][1] == 0 && neighborhood[2][1] == 0) {
            return true;
        }
        // Configuration 2:
        // x x x
        // 0 x 0
        // x x x
        if (neighborhood[1][0] == 0 && neighborhood[1][2] == 0) {
            return true;
        }
        // Configuration 3:
        // 1 0 x
        // 0 x x
        // x x x
        if (neighborhood[0][0] == 1 && neighborhood[0][1] == 0 && neighborhood[1][0] == 0) {
            return true;
        }
        // Configuration 4:
        // x 0 1
        // x x 0
        // x x x
        if (neighborhood[0][1] == 0 && neighborhood[0][2] == 1 && neighborhood[1][2] == 0) {
            return true;
        }
        // Configuration 5:
        // x x x
        // 0 x x
        // 1 0 x
        if (neighborhood[1][0] == 0 && neighborhood[2][0] == 1 && neighborhood[2][1] == 0) {
            return true;
        }
        // Configuration 6:
        // x x x
        // x x 0
        // x 0 1
        if (neighborhood[2][1] == 0 && neighborhood[2][2] == 1 && neighborhood[1][2] == 0) {
            return true;
        }
        return false;
    }

    public void thinning(int[][] inAryOne, int[][] inAryTwo, BufferedWriter logFile) throws IOException {
        logFile.write("Entering Thinning() before thinning 4 sides, aryOne is below\n");

        prettyDotPrint(inAryOne, logFile);
        changeCount = 0;

        nothThinning(inAryOne, inAryTwo, logFile);
        logFile.write("After NorthThinning; aryTwo is below:\n");
        prettyDotPrint(inAryTwo, logFile);
        copyArys(inAryTwo, inAryOne);

        southThinning(inAryOne, inAryTwo, logFile);
        logFile.write("After SouthThinning; aryTwo is below:\n");
        prettyDotPrint(inAryTwo, logFile);
        copyArys(inAryTwo, inAryOne);

        westThinning(inAryOne, inAryTwo, logFile);
        logFile.write("After WestThinning; aryTwo is below:\n");
        prettyDotPrint(inAryTwo, logFile);
        copyArys(inAryTwo, inAryOne);

        eastThinning(inAryOne, inAryTwo, logFile);
        logFile.write("After EastThinning; aryTwo is below:\n");
        prettyDotPrint(inAryTwo, logFile);
        copyArys(inAryTwo, inAryOne);

        logFile.write("Leaving Thinning(); cycleCount = " + cycleCount + "; changeCount = " + changeCount + "\n");
    }
}

public class MesaE_Project7_Main {
    public static void main(String[] args) throws IOException {
        if (args.length < 3) {
            System.err.println("Error: Insufficient number of arguments.");
            return;
        }

        System.out.println("Correct number of arguments provided!");
        for (int i = 0; i < args.length; i++) {
            System.out.println("Argument " + i + ": " + args[i]);
        }
        BufferedReader inFile = new BufferedReader(new FileReader(args[0]));
        BufferedWriter outFile = new BufferedWriter(new FileWriter(args[1]));
        BufferedWriter logFile = new BufferedWriter(new FileWriter(args[2]));
        String line = inFile.readLine();
        if (line == null) {
            System.err.println("Error: Input file is empty.");
            return;
        }
        String[] header = line.trim().split("\\s+");
        if (header.length < 4) {
            System.err.println("Error: Invalid header format in input file.");
            return;
        }
        int numRows = Integer.parseInt(header[0]);
        int numCols = Integer.parseInt(header[1]);
        int minVal = Integer.parseInt(header[2]);
        int maxVal = Integer.parseInt(header[3]);

        Thinning img = new Thinning(numRows, numCols, minVal, maxVal);

        img.changeCount = 0;
        img.cycleCount = 0;

        img.loadImage(img.aryOne, inFile);
        outFile.write("In main(), before thinning, changeCount = " + img.changeCount + "; cycleCount = "
                + img.changeCount + ";\n");
        img.prettyDotPrint(img.aryOne, outFile);
        while (img.changeCount >= 0) {
            img.thinning(img.aryOne, img.aryTwo, logFile);
            img.cycleCount++;
            outFile.write("In main() inside iteration, changeCount = " + img.changeCount + "; cycleCount = "
                    + img.cycleCount + ";\n");
            img.prettyDotPrint(img.aryOne, outFile);
        }
        outFile.write("In main(), the final skeleton, changeCount = " + img.changeCount + "; cycleCount = "
                + img.cycleCount + ";\n");
        img.prettyPrint(img.aryOne, outFile);
        inFile.close();
        outFile.close();
        logFile.close();
    }
}