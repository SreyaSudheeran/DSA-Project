# File Compressor and Decompressor

### Team Members:
- **Sreya Sudheeran** (612303175)
- **Shreya Shinde** (612303167)

## Project Overview
This project aims to develop a versatile and efficient file compressor and decompressor capable of handling various file formats such as text files and images. The application implements two powerful compression algorithms—**Huffman Coding** and **Lempel-Ziv-Welch (LZW)**—to reduce file sizes effectively. It provides users with a simple command-line interface to compress and decompress files with ease.

## Key Features:
- **Huffman Coding**: A lossless algorithm that minimizes file size by assigning shorter codes to more frequent characters.
- **LZW Compression**: A dictionary-based algorithm that compresses files by replacing repeating sequences with shorter codes.

## Project Objectives:
- Implement two popular compression algorithms (Huffman Coding and LZW) to efficiently reduce the size of files.
- Provide an intuitive interface for users to compress and decompress files using the command line.
- Support a variety of file formats, including text and image files.

## Scope:
This project focuses on two key algorithms:
- **Huffman Coding**: Suitable for symbol-based compression, where more frequent symbols are given shorter codes.
- **LZW Compression**: A dictionary-based algorithm that identifies recurring patterns in data and compresses them effectively.

The tool allows users to choose between these two algorithms depending on the file type and compression needs, providing both compression and decompression capabilities.

## Problem Statement:
With the rapid increase in digital content, managing large files becomes crucial in both storage and transmission. This project provides a solution by offering efficient file compression, helping users save space and speed up file transfers. It also supports multiple file types, making it a flexible tool for different use cases.

## Data Structures Used:
- **Arrays**: For storing symbol frequencies and handling bit manipulation.
- **Binary Trees**: To construct the Huffman tree for optimal encoding.
- **Dictionaries/Hash Maps**: Used in LZW for storing and referencing compressed sequences.
- **Priority Queues**: Employed in the construction of the Huffman tree to manage symbol priorities efficiently.

## Algorithms:
### 1. **Huffman Coding**:
A lossless data compression algorithm that assigns variable-length codes to characters based on their frequencies. Characters with higher frequencies are assigned shorter codes, leading to a smaller file size.

### 2. **LZW Compression (Lempel-Ziv-Welch)**:
A dictionary-based compression algorithm that replaces repeating sequences of data with shorter codes. It builds a dictionary of input sequences and encodes data using the dictionary entries, making it suitable for use with text and image formats like GIF and TIFF.

## Program Workflow:
### Compression:
- `./program -c1 <file> <compressed-file-name>`: Compresses a file using **Huffman Coding**.
- `./program -c2 <file> <compressed-file-name>`: Compresses a file using **LZW Compression**.

### Decompression:
- `./program -uc1 <compressed-file> <uncompressed-file>`: Decompresses a file compressed with **Huffman Coding**.
- `./program -uc2 <compressed-file> <uncompressed-file>`: Decompresses a file compressed with **LZW Compression**.

## Real-World Applications:
- **Data Storage Optimization**: Reduce the size of files to save storage space while maintaining data integrity.
- **Image and Media Compression**: Efficient storage of image formats like GIF, ensuring high-quality media without excessive file sizes.
- **Network Data Transfer**: Speed up file transfers by reducing the file size, making it ideal for large files sent over the internet.

## Expected Outcomes:
- Significantly reduced file sizes, improving storage and transmission efficiency.
- Lossless compression and decompression without data corruption.
- Flexibility to handle different file types, including text and images.

## Future Enhancements:
- **Additional Algorithms**: Integrate more compression algorithms (e.g., Deflate, Bzip2) for broader file format support.
- **GUI Integration**: Add a graphical user interface (GUI) to simplify user interaction.
- **Batch Compression**: Extend the functionality to compress/decompress multiple files simultaneously.

## Conclusion:
This project provides a comprehensive solution for file size reduction using two well-known algorithms—Huffman Coding and LZW Compression. By implementing these algorithms, the tool enhances file storage, transmission efficiency, and supports a wide range of file types, making it a valuable utility for both individuals and organizations dealing with large files.

