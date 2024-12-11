import java.io.*;
import java.util.*;

class Property{
    int label, numPixels, minR, minC, maxR, maxC;
}

class ccLabel{
    int numRows, numCols, minVal, maxVal, newLabel, trueNumCC, newMin, newMax;
    int[][] zeroFramedAry;
    int[] nonZeroNeighborAry;
    int[] EQTable;
    Property[] CCproperty;
    ccLabel(int numRows, int numCols, int minVal, int maxVal) {
        this.numRows = numRows;
        this.numCols = numCols;
        this.minVal = minVal;
        this.maxVal = maxVal;
        this.newLabel = 0;

        zeroFramedAry = new int[numRows + 2][numCols + 2];
        nonZeroNeighborAry = new int[5];
        EQTable = new int[(numRows * numCols)/4];
        for (int i = 0; i < EQTable.length; i++) {
                EQTable[i] = i;
}
    }

    public void zero2DAry(int[][] inAry){
        for (int i = 0; i < numRows + 2; i++){
            for (int j = 0; j < numCols + 2; j++){
                inAry[i][j] = 0;
            }
        }
    }

    public void loadStruct(BufferedReader inFile, int[][] inAry) throws IOException {
        for (int i = 1; i <= numRows; i++) {
            String line = inFile.readLine();
            if (line != null) {
                String[] values = line.trim().split("\\s+");
                for (int j = 1; j <= numCols && (j - 1) < values.length; j++) {
                    inAry[i][j] = Integer.parseInt(values[j - 1]);
                }
            }
        }
    }
    

    public void printImg(int[][] inAry, BufferedWriter labelFile) throws IOException {
        labelFile.write(numRows + " " + numCols + " " + newMin + " " + newMax + "\n");
        int fieldWidth = String.valueOf(newMax).length() + 1;
        for (int i = 1; i <= numRows; i++) {
            for (int j = 1; j <= numCols; j++) {
                labelFile.write(String.format("%-" + fieldWidth + "d", inAry[i][j]));
            }
            labelFile.newLine();
        }
        labelFile.newLine();
    }
    

    public void prettyDotPrint(int[][] inAry, BufferedWriter outFile, BufferedWriter logFile) throws IOException {
        logFile.write("Enter PrettyDotPrint()\n");
        outFile.write(numRows + " " + numCols + " " + newMin + " " + newMax + "\n");
        int fieldWidth = String.valueOf(newLabel).length() + 1;
    
        for (int i = 1; i < numRows + 1; i++) {
            for (int j = 1; j < numCols + 1; j++) {
                String value = inAry[i][j] == 0 ? "." : String.valueOf(inAry[i][j]);
                outFile.write(String.format("%-" + fieldWidth + "s", value));
            }
            outFile.newLine();
        }
        outFile.newLine();
    
        logFile.write("Leaving PrettyDotPrint()\n");
    }
    
        
    public void connect8Pass1(int[][] inAry, BufferedWriter logFile) throws IOException {
        logFile.write("Entering connect8Pass1 method\n");
        for (int i = 1; i <= numRows; i++) {
            for (int j = 1; j <= numCols; j++) {
                if (inAry[i][j] > 0) {
                    int a = inAry[i - 1][j - 1];
                    int b = inAry[i - 1][j];
                    int c = inAry[i - 1][j + 1];
                    int d = inAry[i][j - 1];
    
                    int[] neighborLabels = {a, b, c, d};
                    int minLabel = Integer.MAX_VALUE;
                    boolean allZero = true;
                    for (int label : neighborLabels) {
                        if (label > 0) {
                            allZero = false;
                            minLabel = Math.min(minLabel, label);
                        }
                    }
                    if (allZero) {
                        newLabel++;
                        inAry[i][j] = newLabel;
                        EQTable[newLabel] = newLabel;
                    } else {
                        inAry[i][j] = minLabel;
                        for (int label : neighborLabels) {
                            if (label > 0 && label != minLabel) {
                                EQTable[label] = Math.min(minLabel, label);
                            }
                        }
                    }
                    newMax = Math.max(newMax, inAry[i][j]);
                }
            }
        }
        logFile.write("Leaving connect8Pass1 method\n");
    }
    
    

