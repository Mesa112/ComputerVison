import java.io.*;

class Morphology {
    int numImgRows, numImgCols, imgMin, imgMax, numStructRows, numStructCols, structMin, structMax, rowOrigin,
            colOrigin, rowFrameSize, colFrameSize, extraRows, extraCols, rowSize, colSize;
    int[][] zeroFramedAry;
    int[][] morphAry;
    int[][] tempAry;
    int[][] structAry;

    public Morphology(int numImgRows, int numImgCols, int imgMin, int imgMax, int numStructRows, int numStructCols,
            int structMin, int structMax, int rowOrigin, int colOrigin) {

        this.numImgRows = numImgRows;
        this.numImgCols = numImgCols;
        this.imgMin = imgMin;
        this.imgMax = imgMax;
        this.numStructRows = numStructRows;
        this.numStructCols = numStructCols;
        this.structMin = structMin;
        this.structMax = structMax;
        this.rowOrigin = rowOrigin;
        this.colOrigin = colOrigin;

        rowFrameSize = numStructRows / 2;
        colFrameSize = numStructCols / 2;
        extraRows = rowFrameSize * 2;
        extraCols = colFrameSize * 2;
        rowSize = numImgRows + extraRows;
        colSize = numImgCols + extraCols;

        zeroFramedAry = new int[rowSize][colSize];
        morphAry = new int[rowSize][colSize];
        tempAry = new int[rowSize][colSize];
        structAry = new int[numStructRows][numStructCols];

    }

    public void zero2DAry(int[][] Ary, int nRows, int nCols) {
        for (int i = 0; i < nRows; i++) {
            for (int j = 0; j < nCols; j++) {
                Ary[i][j] = 0;
            }
        }
    }

    public void loadImg(BufferedReader inFile, int[][] Ary) throws IOException {
        for (int i = 0; i < numImgRows; i++) {
            String line = inFile.readLine();
            if (line != null) {
                String[] values = line.trim().split("\\s+");
                for (int j = 0; j < numImgCols && j < values.length; j++) {
                    Ary[i + rowFrameSize][j + colFrameSize] = Integer.parseInt(values[j]);
                }
            }
        }
    }

    public void loadStruct(BufferedReader inFile, int[][] Ary) throws IOException {
        for (int i = 0; i < numStructRows; i++) {
            String line = inFile.readLine();
            if (line != null) {
                String[] values = line.trim().split("\\s+");
                for (int j = 0; j < numStructCols && j < values.length; j++) {
                    Ary[i][j] = Integer.parseInt(values[j]);
                }
            }
        }
    }

    public void computeDialation(int[][] inAry, int[][] outAry) {
        int i = rowFrameSize;
        while (i < rowSize - rowFrameSize) {
            int j = colFrameSize;
            while (j < colSize - colFrameSize) {
                if (inAry[i][j] > 0) {
                    onePixelDialation(i, j, inAry, outAry);
                }
                j++;
            }
            i++;
        }
    }

    public void computeErosion(int[][] inAry, int[][] outAry) {
        int i = rowFrameSize;
        while (i < rowSize - rowFrameSize) {
            int j = colFrameSize;
            while (j < colSize - colFrameSize) {
                if (inAry[i][j] > 0) {
                    onePixelErosion(i, j, inAry, outAry);
                }
                j++;
            }
            i++;
        }
    }

    public void computeOpening(int[][] inAry, int[][] outAry, int[][] temp) {
        computeErosion(inAry, tempAry);
        computeDialation(tempAry, outAry);
    }

    public void computeClosing(int[][] inAry, int[][] outAry, int[][] temp) {
        computeDialation(inAry, tempAry);
        computeErosion(tempAry, outAry);
    }

    public void onePixelDialation(int i, int j, int[][] inAry, int[][] outAry) {
        int iOffset = i - rowOrigin;
        int jOffset = j - colOrigin;

        int rIndex = 0;
        while (rIndex < numStructRows) {
            int cIndex = 0;
            while (cIndex < numStructCols) {
                if (structAry[rIndex][cIndex] > 0) {
                    outAry[iOffset + rIndex][jOffset + cIndex] = 1;
                }
                cIndex++;
                ;
            }
            rIndex++;
        }
    }

