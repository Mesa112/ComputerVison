# Computer Vision Techniques Implementation

This repository contains fundamental computer vision algorithms implemented from scratch in C++ and Java. These implementations focus on understanding the mathematical foundations and low-level operations of various image processing techniques.

## Key Implementations

### 1. Image Thresholding (Java)
- Binary and non-binary thresholding implementations
- Pixel-level manipulation for optimal threshold selection
- Direct array operations for image processing

### 2. Gaussian Curve Fitting (C++)
- Implementation of Gaussian distribution for histogram analysis
- Statistical approach to threshold selection
- Mathematical modeling for image segmentation

### 3. Morphological Operations (C++)
- Implementation of basic morphological operators:
  - Dilation: Expanding image regions
  - Erosion: Shrinking image regions
- Kernel-based operations with customizable structuring elements

### 4. Hough Transform (C++)
- Line detection implementation from mathematical principles
- Transformation from image space to parameter space
- Peak detection for line identification

## Implementation Approach

All implementations work with images in .txt format to focus on the mathematical foundations:
- Raw pixel values are processed directly
- No dependency on external computer vision libraries
- Emphasis on understanding the underlying algorithms

## Example Results

### Hough Transform, Sobel Edge Detection, and Gaussian Smoothing

| Hough Transform | Edge Detection using Sobel Operator | Gaussian Smoothing |
|-----------------|-------------------------------------|--------------------|
| <img src="https://i.sstatic.net/7XbC8.png" width="300" alt="Hough Transform Example"> | <img src="https://miro.medium.com/v2/resize:fit:4800/format:webp/1*4lPMjSPaS2JLWZAaYrXr2Q.jpeg" width="300" alt="Sobel Edge Detection"> | <img src="https://ars.els-cdn.com/content/image/3-s2.0-B9780123965493000033-f03-42-9780123965493.jpg" width="300" alt="Gaussian Smoothing"> |

- **Hough Transform**: Converts edge points to sinusoidal curves in parameter space, enabling robust line detection.
- **Edge Detection**: Sobel operator calculates gradient magnitude and direction.
- **Gaussian Smoothing**: Demonstrates the effect of kernel sizes on noise reduction.


## Project Structure
```
├── Bi-Gaussian_Thresholding/    # Threshold selection using Gaussian fitting
├── Chain_Code/                  # Contour following implementation
├── Connected_Components/        # Region labeling and analysis
├── Distance_Transform/          # Distance mapping implementation
├── Hough_Transform/            # Line detection algorithm
├── Image_Enhancement/          # Various enhancement techniques
├── Image_Thinning/            # Skeleton extraction methods
├── Image_Thresholding/        # Multiple thresholding approaches
└── Morphology/                # Morphological operations
```

## Technical Details

### Input Format
- Images are read as text files containing pixel values
- Supports grayscale images with values 0-255
- Matrix representation for direct mathematical operations

### Implementation Features
- Pure C++ and Java implementations
- No external libraries for core algorithms
- Focus on mathematical principles and efficiency

## Building and Running

### Prerequisites
- C++ Compiler (GCC/G++ recommended)
- Java JDK 8 or higher
- Make (optional, for building C++ projects)

### Compilation
For C++ programs:
```bash
g++ -o program_name source_file.cpp
```

For Java programs:
```bash
javac ProgramName.java
```

### Running
For C++ executables:
```bash
./program_name input_image.txt output_image.txt
```

For Java programs:
```bash
java ProgramName input_image.txt output_image.txt
```

## Educational Value

This repository serves as a learning resource for:
- Understanding core computer vision concepts
- Implementing mathematical algorithms from scratch
- Learning low-level image processing techniques
- Developing efficient computational methods

## Contributing

Contributions to improve implementations or add new algorithms are welcome:
1. Fork the repository
2. Create your feature branch
3. Commit your changes
4. Push to the branch
5. Create a Pull Request

## License

This project is licensed under the MIT License - see the LICENSE file for details.