    public void connect8Pass2(int[][] inAry, BufferedWriter logFile) throws IOException {
        logFile.write("Entering connect8Pass2 method\n");
        for (int i = numRows; i >= 1; i--) {
            for (int j = numCols; j >= 1; j--) {
                if (inAry[i][j] > 0) {
                    int e = inAry[i][j + 1];
                    int f = inAry[i + 1][j - 1];
                    int g = inAry[i + 1][j];
                    int h = inAry[i + 1][j + 1];
                    int[] neighbors = {e, f, g, h};
                    int minLabel = inAry[i][j];
                    for (int label : neighbors) {
                        if (label > 0) {
                            minLabel = Math.min(minLabel, label);
                        }
                    }
                    if (minLabel != inAry[i][j]) {
                        inAry[i][j] = minLabel;
                        for(int label : neighbors){
                            if(label > 0){
                                EQTable[label] = Math.min(minLabel, label);
                            }
                        }
                    }
                    newMax = Math.max(newMax, inAry[i][j]);
                }
            }
        }
        logFile.write("Leaving connect8Pass2 method\n");
    }
    

    public void connect4Pass1(int[][] inAry, BufferedWriter logFile) throws IOException {
        logFile.write("Entering connect4Pass1 method\n");
        for (int i = 1; i <= numRows; i++) {
            for (int j = 1; j <= numCols; j++) {
                if (inAry[i][j] > 0) {
                    int b = inAry[i - 1][j];
                    int d = inAry[i][j - 1];
    
                    int[] neighborLabels = {    b,d};
                    int minLabel = Integer.MAX_VALUE;
                    if(b == 0 && d == 0){
                        newLabel++;
                        inAry[i][j] = newLabel;
                    }else {
                        for (int label : neighborLabels) {
                            if (label > 0) {
                                minLabel = Math.min(minLabel, label);
                                EQTable[label] = minLabel;
                            }
                        }
                        inAry[i][j] = minLabel;
                    }
                    newMax = Math.max(newMax, inAry[i][j]);
                }
            }
        }
        logFile.write("Leaving connect4Pass1 method\n");
    }

    public void connect4Pass2(int[][] inAry, BufferedWriter logFile) throws IOException {
        logFile.write("Entering connect4Pass2 method\n");
        for (int i = numRows + 1; i < 1; i++) {
            for (int j = numCols + 1; j < 1; j++) {
                if (inAry[i][j] > 0) {
                    int e = inAry[i][j + 1];
                    int g = inAry[i + 1][j];
                    int[] neighbors = {
                        e,g
                    };
                    int minLabel = Integer.MAX_VALUE;
                    if (e == 0 && g == 0) {
                        continue;
                    } else {
                        for (int label : neighbors) {
                            if (label > 0) {
                                minLabel = Math.min(minLabel, label);
                            }
                        }
                        if (minLabel != inAry[i][j]) {
                            EQTable[inAry[i][j]] = minLabel;
                            inAry[i][j] = minLabel;
                        }
                    }
                }
                newMax = Math.max(newMax, inAry[i][j]);
            }
        }
        logFile.write("Leaving connect4Pass2 method\n");
    }
    
    
    public void connectPass3(int[][] inAry, int[] EQTable, BufferedWriter logFile) throws IOException {
        logFile.write("Entering connectPass3 method\n");
        CCproperty = new Property[trueNumCC + 1];

        for (int i = 1; i <= trueNumCC; i++) {
            CCproperty[i] = new Property();
            CCproperty[i].label = i;
            CCproperty[i].numPixels = 0;
            CCproperty[i].minR = numRows;
            CCproperty[i].maxR = 0;
            CCproperty[i].minC = numCols;
            CCproperty[i].maxC = 0;
        }

        for (int i = 1; i <= numRows; i++) {
            for (int j = 1; j <= numCols; j++) {
                if (inAry[i][j] > 0) {
                    inAry[i][j] = EQTable[inAry[i][j]];
                    int k = inAry[i][j];

                    CCproperty[k].numPixels++;
                    if (i < CCproperty[k].minR) {
                        CCproperty[k].minR = i;
                    }
                    if (i > CCproperty[k].maxR) {
                        CCproperty[k].maxR = i;
                    }
                    if (j < CCproperty[k].minC) {
                        CCproperty[k].minC = j;
                    }
                    if (j > CCproperty[k].maxC) {
                        CCproperty[k].maxC = j;
                    }
                }
            }
        }
        logFile.write("Leaving connectPass3 method\n");
    }
    
    
    public void updateEQTable(int[] inAry, int minLabel, int[] EQTable) {
        for (int label : inAry) {
            if (label > 0 && label != minLabel) {
                EQTable[label] = minLabel;
            }
        }
    }    

