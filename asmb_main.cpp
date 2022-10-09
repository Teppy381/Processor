#include "onegin_func.h"
#include <string.h>

/*
push
pop

add
sub
mul
div

inp ?
out

dump
jump
hlt
*/
const char* command_list[] =
{
    "push", "pop",
    "add", "sub", "mul", "div",
    "out", "hlt"
};

int command_num(char* str)
{
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
    for (int i = 0; i < n; i++)
    {
        fprintf(listing_file, "%02x ", str[i]);
    }
    fprintf(listing_file, "\n");
    return 0;
}

int main()
{
    printf("\n");

    FILE* text_input = fopen("Y-prog.txt", "r");
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
    int n = 0;
    double num_buf = 0;
    int result_pointer = 0;

    while (line < line_amount)
    {
        n = command_num(indexPtr[line]);
        if (n < 0)
        {
            printf("Unknown command \"%s\" (word number %i)\n", indexPtr[line], line + 1);
            free(buffer);
            free(indexPtr);
            free(result);
            return 1;
        }
        else if (n == 0) // push
        {
            if (sscanf(indexPtr[line + 1], "%lf", &num_buf) != 1)
            {
                printf("Wrong argument \"%s\" in command \"%s\" (word number %i)\n", indexPtr[line + 1], indexPtr[line], line + 1);
                free(buffer);
                free(indexPtr);
                free(result);
                return 1;
            }
            else
            {
                result[result_pointer] = n;
                BytewiseDouble(&result[result_pointer + 1], num_buf);

                PrintListing(listing_file, &result[result_pointer], 9);
                result_pointer += 9;
                line += 2;
            }
        }
        else if (n == 1) // pop
        {
            if (line + 1 < line_amount && command_num(indexPtr[line + 1]) < 0)
            {
                result[result_pointer] = n + 32;
                result[result_pointer + 1] = 0;
                PrintListing(listing_file, &result[result_pointer], 2);
                result_pointer += 2;
                line += 2;
            }
            else
            {
                result[result_pointer] = n;
                PrintListing(listing_file, &result[result_pointer], 1);
                result_pointer += 1;
                line += 1;
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
