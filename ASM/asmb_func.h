#ifndef ASMB_FUNC_H
#define ASMB_FUNC_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "colors.h"

#define CHECKUS(condition, ReturnNum)                                                 \
    do                                                                                \
    {                                                                                 \
        if (!(condition))                                                             \
        {                                                                             \
            printf("Error: %s, file %s, line %i\n", #condition, __FILE__, __LINE__);  \
            return ReturnNum;                                                         \
        }                                                                             \
    } while (0)


typedef struct label_struct
{
    char* name = NULL;
    int adress = 0;
} label_struct;

int CommandNum(const char* str);

int AssembleAll(char** indexPtr, unsigned char* result, FILE* listing_file, label_struct* label_list, int* label_counter_p, int line_amount, int passage, int* byte_amount);

int IsLabel(const char* str);

int FindLabel(const char* str, const label_struct* label_list, const int label_amount);

int IsAllowedDigitsInt(const char* str);

int IsAllowedDigitsDouble(const char* str);

int PrintListing(FILE* listing_file, unsigned char* str, int n);

int GetArg(unsigned char* command_data_p, int* result_pointer_p, char** indexPtr, int* line_p, FILE* listing_file);

int RegNum(const char* str);

#endif