    public int manageEQTable(int[] EQTable) {
        int relabel = 0;
        for (int i = 1; i <= newLabel; i++) {
            if (i != EQTable[i]) {
                EQTable[i] = EQTable[EQTable[i]];
            } else {    
                relabel++;
                EQTable[i] = relabel;
            }
        }
        return relabel;
    }
    
    

    public void printCCproperty(BufferedWriter outFile) throws IOException {
        outFile.write(numRows + " " + numCols + " " + minVal + " " + maxVal);
        outFile.newLine();
        outFile.write(String.valueOf(trueNumCC));
        outFile.newLine();
        for (int i = 1; i <= trueNumCC; i++) {
            outFile.write(String.valueOf(CCproperty[i].label));
            outFile.newLine();
            outFile.write(String.valueOf(CCproperty[i].numPixels));
            outFile.newLine();
            outFile.write(CCproperty[i].minR + " " + CCproperty[i].minC);
            outFile.newLine();
            outFile.write(CCproperty[i].maxR + " " + CCproperty[i].maxC);
            outFile.newLine();
        }
    }
    
    
    

    public void printEQTable(BufferedWriter prettyPrintFile) throws IOException {
        prettyPrintFile.write("Index    EQTable[index]\n");
        prettyPrintFile.write("-------------------------------------------------------\n");
        for (int i = 0; i <= newLabel; i++) {
            prettyPrintFile.write(i + "        " + EQTable[i] + "\n");
        }
    }

    public void connected4(int[][] inAry, int[] EQTable, BufferedWriter prettyPrintFile, BufferedWriter logFile) throws IOException {
        logFile.write("Entering connected4 method\n");
        prettyPrintFile.write("After connect4 pass 1:\n");
        connect4Pass1(inAry, logFile);
        logFile.write("After connected4 pass1, newLabel = " + this.newLabel + "\n");
        prettyDotPrint(inAry, prettyPrintFile, logFile);
        printEQTable(prettyPrintFile);
        prettyPrintFile.write("After connect4 pass 2:\n");
        connect4Pass2(inAry, logFile);
        logFile.write("After connected4 pass2, newLabel = " + this.newLabel + "\n");
        prettyDotPrint(inAry, prettyPrintFile, logFile);
        printEQTable(prettyPrintFile);
        trueNumCC = manageEQTable(EQTable);
        printEQTable(prettyPrintFile);
        newMin = 0;
        newMax = trueNumCC;
    
        CCproperty = new Property[trueNumCC + 1];
        logFile.write("In connected4, after manageEQTable, trueNumCC = " + this.trueNumCC + "\n");
        prettyPrintFile.write("After connect4 pass 3:\n");
        connectPass3(inAry, EQTable, logFile);
        prettyDotPrint(inAry, prettyPrintFile, logFile);
        printEQTable(prettyPrintFile);
        logFile.write("Leaving connected4 method\n");
    }
    

