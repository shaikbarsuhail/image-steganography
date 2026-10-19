# Image Steganography in C

A C project for hiding a secret file inside a BMP image and extracting it later.

## Project files

Add your complete source and header files here. The encoder files and `types.h` are needed to build the project.

## Build

Compile the project with GCC:

Pass this input arguments  in commond line to get output 
gcc *.c
./a.out -e beautiful.bmp secret.txt
for output image
./a.out -d stego.bmp
