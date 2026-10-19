#include <stdio.h>
#include<unistd.h>
#include "encode.h"
#include "types.h"
#include"decode.h"
/*
/*
Name: SHAIK BAR SUHAIL 
Batch Number: 26012B
Roll Number: 26012B_119
Starting Date:04/09/2026
Ending Date:29/09/2026


for encoding :
Input : ./a.out -e beautiful.bmp secret.txt

Encoding started ...
Successfully File Opend
width = 1024
height = 768
INFO : Capacity checked successfully
INFO : Copied header data successfully
Extension: .txt
Extension size: 4
INFO : Copied remaining data successfully
INFO : Copied image data successfully
INFO : Encoding completed successfully 

for decoding :
Input :./a.out stego.bmp

Decoding started ...
Decoded extension size = 4
Decode file extension =.txt
INFO : Decoding completed successfully 

*/


OperationType check_operation_type(char *);

int main(int argc, char *argv[]) // .aout -e <source_file> <secret_data_file> <optional>
{
    //step 1 -> check check_operation_type(argv[1]) is returning e_encode or not
        // yes -> EncodeInfo encInfo;
            // check read_and_validate_encode_args(argv, &encInfo) is returning e_success or e_failure
                // failure -> print error msg and stop
                // success -> print success msg 
                            // check do_encoding(&encInfo) is returning e_success or e_failure
                                    // failure -> print error msg and stop
                                    // success -> print success msg and stop

    // Check whether the user has passed at least one command-line argument.
    // argv[1] should contain the operation type (-e or -d).
    if(argc <= 1)
    {
        printf("pass Arguments \n Type -e or -d <source_file> <secret_data_file>\n");

        // Stop the program because required arguments are not provided.
        return e_failure;
    }

    /* Check whether the user selected encoding operation */
    if(check_operation_type(argv[1]) == e_encode)
    {
        // Create a structure to store all encoding-related information.
        EncodeInfo encInfo;

        // Read and validate the command-line arguments for encoding.
        if(read_and_validate_encode_args(argv, &encInfo) == e_failure)
        {
            // Arguments are invalid, so display an error message.
            printf("Error : Invalid aruguments for encoding\n");

            // Stop encoding because of invalid arguments.
            return e_failure;
        }

        // Start the encoding process.
        printf("Encoding started ...\n");

        // Perform the actual encoding operation.
        if(do_encoding(&encInfo) == e_failure)
        {
            // Encoding failed, so display an error message.
            printf("Error : encoding failed !\n");

            // Stop the program because encoding failed.
            return e_failure;
        }

        // Encoding completed successfully.
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
        printf("INFO : Encoding completed successfully \n");

        // Return from main and terminate the program.
        return e_failure;
    }


    /* Check whether the user selected decoding operation */
    else if(check_operation_type(argv[1]) == e_decode)
    {
        // Create a structure to store all decoding-related information.
        decodeInfo decInfo;

        // Read and validate the command-line arguments for decoding.
        if(read_and_validate_decode_args(argv, &decInfo) == e_failure)
        {
            // Arguments are invalid, so display an error message.
            printf("Error : Invalid aruguments for decoding\n");

            // Stop the program because of invalid arguments.
            return e_failure;
        }
        else
        {
            // Start the decoding process.
            printf("Decoding started ...\n");

            // Perform the actual decoding operation.
            if(do_decoding(&decInfo) == e_failure)
            {
                // Decoding failed, so display an error message.
                printf("Error : decoding failed !\n");

                // Stop the program because decoding failed.
                return e_failure;
            }

            // Decoding completed successfully.
            return e_failure;
        }
    }


    /* If the operation is neither encoding nor decoding */
    else
    {
        // The user entered an invalid operation such as -x.
        printf("Entered wrong arguments\n");

        // Stop the program.
        return e_failure;
    }
    return 0;
   
}

OperationType check_operation_type(char *symbol)
{
    //step 1 -> check symbol is -e or not
        // yes -> return e_encode
    
    // step 2 -> check symbol is -d or not
        // yes -> return e_decode

    // return e_unsupported

    
    // Check whether the command-line argument is "-e"
    if(strcmp(symbol, "-e") == e_encode)
    {
        // "-e" means encoding operation
        return e_encode;
    }

    // Check whether the command-line argument is "-d"
    else if(strcmp(symbol, "-d") == e_encode)
    {
        // "-d" means decoding operation
        return e_decode;
    }

    // If the argument is neither "-e" nor "-d",
    // return unsupported operation
    return e_unsupported;
}
