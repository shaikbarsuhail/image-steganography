#include <stdio.h>
#include<stdlib.h>
#include "encode.h"
#include "types.h"
#include"common.h"




/* Function Definitions */

/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */
uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    // Seek to 18th byte
    fseek(fptr_image, 18, SEEK_SET);

    // Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_image);
    printf("width = %u\n", width);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);
    printf("height = %u\n", height);

    // Return image capacity
    return width * height * 3;
}

uint get_file_size(FILE *fptr)
{
    // Find the size of secret file data
    // return secret file size
    
    fseek(fptr,0,SEEK_END);
    return ftell(fptr);
}

/*
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */

Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo)
{
    // step 1 -> check argv[2] is having .bmp or not
        //yes -> store the file name into encInfo -> src_image_fname = argv[2]
        //no - > return e_failure

    // step 2 -> check argv[3] is having extn there or not
        // yes - > store the file name into encInfo -> secret_fname = argv[3]
        // no -> return e_failure
    
    // step 3 -> check argv[4] is NULL or not
        //yes -> check argv[4] is having .bmp or not
                //yes -> store the file name into encInfo -> stego_image_fname = argv[4]
                //no - > return e_failure
        
        //no -> store default name the encInfo -> stego_image_fname = "stego.bmp"


    // return e_success;
    if(argv[1] == NULL || argv[2] == NULL || argv[3] == NULL)
    {
        
        return e_failure;
    }


    // Check whether the source image has a .bmp extension
    if(strcmp(strstr(argv[2], "."), ".bmp") != e_failure)
    {
        // If the extension is not .bmp, return failure
        return e_failure;
    }


    // Check whether the secret file has a .txt extension
    if(strcmp(strstr(argv[3], "."), ".txt") != e_failure)
    {
        // If the extension is not .txt, return failure
        return e_failure;
    }


    // Assign the source image filename
    encInfo->src_image_fname = argv[2];


    // Assign the secret file filename
    encInfo->secret_fname = argv[3];


    // Find the extension of the secret file using strstr()
    // and copy the extension into extn_secret_file
    strcpy(encInfo->extn_secret_file, strstr(argv[3], "."));


    // Check whether the user has provided a stego image filename
    if(argv[4] != NULL)
    {
        // If provided, assign the user-given filename
        // as the stego image filename
        encInfo->stego_image_fname = argv[4];

        // Arguments are valid, so return success
        return e_success;
    }
    else
    {
        // If no stego image filename is provided,
        // use "stego.bmp" as the default filename
        encInfo->stego_image_fname = "stego.bmp";
    }


    // All arguments are valid
    return e_success;
}

Status open_files(EncodeInfo *encInfo)
{
    // Src Image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "rb");
    // Do Error handling
    if (encInfo->fptr_src_image == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);

        return e_failure;
    }

    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
    // Do Error handling
    if (encInfo->fptr_secret == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);

        return e_failure;
    }

    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "wb");
    // Do Error handling
    if (encInfo->fptr_stego_image == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);

        return e_failure;
    }

    // No failure return e_success
    printf("Successfully File Opend\n");
    return e_success;
}

