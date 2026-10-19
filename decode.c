#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<unistd.h>


#include "types.h"
#include "common.h"
#include "decode.h"


/* Read and validate command-line arguments for decoding */
Status read_and_validate_decode_args(char *argv[], decodeInfo *decInfo)
{
    // Check whether source image file is provided
    if (argv[2] == NULL)
    {
        printf("Error : Invalid arguments for decoding\n");
        return e_failure;
    }

    // Store source image file name
    decInfo->src_image_fname = argv[2];

    // Store secret file name if required
    // decInfo->secret_fname = argv[3];

    // Return success
    return e_success;
}


/* Perform the complete decoding process */
Status do_decoding(decodeInfo *decInfo)
{
    // Open source stego image
    if (open_files_decode(decInfo) == e_failure)
    {
        printf("Error : Failed to open files\n");
        return e_failure;
    }

    // Decode and verify magic string
    if (decode_magic_string(MAGIC_STRING, decInfo) == e_failure)
    {
        printf("Error : Magic string doesn't match\n");
        return e_failure;
    }

    // Decode secret file extension size
    if (decode_secret_file_extn_size(decInfo) == e_failure)
    {
        printf("Error : Failed to decode extension size\n");
        return e_failure;
    }

    // Decode secret file extension
    if (decode_secret_file_extn(decInfo->extn_secret_file, decInfo) == e_failure)
    {
        printf("Error : Failed to decode extension\n");
        return e_failure;
    }

    // Decode secret file size
    if (decode_secret_file_size(decInfo) == e_failure)
    {
        printf("Error : Failed to decode secret file size\n");
        return e_failure;
    }

    // Decode actual secret file data
    if (decode_secret_file_data(decInfo) == e_failure)
    {
        printf("Error : Failed to decode secret data\n");
        return e_failure;
    }

    // Display successful decoding message
    // Display decoded extension size
    printf("Decoded extension size = %d\n",decInfo->extn_size);
    
    printf("Decode file extension =%s\n",decInfo->extn_secret_file);
    
    for(int i=0;i*i<100;i=i+2)
    {
        printf("Loading[");
        for(int dash=0;dash<i;dash++)
        {
            printf(".");
        }
        printf("]%d%%\r",i);
        sleep(1);
        fflush(stdout);
    }
    printf("INFO : Decoding completed successfully \n");

    // Close source image file
    fclose(decInfo->fptr_src_image);

    // Close secret file
    fclose(decInfo->fptr_secret);

    // Return success
    return e_success;
}


/* Open source stego image and output secret file */
Status open_files_decode(decodeInfo *decInfo)
{
    // Open source stego image in read-binary mode
    decInfo->fptr_src_image = fopen(decInfo->src_image_fname, "rb");

    // Check whether source image was opened successfully
    if (decInfo->fptr_src_image == NULL)
    {
        printf("Error : Unable to open source image\n");
        return e_failure;
    }

    

    // Return success
    return e_success;
}


/* Decode magic string */
Status decode_magic_string(const char *magic_string, decodeInfo *decInfo)
{
    // Buffer to store 8 bytes of image data
    char buffer[8];

    // Variable to store one decoded character
    char decoded_char;

    // Skip the 54-byte BMP header
    fseek(decInfo->fptr_src_image, 54, SEEK_SET);

    // Decode each character of magic string
    for (int i = 0; i < strlen(magic_string); i++)
    {
        // Read 8 bytes from stego image
        if (fread(buffer, 1, 8, decInfo->fptr_src_image) != 8)
        {
            return e_failure;
        }

        // Decode one character from LSBs
        decode_byte_to_lsb(&decoded_char, buffer);

        // Compare decoded character with magic string
        if (decoded_char != magic_string[i])
        {
            return e_failure;
        }
    }

    // Magic string matched successfully
    return e_success;
}