    public void onePixelErosion(int i, int j, int[][] inAry, int[][] outAry) {
        int iOffset = i - rowOrigin;
        int jOffset = j - colOrigin;

        boolean matchFlag = true;
        int rIndex = 0;
        while (matchFlag == true && rIndex < numStructRows) {
            int cIndex = 0;
            while (matchFlag == true && cIndex < numStructCols) {
                if (structAry[rIndex][cIndex] > 0 && inAry[iOffset + rIndex][jOffset + cIndex] <= 0) {
                    matchFlag = false;
                }
                cIndex++;
            }
            rIndex++;
        }
        if (matchFlag == true) {
            outAry[i][j] = 1;
        } else {
            outAry[i][j] = 0;
        }
    }

    public void AryToFile(int[][] inAry, BufferedWriter fileOut) throws IOException {
        fileOut.write(numImgRows + " " + numImgCols + " " + imgMin + " " + imgMax);
        fileOut.newLine();

        for (int i = rowFrameSize; i < rowSize - rowFrameSize; i++) {
            for (int j = colFrameSize; j < colSize - colFrameSize; j++) {
                fileOut.write(inAry[i][j] + " ");
            }
            fileOut.newLine();
        }
    }

    public void binaryPrettyPrint(int[][] inAry, BufferedWriter fileOut, String printing, int numRows, int numCols,
            boolean isStructElem) throws IOException {
        fileOut.write(printing);
        fileOut.newLine();
        if(!isStructElem){
        fileOut.write((numRows + extraRows) + " " + (numCols + extraCols) + " " + imgMin + " " + imgMax + "\n");
        }

        if (isStructElem) {
            fileOut.write(numRows + " " + numCols + " " + imgMin + " " + imgMax + "\n");
            fileOut.write(rowOrigin + " " + colOrigin + "\n");
        }

        if(!isStructElem){
                for (int i = 0; i < numRows + extraRows; i++) {
                    for (int j = 0; j < numCols + extraCols; j++) {
                        if (inAry[i][j] == 0) {
                            fileOut.write(". ");
                        } else {
                            fileOut.write(inAry[i][j] + " ");
                        }
                    }
                    fileOut.newLine();
                }
                fileOut.newLine();
            
        }else{
        for (int i = 0; i < numRows; i++) {
            for (int j = 0; j < numCols; j++) {
                if (inAry[i][j] == 0) {
                    fileOut.write(". ");
                } else {
                    fileOut.write("1 ");
                }
            }
            fileOut.newLine();
        }
        fileOut.newLine();
    }
} 


    public void process1(BufferedWriter prettyPrintFile, String outputDir) throws IOException {
        zero2DAry(morphAry, rowSize, colSize);
        computeDialation(zeroFramedAry, morphAry);
        String fileName = outputDir + "dilationOutFile.txt";
        try (BufferedWriter fileOut = new BufferedWriter(new FileWriter(fileName))) {
            AryToFile(morphAry, fileOut);
            binaryPrettyPrint(morphAry, prettyPrintFile, "Dilation Result", numImgRows, numImgCols, false);
        }
    }

    // Process 2: Erosion
    public void process2(BufferedWriter prettyPrintFile, String outputDir) throws IOException {
        zero2DAry(morphAry, rowSize, colSize);
        computeErosion(zeroFramedAry, morphAry);
        String fileName = outputDir + "erosionOutFile.txt";
        try (BufferedWriter fileOut = new BufferedWriter(new FileWriter(fileName))) {
            AryToFile(morphAry, fileOut);
            binaryPrettyPrint(morphAry, prettyPrintFile, "Erosion Result", numImgRows, numImgCols, false);
        }
    }

    // Process 3: Opening
    public void process3(BufferedWriter prettyPrintFile, String outputDir) throws IOException {
        zero2DAry(morphAry, rowSize, colSize);
        computeOpening(zeroFramedAry, morphAry, tempAry);
        String fileName = outputDir + "openingOutFile.txt";
        try (BufferedWriter fileOut = new BufferedWriter(new FileWriter(fileName))) {
            AryToFile(morphAry, fileOut);
            binaryPrettyPrint(morphAry, prettyPrintFile, "Opening Result", numImgRows, numImgCols, false);
        }
    }

