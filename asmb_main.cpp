#include "onegin_func.h"
#include "asmb_func.h"

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


#define DEF_CMD(name, num, need_arg, ...)                                       \
else if (strcmp(indexPtr[line], #name) == 0)                                    \
{                                                                               \
    result[result_pointer] = num;                                               \
    if (need_arg)                                                               \
    {                                                                           \
        do_return = GetArg(&result[result_pointer], &result_pointer, indexPtr, &line, listing_file);  \
    }                                                                           \
    else                                                                        \
    {                                                                           \
        PrintListing(listing_file, &result[result_pointer], 1);                 \
    }                                                                           \
    result_pointer += 1;                                                        \
    line += 1;                                                                  \
}



int main()
{
    const int local_version = 1;

    FILE* text_input = fopen("Y-prog.txt", "r"); // TODO pass file name as cmd arg
    char* buffer = ReadFile(text_input);
    int simbol_amount = CountSimbols(text_input);
    fclose(text_input);


    int line_amount = Countlines(buffer, simbol_amount);

    char** indexPtr = (char**) calloc(line_amount + 1, sizeof(char*));

    SplitToLines(indexPtr, buffer, simbol_amount);

    unsigned char* result = (unsigned char*) calloc(simbol_amount * 3, sizeof(*result));
    CHECKUS(result != NULL, 1);

    FILE* listing_file = fopen("Y-listing.txt", "w");
    CHECKUS(listing_file != NULL, 1);

    int line = 0, result_pointer = 0, do_return = 0;

    while (line < line_amount)
    {
        // need check for jump marker
        if (do_return != 0)
        {
            free(buffer);
            free(indexPtr);
            free(result);
            return 1;
        }

        #include "command.h" // defined check for all commands from "command.h"

        else // error
        {
            printf("Unknown command \"%s\" (word number %i)\n", indexPtr[line], line + 1);
            free(buffer);
            free(indexPtr);
            free(result);
            return 1;
        }
    }


    FILE* result_file = fopen("Y-compiled.txt", "w");
    CHECKUS(result_file != NULL, 1);

/*
    OutputResult(result_file, indexPtr, line_amount);
*/

    fprintf(result_file, "%c%c%i", 41, local_version, result_pointer);
    fwrite(result, sizeof(*result), result_pointer, result_file);

    free(buffer);
    free(indexPtr);
    free(result);
    fclose(result_file);

    printf("BOOOOM BEACH ASSEMBLED !!!!!!!\n");
    return 0;
}
