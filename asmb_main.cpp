#include "onegin_func.h"
#include <string.h>
#include <ctype.h>

/*
push number (double) = 00 | 09 90 38 91 38 29 83 29
push rbx = 20 | 01
push [10] = 40 | 0a
push [rcx] = 60 | 02

pop = 01 |
pop rbx = 21 | 01
pop [10] = 41 | 0a
pop [rdx] = 61 | 03

add sub mul div

inp ?
out

dump
jump
hlt
*/
/*
typedef struct list_struct
{
    const char** list;
    int elem_amount = 0;
} list_struct;

list_struct cd = {}; */


const char* command_list[] =
{
    "push", "pop", "add", "sub", "mul", "div", "out", "hlt"
};


const char* register_list[] =
{
    "rax", "rbx", "rcx", "rdx"
};




int IsNumber(char* str, int symbol_amount)
{
    CHECKUS(str != NULL, 1);

    for (int i = 0; i < symbol_amount; i++)
    {
        if (!(isdigit(str[i])))
        {
            return 0;
        }
    }
    return 1;
}


int CommandNum(char* str) // TODO make function that combines CommandNum RegisterNum RamRegisterNum using structures (char* str, int elem_amount)
{
    CHECKUS(str != NULL, 1);
    int command_amount = sizeof(command_list) / sizeof(*command_list);
    for (int i = 0; i < command_amount; i++)
    {
        if (strcmp(str, command_list[i]) == 0)
        {
            return i;
        }
    }
    return -1;
}

int RegisterNum(char* str)
{
    CHECKUS(str != NULL, 1);
    int register_amount = sizeof(register_list) / sizeof(*register_list);
    for (int i = 0; i < register_amount; i++)
    {
        if (strcmp(str, register_list[i]) == 0)
        {
            return i;
        }
    }
    return -1;
}

int RamRegisterNum(char* str)
{
    CHECKUS(str != NULL, 1);
    int register_amount = sizeof(register_list) / sizeof(*register_list);
    for (int i = 0; i < register_amount; i++)
    {
        if (strncmp(str, register_list[i], 3) == 0)
        {
            return i;
        }
    }
    return -1;
}

int BytewiseDouble(unsigned char* str, double n)
{
    CHECKUS(str != NULL, 1);
    unsigned char* n_p = (unsigned char*) &n;
    for (int i = 0; i < 8; i++)
    {
        str[i] = n_p[i];
    }
    return 0;
}

int PrintListing(FILE* listing_file, unsigned char* str, int n)
{
    CHECKUS(str != NULL, 1);
    CHECKUS(n > 0, 1);

    fprintf(listing_file, "%02x | ", str[0]);

    for (int i = 1; i < n; i++)
    {
        fprintf(listing_file, "%02x ", str[i]);
    }

    fprintf(listing_file, "\n");
    return 0;
}

