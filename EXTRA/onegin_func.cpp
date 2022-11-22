#include "onegin_func.h"

char* ReadFile(FILE* text_input)
{
    CHECKUS(text_input != NULL, NULL);

    int simbol_amount = CountSimbols(text_input);

    char* buffer = (char*) calloc(simbol_amount + 1, sizeof(*buffer));
    CHECKUS(buffer != NULL, NULL);

    fread(buffer, sizeof(*buffer), simbol_amount, text_input);
    buffer[simbol_amount - 1] = '\0';

    return buffer;
}


int CountSimbols(FILE* text_input)
{
    CHECKUS(text_input != NULL, 1);

    fseek(text_input, 0, SEEK_END);
    int simbol_amount = ftell(text_input);
    fseek(text_input, 0, SEEK_SET);

    return simbol_amount;
}


int Countlines(char* buffer, int simbol_amount)
{
    CHECKUS(buffer != NULL, 1);
    CHECKUS(simbol_amount >= 0, 1);

    int simbol_num = 0;
    int line_num = 0;

    if (!(isspace(buffer[0]) || buffer[0] == ';'))
    {
        line_num += 1;
    }

    while (simbol_num <= simbol_amount)
    {
        if (buffer[simbol_num] == ';')
        {
            while (buffer[simbol_num] != '\n')
            {
                simbol_num++;
            }
        }
        if (isspace(buffer[simbol_num]))
        {
            simbol_num++;
            if (buffer[simbol_num] == ';')
            {
                while (buffer[simbol_num] != '\n')
                {
                    simbol_num++;
                }
            }
            while (isspace(buffer[simbol_num]))
            {
                simbol_num++;
                if (buffer[simbol_num] == ';')
                {
                    while (buffer[simbol_num] != '\n')
                    {
                        simbol_num++;
                    }
                }
            }
            line_num++;
        }
        simbol_num++;
    }
    return line_num - 1;
}


int SplitToLines(char** indexPtr, char* buffer, int simbol_amount)
{
    CHECKUS(indexPtr != NULL, 1);
    CHECKUS(buffer != NULL, 1);
    CHECKUS(simbol_amount >= 0, 1);

    int simbol_num = 0;
    int line_num = 0;

    if (!(isspace(buffer[0]) || buffer[0] == ';'))
    {
        indexPtr[0] = &(buffer[0]);
        line_num += 1;
    }

    while (simbol_num <= simbol_amount)
    {
        if (buffer[simbol_num] == ';')
        {
            buffer[simbol_num] = '\0';
            while (buffer[simbol_num] != '\n')
            {
                simbol_num++;
            }
        }
        if (isspace(buffer[simbol_num]))
        {
            buffer[simbol_num] = '\0';
            simbol_num++;
            if (buffer[simbol_num] == ';')
            {
                while (buffer[simbol_num] != '\n')
                {
                    simbol_num++;
                }
            }
            while (isspace(buffer[simbol_num]))
            {
                simbol_num++;
                if (buffer[simbol_num] == ';')
                {
                    while (buffer[simbol_num] != '\n')
                    {
                        simbol_num++;
                    }
                }
            }

            indexPtr[line_num] = &(buffer[simbol_num]);
            line_num++;
        }
        simbol_num++;
    }
    return 0;
}


void OutputResult(FILE* result_file, char** indexPtr, int line_amount)
{
    CHECKUS(result_file != NULL, (void) 0);
    CHECKUS(indexPtr != NULL, (void) 0);
    CHECKUS(line_amount >= 0, (void) 0);

    for (int i = 0; i < line_amount; i++)
    {
        fprintf(result_file,"\"%s\"\n", indexPtr[i]);
    }
    fprintf(result_file,"\n\n");
}