Status check_capacity(EncodeInfo *encInfo)
{
    //step 1 -> encInfo -> image_capacity = call get_image_size_for_bmp(encoInfo -> fptr_src_image)

    //step 2 -> encInfo -> size_secret_file = call get_file_size(encInfo -> fptr_secret)

    //step 3 -> check encInfo -> image_capacity > 16 + 32 + (strlen(encInfo -> extn_secret_file) * 8) + 32 + (encInfo -> size_secret_file * 8)
                //t -> return e_success;
                //f -> return e_failure



    /*16 bits       → Magic string
    32 bits       → Extension size
    N × 8 bits    → Extension (.txt)
    32 bits       → Secret file size
    File × 8 bits → Secret file data   
    */         
    // Get the total image capacity of the source BMP image
    // The capacity is calculated in bytes available for storing data
    encInfo->image_capacity = get_image_size_for_bmp(encInfo->fptr_src_image);


    // Get the size of the secret file in bytes
    encInfo->size_secret_file = get_file_size(encInfo->fptr_secret);


    // Check whether the BMP image has enough capacity
    // to store all the required secret information
    if(encInfo->image_capacity >
    16 +                         // Magic string size
    32 +                         // Size of secret file extension
    (strlen(encInfo->extn_secret_file) * 8) +  // Secret file extension
    32 +                         // Size of secret file
    (encInfo->size_secret_file * 8))           // Secret file data


    {
        // Image has enough capacity to store the secret data
        printf("INFO : Capacity checked successfully\n");

        // Return success
        return e_success;
    }
    else
    {
        // Image does not have enough capacity
        return e_failure;
    }
}

Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)
{
    // step 1 -> rewind source file pointer

    // step 2 -> read 54 bytes from source file

    // step 3 -> write 54 bytes to dest file

    // return e_success
    
    // Move the file pointer to the beginning of the source image
    rewind(fptr_src_image);


    // Create a buffer to temporarily store the BMP header
    // Standard BMP header size used here is 54 bytes
    char buffer[54];


    // Read 54 bytes (BMP header) from the source image
    if(fread(buffer, 54, 1, fptr_src_image) == 1)
    {
        // Write the 54-byte header into the destination/stego image
        fwrite(buffer, 54, 1, fptr_dest_image);

        // Display success message
        printf("INFO : Copied header data successfully\n");

        // Header copied successfully
        return e_success;
    }


// If fread() fails to read the BMP header,
// return failure
return e_failure;
    

    return e_failure;
    
}
Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo)
{
    // char buffer[8];

    //step 1 -> read 8 bytes from source file

    //step 2 -> call encode_byte_to_lsb(magic_string[i], buffer)

    //step 3 -> write the buffer into dest file

    // repeat this for strlen(magic_string) times from step 1

   // Create a buffer to store 8 bytes of image data
    char buffer[8];


    // Loop through each character of the magic string
    for(int i = 0; i < strlen(magic_string); i++)
    {
        // Read 8 bytes from the source image
        // 8 image bytes are required to hide 1 character
        if(fread(buffer, 1, 8, encInfo->fptr_src_image) != 8)
        {
            // If 8 bytes could not be read, return failure
            return e_failure;
        }


        // Encode one character of the magic string
        // into the LSBs of the 8 image bytes
        if(encode_byte_to_lsb(magic_string[i], buffer) == e_success)
        {
            // Write the modified 8 bytes into the stego image
            fwrite(buffer, 1, 8, encInfo->fptr_stego_image);
        }
    }


    // Magic string was successfully encoded
    return e_success;
}
Status encode_secret_file_extn_size(int size, EncodeInfo *encInfo)
{
    // char buffer[32];

    //step 1 -> read 32 bytes from source file

    //step 2 -> call encode_size_to_lsb(size, buffer)

    //step 3 -> write the buffer into dest file

        // Print the secret file extension
    // Example: .txt
    printf("Extension: %s\n", encInfo->extn_secret_file);


    // Print the size of the secret file extension
    // strlen(".txt") = 4
    printf("Extension size: %zu\n", strlen(encInfo->extn_secret_file));


    // Create a buffer to store 32 bytes of image data
    // 32 image bytes are required to store a 32-bit integer
    char buffer[32];


    // Read 32 bytes from the source image
    if(fread(buffer, 32, 1, encInfo->fptr_src_image) != 1)
    {
        // If 32 bytes could not be read, return failure
        return e_failure;
    }


    // Encode the extension size into the LSBs of the 32 image bytes
    // One bit is stored in each image byte
    if(encode_size_to_lsb(size, buffer) != e_success)
    {
        // If encoding the size fails, return failure
        return e_failure;
    }


    // Write the modified 32 bytes into the stego image
    if(fwrite(buffer, 32, 1, encInfo->fptr_stego_image) != 1)
    {
        // If writing fails, return failure
        return e_failure;
    }


    // Extension size was successfully encoded
    return e_success;
}

Status encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo)
{
    // char buffer[8];

    //step 1 -> read 8 bytes from source file

    //step 2 -> call encode_byte_to_lsb(file_extn[i], buffer)

    //step 3 -> write the buffer into dest file

    // repeat this for strlen(file_extn) times from step 1   

   // Create a buffer to store 8 bytes of image data
    char buffer[8];


    // Loop through each character of the file extension
    // Example: ".txt" contains 4 characters
    for(int i = 0; i < strlen(file_extn); i++)
    {
        // Read 8 bytes from the source image
        // 8 image bytes are required to store 1 character (8 bits)
        if(fread(buffer, 1, 8, encInfo->fptr_src_image) != 8)
        {
            // If 8 bytes could not be read, return failure
            return e_failure;
        }


        // Encode one character of the file extension
        // into the LSBs of the 8 image bytes
        encode_byte_to_lsb(file_extn[i], buffer);


        // Write the modified 8 bytes into the stego image
        if(fwrite(buffer, 1, 8, encInfo->fptr_stego_image) != 8)
        {
            // If writing 8 bytes fails, return failure
            return e_failure;
        }
    }


    // File extension was successfully encoded
    return e_success;     

}

