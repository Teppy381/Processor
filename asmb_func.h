#ifndef ASMB_FUNC_H
#define ASMB_FUNC_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


#define CHECKUS(condition, ReturnNum)                                                 \
    do                                                                                \
    {                                                                                 \
        if (!(condition))                                                             \
        {                                                                             \
            printf("Error: %s, file %s, line %i\n", #condition, __FILE__, __LINE__);  \
            return ReturnNum;                                                         \
        }                                                                             \
    } while (0)




int CommandNum(const char* str);

int IsNumber(char* str, int symbol_amount);

int BytewiseDouble(unsigned char* str, double n);

int PrintListing(FILE* listing_file, unsigned char* str, int n);

int GetArg(unsigned char* command_data_p, int* result_pointer_p, char** indexPtr, int* line_p, FILE* listing_file);

int RegNum(const char* str);

#endif
