---

# PGM Image Compressor and Decompressor  

This project is a C++ implementation of a simple **PGM (Portable Gray Map)** image compressor and decompressor. The program allows users to compress grayscale images in the PGM format with a customizable compression degree. It also includes functionality to decompress the compressed files back to their original PGM format.  

## Features  
- **PGM Image Compression**: Users can input the desired compression degree to control the tradeoff between file size and image quality.  
- **PGM Image Decompression**: Restores compressed PGM images to their original format or as close as possible.  
- **Lossy Compression**: Implements basic lossy compression techniques to reduce file size.  
- **Efficient File Handling**: Optimized for performance and file size reduction.  

## How It Works  
1. **Compression**:  
   - Users provide a PGM image file and specify a compression degree (e.g., low, medium, or high compression).  
   - The program reduces the image's file size using a lossy method while maintaining a balance between quality and compression.  

2. **Decompression**:  
   - The decompressor takes the compressed file and reconstructs the original PGM image as closely as possible.  

## Applications  
- Reducing the size of large grayscale images for storage or transmission.  
- Understanding fundamental image processing and compression concepts in C++.  