Status encode_secret_file_size(long file_size, EncodeInfo *encInfo)
{
    // char buffer[32];

    //step 1 -> read 32 bytes from source file

    //step 2 -> call encode_size_to_lsb(file_size, buffer)

    //step 3 -> write the buffer into dest file
   // Print the size of the secret file
    //printf("Encoding secret file size = %ld\n", file_size);

    // Create a buffer to store 32 bytes of image data
    char buffer[32];

    // Read 32 bytes from the source image
    if(fread(buffer, 1, 32, encInfo->fptr_src_image) != 32)
    {
        // If 32 bytes are not successfully read, return failure
        return e_failure;
    }

    // Encode the secret file size into the LSBs of the 32 bytes
    encode_size_to_lsb(file_size, buffer);

    // Write the modified 32 bytes into the stego image
    if(fwrite(buffer, 1, 32, encInfo->fptr_stego_image) != 32)
    {
        // If 32 bytes are not successfully written, return failure
        return e_failure;
    }

    // Encoding completed successfully
    return e_success; 
}

Status encode_secret_file_data(EncodeInfo *encInfo)
{
    //step 1 -> read secret data from file and store it into encInfo -> secret_data

    // char buffer[8];

    //step 2 -> read 8 bytes from source file

    //step 3 -> call encode_byte_to_lsb(encInfo -> secret_data[i], buffer)

    //step 4 -> write the buffer into dest file

    // repeat this for strlen(encInfo -> size_secret_file) times from step 2

    // Create a buffer to store 8 bytes of image data
    char buffer[8];

    // Variable to store one character read from the secret file
    char data;


    // Move the secret file pointer to the beginning
    // so that we can read the secret file from the first byte
    rewind(encInfo->fptr_secret);


    // Loop through every byte/character of the secret file
    for(long i = 0; i < encInfo->size_secret_file; i++)
    {
        // Read one byte (one character) from the secret file
        if(fread(&data, 1, 1, encInfo->fptr_secret) != 1)
        {
            // If one byte could not be read, return failure
            return e_failure;
        }


        // Read 8 bytes from the source image
        // These 8 image bytes are used to store one secret character
        if(fread(buffer, 1, 8, encInfo->fptr_src_image) != 8)
        {
            // If 8 image bytes could not be read, return failure
            return e_failure;
        }


        // Encode the secret character into the LSBs
        // of the 8 image bytes
        if(encode_byte_to_lsb(data, buffer) != e_success)
        {
            // If encoding fails, return failure
            return e_failure;
        }


        // Write the modified 8 image bytes into the stego image
        if(fwrite(buffer, 1, 8, encInfo->fptr_stego_image) != 8)
        {
            // If writing fails, return failure
            return e_failure;
        }
    }


    // All secret file data was successfully encoded
    return e_success;
}

Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest)
{
    // write logic for copy remaining data

    // return e_success

   // Variable to store one character (1 byte) at a time
    char ch;

    // Read one byte from source file until the end of file
    while(fread(&ch, 1, 1, fptr_src) == 1)
    {
        // Write the read byte into the destination file
        fwrite(&ch, 1, 1, fptr_dest);
    }

    // Display success message after copying all remaining data
    printf("INFO : Copied remaining data successfully\n");

    // Return success
    return e_success;
}

Status encode_byte_to_lsb(char data, char *image_buffer)
{
    // write logic to encode the char

    //repeat for 8 bits

    //clear LSB from data

    // put data bit into image LSB

    //shift data right 

   // Process 8 bytes of image data to store 8 bits of data
    for(int i = 0; i < 8; i++)
    {
        // Clear the LSB (Least Significant Bit) of the image byte
        image_buffer[i] &= 0xFE;

        // Get the LSB of data and store it in the image byte's LSB
        image_buffer[i] |= (data & 1);

        // Shift data right by 1 bit to get the next bit
        data >>= 1;
    }

    // Return success after encoding all 8 bits
    return e_success;
    
}

Status encode_size_to_lsb(long size, char *imageBuffer)
{
    // write logic to encode the int

    // Process 32 bytes of image data to store 32 bits of file size
    for(int i = 0; i < 32; i++)
    {
        // Clear the LSB of the current image byte
        imageBuffer[i] &= 0xFE;

        // Store the current LSB of size into the image byte's LSB
        imageBuffer[i] |= (size & 1);

        // Shift size right by 1 bit to get the next bit
        size >>= 1;
    }

    // Return success after encoding all 32 bits
    return e_success;
}

