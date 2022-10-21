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

in
out
hlt

jump label
label:
*/


int main()
{
    const int local_version = 1;

    FILE* text_input = fopen("Y-prog.txt", "r"); // TODO pass file name as cmd arg
    char* buffer = ReadFile(text_input);
    int simbol_amount = CountSimbols(text_input);
    fclose(text_input);


    int line_amount = Countlines(buffer, simbol_amount);

    char** indexPtr = (char**) calloc(line_amount + 1, sizeof(char*));
    CHECKUS(indexPtr != NULL, 1);

    SplitToLines(indexPtr, buffer, simbol_amount);

    unsigned char* result = (unsigned char*) calloc(simbol_amount * 3, sizeof(*result));
    CHECKUS(result != NULL, 1);

    FILE* listing_file = fopen("Y-listing.txt", "w");
    CHECKUS(listing_file != NULL, 1);

    int byte_amount = 0;

    label_struct* label_list = (label_struct*) calloc(256, sizeof(*label_list));
    int label_counter = 0;

    if (AssembleAll(indexPtr, result, listing_file, label_list, &label_counter, line_amount, 0, &byte_amount) != 0)
    {
        free(label_list);
        free(buffer);
        free(indexPtr);
        free(result);
        return 1;
    }

    fseek(listing_file, 0, SEEK_SET);

    if (AssembleAll(indexPtr, result, listing_file, label_list, &label_counter, line_amount, 1, &byte_amount) != 0)
    {
        free(label_list);
        free(buffer);
        free(indexPtr);
        free(result);
        return 1;
    }


    FILE* result_file = fopen("Y-compiled.txt", "w");
    CHECKUS(result_file != NULL, 1);


    fprintf(result_file, "%c%c%i", 41, local_version, byte_amount);
    fwrite(result, sizeof(*result), byte_amount, result_file);

    free(label_list);
    free(buffer);
    free(indexPtr);
    free(result);
    fclose(result_file);

    printf("BOOOOM BEACH ASSEMBLED !!!!!!!\n");
    return 0;
}