/* Decode extension size - 32 bits */
Status decode_secret_file_extn_size(decodeInfo *decInfo)
{
    // Buffer to store 32 bytes of image data
    char buffer[32];

    // Variable to store decoded extension size
    int extn_size = 0;

    // Read 32 bytes from stego image
    if (fread(buffer, 1, 32, decInfo->fptr_src_image) != 32)
    {
        return e_failure;
    }

    // Extract LSB from each image byte
    for (int i = 0; i < 32; i++)
    {
        // Store each bit at its corresponding position
        extn_size |= (buffer[i] & 1) << i;
    }

    // Store decoded extension size
    decInfo->extn_size = extn_size;

    // Return success
    return e_success;
}


/* Decode secret file extension */
Status decode_secret_file_extn(const char *file_extn, decodeInfo *decInfo)
{
    // Buffer to store 8 bytes of image data
    char buffer[8];

    // Variable to store one decoded character
    char decoded_char;

    

    

    // Check whether extension size is valid
    if (decInfo->extn_size <= 0 || decInfo->extn_size >= sizeof(decInfo->extn_secret_file))
    {
        return e_failure;
    }

    // Decode each character of file extension
    for (int i = 0; i < decInfo->extn_size; i++)
    {
        // Read 8 bytes from stego image
        if (fread(buffer, 1, 8, decInfo->fptr_src_image) != 8)
        {
            return e_failure;
        }

        // Decode one character from 8 LSBs
        decode_byte_to_lsb(&decoded_char, buffer);

        // Store decoded character in extension array
        decInfo->extn_secret_file[i] = decoded_char;
    }

    // Add null character to terminate string
    decInfo->extn_secret_file[decInfo->extn_size] = '\0';

    // Create output secret file name
    sprintf(decInfo->secret_fname,"decode_secret_file%s",decInfo->extn_secret_file);

    // Create secret file in write-binary mode
    decInfo->fptr_secret = fopen(decInfo->secret_fname, "wb");

    // Check whether secret file was created successfully
    if (decInfo->fptr_secret == NULL)
    {
        printf("Error : Unable to create secret file\n");

        // Close source image before returning failure
        fclose(decInfo->fptr_src_image);

        return e_failure;
    }

    // Return success
    return e_success;
}


/* Decode secret file size - 32 bits */
Status decode_secret_file_size(decodeInfo *decInfo)
{
    // Buffer to store 32 bytes of image data
    char buffer[32];

    // Variable to store decoded file size
    long size = 0;

    // Read 32 bytes from stego image
    if (fread(buffer, 1, 32, decInfo->fptr_src_image) != 32)
    {
        return e_failure;
    }

    // Extract LSB from each image byte
    for (int i = 0; i < 32; i++)
    {
        // Store each decoded bit at its corresponding position
        size |= ((long)(buffer[i] & 1)) << i;
    }

    // Store decoded secret file size
    decInfo->size_secret_file = size;

    // Display decoded file size
   

    // Return success
    return e_success;
}


/* Decode secret file data */
Status decode_secret_file_data(decodeInfo *decInfo)
{
    // Buffer to store 8 bytes of image data
    char buffer[8];

    // Variable to store one decoded character
    char decoded_char;

    // Repeat until all secret file bytes are decoded
    for (long i = 0; i < decInfo->size_secret_file; i++)
    {
        // Read 8 bytes from stego image
        if (fread(buffer, 1, 8, decInfo->fptr_src_image) != 8)
        {
            return e_failure;
        }

        // Decode one character from 8 LSBs
        decode_byte_to_lsb(&decoded_char, buffer);

        // Write decoded character into secret file
        fwrite(&decoded_char, 1, 1, decInfo->fptr_secret);
    }

    // Return success
    return e_success;
}


/* Decode one byte from 8 image bytes */
Status decode_byte_to_lsb(char *data, char *image_buffer)
{
    // Initialize decoded data to zero
    *data = 0;

    // Process 8 image bytes
    for (int i = 0; i < 8; i++)
    {
        // Extract LSB from image byte
        // and place it in corresponding bit position
        *data |= (image_buffer[i] & 1) << i;
    }

    // Return success
    return e_success;
}