Status do_encoding(EncodeInfo *encInfo)
{
    //step 1 -> check open_files(encInfo) is returning e_success or not
            //no -> print error return e_failure
            //yes -> print success and goto next step

    //step 2 -> check check_capacity(encInfo) is returning e_success or not
            //no -> print error return e_failure
            //yes -> print success and goto next step

    //step 3 -> call copy_bmp_header(encInfo -> fptr_src_image, encInfo -> fptr_stego_image)is returning e_success or not
            //no -> print error return e_failure
            //yes -> print success and goto next step

    //step 4 -> call encode_magic_string(MAGIC_STRING, encInfo)

    //step 5 -> call encode_secret_file_extn_size(strlen(encInfo -> extn_secret_file), encInfo)

    //step 6 -> call encode_secret_file_extn(encInfo -> extn_secret_file, encInfo)

    //step 7 -> call encode_secret_file_size(encInfo ->  size_secret_file, encInfo)

    //step 8 -> call encode_secret_file_data(encInfo)

    //step 9 -> copy_remaining_img_data(encInfo -> fptr_src_image, encInfo -> fptr_stego_image)is returning e_success or not
            //no -> print error return e_failure
            //yes -> print success


    // return e_success


    // Open the source image, secret file and stego image
    if(open_files(encInfo) == e_failure)
    {
        // If any file fails to open, display an error message
        fprintf(stderr, "Error : Failed to open file\n");

        // Stop the encoding process
        return e_failure;
    }


    // Check whether the source BMP image has enough capacity
    // to store the secret data
    if(check_capacity(encInfo) == e_failure)
    {
        // If the image does not have enough space, display an error
        fprintf(stderr, "Failed to check capacity of the frame\n");

        // Stop the encoding process
        return e_failure;
    }


    // Copy the BMP header from the source image
    // to the stego image
    if(copy_bmp_header(encInfo->fptr_src_image,
                    encInfo->fptr_stego_image) == e_failure)
    {
        // If copying the BMP header fails, display an error
        fprintf(stderr, "Failed to copy BMP header\n");

        // Stop the encoding process
        return e_failure;
    }


    // Encode the magic string into the stego image
    // The magic string is used later during decoding
    // to identify whether the image contains hidden data
    if(encode_magic_string(MAGIC_STRING, encInfo) == e_failure)
    {
        // If encoding the magic string fails, display an error
        fprintf(stderr, "Failed to encode magic string\n");

        // Stop the encoding process
        return e_failure;
    }


    // Encode the size of the secret file extension
    // Example: ".txt" has a size of 4
    if(encode_secret_file_extn_size(strlen(encInfo->extn_secret_file),encInfo) == e_failure)
    {
        // If encoding the extension size fails, display an error
        fprintf(stderr, "Failed to encode secret file extension size\n");

        // Stop the encoding process
        return e_failure;
    }


    // Encode the secret file extension into the stego image
    // Example: ".txt"
    if(encode_secret_file_extn(encInfo->extn_secret_file,
                            encInfo) == e_failure)
    {
        // If encoding the extension fails, display an error
        fprintf(stderr, "Failed to encode secret file extension\n");

        // Stop the encoding process
        return e_failure;
    }


    // Encode the size of the secret file
    // This size will be used during decoding
    // to know how many bytes of secret data to extract
    if(encode_secret_file_size(encInfo->size_secret_file,
                            encInfo) == e_failure)
    {
        // If encoding the secret file size fails, display an error
        fprintf(stderr, "Failed to encode secret file size\n");

        // Stop the encoding process
        return e_failure;
    }


    // Encode the actual secret file data into the stego image
    if(encode_secret_file_data(encInfo) == e_failure)
    {
        // If encoding the secret data fails, display an error
        fprintf(stderr, "Failed to encode secret file data\n");

        // Stop the encoding process
        return e_failure;
    }


    // Copy the remaining image data from the source image
    // to the stego image without modifying it
    if(copy_remaining_img_data(encInfo->fptr_src_image,
                            encInfo->fptr_stego_image) == e_failure)
    {
        // If copying the remaining image data fails, display an error
        fprintf(stderr, "Failed to copy image data\n");

        // Stop the encoding process
        return e_failure;
    }
    else
    {
        // All encoding operations completed successfully
        printf("INFO : Copied image data successfully\n");

        // Return success
        return e_success;
    }
     

}