int main() // TODO too long function
{
    printf("\n");

    FILE* text_input = fopen("Y-prog.txt", "r"); // TODO pass file name as cmd arg
    char* buffer = ReadFile(text_input);
    int simbol_amount = CountSimbols(text_input);
    fclose(text_input);

    int line_amount = Countlines(buffer, simbol_amount);

    char** indexPtr = (char**) calloc(line_amount + 1, sizeof(char*));

    SplitToLines(indexPtr, buffer, simbol_amount);

    unsigned char* result = (unsigned char*) calloc(simbol_amount * 3, sizeof(*result));
    CHECKUS(result != NULL, 1);



    FILE* result_file = fopen("Y-compiled.txt", "w");
    CHECKUS(result_file != NULL, 1);

    OutputResult(result_file, indexPtr, line_amount);



    FILE* listing_file = fopen("Y-listing.txt", "w");
    CHECKUS(listing_file != NULL, 1);


    int line = 0;
    int n = 0, arg_str_length = 0;
    double double_num_buf = 0;
    int int_num_buf = 0;
    int result_pointer = 0;

    while (line < line_amount)
    {
        n = CommandNum(indexPtr[line]);
        if (n < 0) // error
        {
            printf("Unknown command \"%s\" (word number %i)\n", indexPtr[line], line + 1);
            free(buffer);
            free(indexPtr);
            free(result);
            return 1;
        }
        else if (n == 0) // push
        {
            arg_str_length = strlen(indexPtr[line + 1]);

            if (sscanf(indexPtr[line + 1], "%lf", &double_num_buf) == 1) // number argument (double)
            {
                result[result_pointer] = n;
                BytewiseDouble(&result[result_pointer + 1], double_num_buf);

                PrintListing(listing_file, &result[result_pointer], 9);
                result_pointer += 9;
                line += 2;
            }
            else if (RegisterNum(indexPtr[line + 1]) >= 0) // push rax
            {
                result[result_pointer] = n + 32;
                result[result_pointer + 1] = RegisterNum(indexPtr[line + 1]);
                PrintListing(listing_file, &result[result_pointer], 2);
                result_pointer += 2;
                line += 2;
            }
            else if (indexPtr[line + 1][0] == '['  &&  indexPtr[line + 1][arg_str_length - 1] == ']' // push [10] (int)
            && IsNumber(&indexPtr[line + 1][1], arg_str_length - 2) && sscanf(indexPtr[line + 1], "[%i]", &int_num_buf) == 1)
            {
                result[result_pointer] = n + 64;
                result[result_pointer + 1] = int_num_buf;
                PrintListing(listing_file, &result[result_pointer], 2);
                result_pointer += 2;
                line += 2;
            }
            else if (indexPtr[line + 1][0] == '['  &&  indexPtr[line + 1][arg_str_length - 1] == ']' // push [rax]
            && RamRegisterNum(indexPtr[line + 1] + 1) >= 0 && arg_str_length == 5)
            {
                result[result_pointer] = n + 32 + 64;
                result[result_pointer + 1] = RamRegisterNum(indexPtr[line + 1] + 1);
                PrintListing(listing_file, &result[result_pointer], 2);
                result_pointer += 2;
                line += 2;
            }
            else // error for push
            {
                printf("Wrong argument \"%s\" in command \"%s\" (word number %i)\n", indexPtr[line + 1], indexPtr[line], line + 1);
                free(buffer);
                free(indexPtr);
                free(result);
                return 1;
            }
        }
        else if (n == 1) // pop
        {
            arg_str_length = strlen(indexPtr[line + 1]);

            if (CommandNum(indexPtr[line + 1]) >= 0) // no argument
            {
                result[result_pointer] = n;
                PrintListing(listing_file, &result[result_pointer], 1);
                result_pointer += 1;
                line += 1;
            }
            else if (RegisterNum(indexPtr[line + 1]) >= 0) // rax
            {
                result[result_pointer] = n + 32;
                result[result_pointer + 1] = RegisterNum(indexPtr[line + 1]);
                PrintListing(listing_file, &result[result_pointer], 2);
                result_pointer += 2;
                line += 2;
            }
            else if (indexPtr[line + 1][0] == '['  &&  indexPtr[line + 1][arg_str_length - 1] == ']' // pop [10] (int)
            && IsNumber(&indexPtr[line + 1][1], arg_str_length - 2) && sscanf(indexPtr[line + 1], "[%i]", &int_num_buf) == 1)
            {
                result[result_pointer] = n + 64;
                result[result_pointer + 1] = int_num_buf;
                PrintListing(listing_file, &result[result_pointer], 2);
                result_pointer += 2;
                line += 2;
            }
            else if (indexPtr[line + 1][0] == '['  &&  indexPtr[line + 1][arg_str_length - 1] == ']' // pop [rax]
            && RamRegisterNum(indexPtr[line + 1] + 1) >= 0 && arg_str_length == 5)
            {
                result[result_pointer] = n + 32 + 64;
                result[result_pointer + 1] = RamRegisterNum(indexPtr[line + 1] + 1);
                PrintListing(listing_file, &result[result_pointer], 2);
                result_pointer += 2;
                line += 2;
            }
            else // error for pop
            {
                printf("Wrong argument \"%s\" in command \"%s\" (word number %i)\n", indexPtr[line + 1], indexPtr[line], line + 1);
                printf("arg_str_length: %i\n", arg_str_length);
                free(buffer);
                free(indexPtr);
                free(result);
                return 1;
            }
        }
        else
        {
            result[result_pointer] = n;
            PrintListing(listing_file, &result[result_pointer], 1);
            result_pointer += 1;
            line += 1;
        }
    }







    free(buffer);
    free(indexPtr);
    free(result);
    fclose(result_file);

    printf("BOOOOM BEACH ASSEMBLED !!!!!!!\n");
    return 0;
}