    // Process 4: Closing
    public void process4(BufferedWriter prettyPrintFile, String outputDir) throws IOException {
        zero2DAry(morphAry, rowSize, colSize);
        computeClosing(zeroFramedAry, morphAry, tempAry);
        String fileName = outputDir + "closingOutFile.txt";
        try (BufferedWriter fileOut = new BufferedWriter(new FileWriter(fileName))) {
            AryToFile(morphAry, fileOut);
            binaryPrettyPrint(morphAry, prettyPrintFile, "Closing Result", numImgRows, numImgCols, false);
        }
    }

    // Process 5: All Operations
    public void process5(BufferedWriter prettyPrintFile, String outputDir) throws IOException {
        // Dilation
        process1(prettyPrintFile, outputDir);

        // Erosion
        process2(prettyPrintFile, outputDir);

        // Opening
        process3(prettyPrintFile, outputDir);

        // Closing
        process4(prettyPrintFile, outputDir);
    }

}

public class MesaE_Project4_Main {
    public static void main(String[] args) throws IOException {
        // Check if the correct number of arguments is provided
        if (args.length < 3) {
            System.out.println("Usage: java MesaE_Project4_Main <input_image_file> <struct_file> <choice>");
            System.out.println(
                    "<choice>: 1 for Dilation, 2 for Erosion, 3 for Opening, 4 for Closing, 5 for All Operations");
            return;
        }

        // Read file paths and choice from command-line arguments
        String inputImagePath = args[0];
        String structFilePath = args[1];
        int choice = Integer.parseInt(args[2]);

        // Set output directory and pretty print file path
        String outputDirPath = "./"; 
        String prettyPrintOutputPath = outputDirPath + "prettyPrintOutput.txt";

        // Open files for reading and writing
        BufferedReader inFile = new BufferedReader(new FileReader(inputImagePath));
        BufferedReader structFile = new BufferedReader(new FileReader(structFilePath));
        BufferedWriter prettyPrintFile = new BufferedWriter(new FileWriter(prettyPrintOutputPath));

        // Read image and structuring element headers
        String[] imgParams = inFile.readLine().split(" ");
        int numImgRows = Integer.parseInt(imgParams[0]);
        int numImgCols = Integer.parseInt(imgParams[1]);
        int imgMin = Integer.parseInt(imgParams[2]);
        int imgMax = Integer.parseInt(imgParams[3]);

        String[] structParams = structFile.readLine().split(" ");
        int numStructRows = Integer.parseInt(structParams[0]);
        int numStructCols = Integer.parseInt(structParams[1]);
        int structMin = Integer.parseInt(structParams[2]);
        int structMax = Integer.parseInt(structParams[3]);

        String[] originParams = structFile.readLine().split(" ");
        int rowOrigin = Integer.parseInt(originParams[0]);
        int colOrigin = Integer.parseInt(originParams[1]);

        // Initialize Morphology object
        Morphology img = new Morphology(numImgRows, numImgCols, imgMin, imgMax, numStructRows, numStructCols, structMin,
                structMax, rowOrigin, colOrigin);

        // Zero-frame the image and struct arrays
        img.zero2DAry(img.zeroFramedAry, img.rowSize, img.colSize);
        img.loadImg(inFile, img.zeroFramedAry);
        img.binaryPrettyPrint(img.zeroFramedAry, prettyPrintFile, "Zero-Framed Image", numImgRows, numImgCols, false);

        img.zero2DAry(img.structAry, img.numStructRows, img.numStructCols);
        img.loadStruct(structFile, img.structAry);
        img.binaryPrettyPrint(img.structAry, prettyPrintFile, "Structuring Element", numStructRows, numStructCols,
                true);

        // Process based on choice
        switch (choice) {
            case 1:
                img.process1(prettyPrintFile, outputDirPath);
                break;
            case 2:
                img.process2(prettyPrintFile, outputDirPath);
                break;
            case 3:
                img.process3(prettyPrintFile, outputDirPath);
                break;
            case 4:
                img.process4(prettyPrintFile, outputDirPath);
                break;
            case 5:
                img.process5(prettyPrintFile, outputDirPath);
                break;
            default:
                System.out.println("Invalid choice. Please select a value between 1 and 5.");
                break;
        }

        // Close all files
        inFile.close();
        structFile.close();
        prettyPrintFile.close();

        System.out.println("Execution done successfully");
    }
}