#ifndef DECODE_H
#define DECODE_H
#include <stdio.h>
#include<string.h>

#include "types.h" // Contains user defined types

/*
 * Structure to store information required for
 * encoding secret file to source Image
 * Info about output and intermediate data is 
 * also stored
 */

typedef struct _DecodeInfo
{
    char *src_image_fname;
    FILE *fptr_src_image;
    uint image_capacity;

    char secret_fname[100];
    FILE *fptr_secret;

    char extn_secret_file[5];
    int extn_size;

    char secret_data[100];
    long size_secret_file;

    

} decodeInfo;

/* Encoding function prototype */

/* Read and validate Encode args from argv */
Status read_and_validate_decode_args(char *argv[], decodeInfo *decInfo);

/* Perform the encoding */
Status do_decoding(decodeInfo *decInfo);

/* Get File pointers for i/p and o/p files */
Status open_files_decode(decodeInfo *decInfo);

/* check capacity */
Status check_capacity_decode(decodeInfo *decInfo);

/* Get image size */
uint get_image_size_for_bmp(FILE *fptr_image);

/* Get file size */
uint get_file_size(FILE *fptr);

/* Copy bmp image header */
Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image);

/* Store Magic String */
Status decode_magic_string(const char *magic_string, decodeInfo *decInfo);

/*Encode extension size*/
Status decode_secret_file_extn_size(decodeInfo *decInfo);

/* Encode secret file extenstion */
Status decode_secret_file_extn(const char *file_extn, decodeInfo *decInfo);

/* Encode secret file size */
Status decode_secret_file_size(decodeInfo *decInfo);

/* Encode secret file data*/
Status decode_secret_file_data(decodeInfo *decInfo);

/* Encode a byte into LSB of image data array */
Status decode_byte_to_lsb(char *data, char *image_buffer);

// Encode a size to lsb
Status decode_size_to_lsb(int size, char *imageBuffer);

/* Copy remaining image bytes from src to stego image after encoding */
Status copy_remaining_img_data_decode(FILE *fptr_src, FILE *fptr_dest);

#endif