    public void connected8(int[][] inAry, int[] EQTable, BufferedWriter prettyPrintFile, BufferedWriter logFile) throws IOException {
        logFile.write("Entering connected8 method\n");
        prettyPrintFile.write("After connect8 pass 1:\n");
        connect8Pass1(inAry, logFile);
        logFile.write("After connected8 pass1, newLabel = " + newLabel + "\n");
        prettyDotPrint(inAry, prettyPrintFile, logFile );
        prettyPrintFile.write("EQTable after pass 1:\n");
        printEQTable(prettyPrintFile);
        connect8Pass2(inAry, logFile);
        prettyPrintFile.write("After connect8 pass 2:\n");
        logFile.write("After connected8 pass2, newLabel = " + newLabel + "\n");
        prettyDotPrint(inAry, prettyPrintFile, logFile );
        prettyPrintFile.write("EQTable after pass 2:\n");
        trueNumCC = manageEQTable(EQTable);
        printEQTable(prettyPrintFile);
        newMin = 0;
        newMax = trueNumCC;
        CCproperty = new Property[trueNumCC + 1];
        logFile.write("In connected8, after manage EQTable, trueNumCC = " + trueNumCC + "\n");
        prettyPrintFile.write("After connect8 pass 3:\n");
        connectPass3(inAry, EQTable, logFile);
        prettyDotPrint(inAry, prettyPrintFile, logFile);
        prettyPrintFile.write("EQTable after pass 3:\n");
        printEQTable(prettyPrintFile);
        logFile.write("Leaving connected8 method\n");
    }
    
    public void drawBoxes(int[][] inAry, Property[] CCproperty, int trueNumCC, BufferedWriter logFile) throws IOException {
        logFile.write("Entering drawBoxes method\n");
        for (int index = 1; index <= trueNumCC; index++) {
            int minRow = CCproperty[index].minR;
            int minCol = CCproperty[index].minC;
            int maxRow = CCproperty[index].maxR;
            int maxCol = CCproperty[index].maxC;
            int label = CCproperty[index].label;
            for (int j = minCol; j <= maxCol; j++) {
                inAry[minRow][j] = label;
                inAry[maxRow][j] = label;
            }
            for (int j = minRow; j <= maxRow; j++) {
                inAry[j][minCol] = label;
                inAry[j][maxCol] = label;
            }
        }
        logFile.write("Leaving drawBoxes method\n");
    }

public class MesaE_Project6_Main {
    public static void main(String[] args) throws IOException {
        // Check for sufficient arguments
        if (args.length < 6) {
            System.err.println("Error: Insufficient number of arguments.");
            return;
        }

        System.out.println("Correct number of arguments provided!");
        for (int i = 0; i < args.length; i++) {
            System.out.println("Argument " + i + ": " + args[i]);
        }

        int connectness = Integer.parseInt(args[1]);

        // Use try-with-resources to manage resources automatically
        try (
            BufferedReader inFile = new BufferedReader(new FileReader(args[0]));
            BufferedWriter prettyPrintFile = new BufferedWriter(new FileWriter(args[2]));
            BufferedWriter labelFile = new BufferedWriter(new FileWriter(args[3]));
            BufferedWriter propertyFile = new BufferedWriter(new FileWriter(args[4]));
            BufferedWriter logFile = new BufferedWriter(new FileWriter(args[5]))
        ) {
            // Read header information from the input file
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

            // Initialize ccLabel object
            ccLabel img = new ccLabel(numRows, numCols, minVal, maxVal);

            img.zero2DAry(img.zeroFramedAry);
            img.loadStruct(inFile, img.zeroFramedAry);

            // Process based on connectivity
            if (connectness == 4) {
                img.connected4(img.zeroFramedAry, img.EQTable, prettyPrintFile, logFile);
            } else if (connectness == 8) {
                img.connected8(img.zeroFramedAry, img.EQTable, prettyPrintFile, logFile);
            } else {
                System.err.println("Error: Invalid connectivity value. Must be 4 or 8.");
                return;
            }

            img.printImg(img.zeroFramedAry, labelFile);
            img.printCCproperty(propertyFile);
            img.drawBoxes(img.zeroFramedAry, img.CCproperty, img.trueNumCC, logFile);
            prettyPrintFile.write("Bounding Boxes Img:\n");
            img.prettyDotPrint(img.zeroFramedAry, prettyPrintFile, logFile);
            prettyPrintFile.write("The true number of connected components: " + img.trueNumCC + "\n");

            } // Resources are automatically closed here, even if an exception occurs
        }
    }
}