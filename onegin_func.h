#ifndef ONEGIN_FUNC_H
#define ONEGIN_FUNC_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

#define CHECKUS(condition, ReturnNum)                                                     \
        do                                                                                \
        {                                                                                 \
            if (!(condition))                                                             \
            {                                                                             \
                printf("Error: %s, file %s, line %i\n", #condition, __FILE__, __LINE__);  \
                return ReturnNum;                                                         \
            }                                                                             \
        } while (0)


char* ReadFile(FILE* text_input);

int CountSimbols(FILE* text_input);

int Countlines(char* buffer, int simbol_amount);

int SplitToLines(char** indexPtr, char* buffer, int simbol_amount);

void OutputResult(FILE* result_file, char** indexPtr, int line_amount);


#endif
