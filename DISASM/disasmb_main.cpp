#include <stdio.h>
#include <stdlib.h>
#include "colors.h"
#include "command+.h"
#include "command++.h"

#define DEF_CMD(name, num, need_arg, ...)                       \
else if ((input[cn] & 15) == num)                               \
{                                                               \
    if (need_arg)                                               \
    {                                                           \
        fprintf(output_file, #name);                            \
        PrintArg(&input[cn], &cn, output_file);                 \
    }                                                           \
    else                                                        \
    {                                                           \
        fprintf(output_file, #name "\n");                       \
    }                                                           \
    cn += 1;                                                    \
}
/*
push number (double) = 00 | 09 90 38 91 38 29 83 29
push rbx = 20 | 01
push [10] = 40 | 0a
push [rcx] = 60 | 02
32 64
pop = 01 |
pop rbx = 21 | 01
pop [10] = 41 | 0a
pop [rdx] = 61 | 03
*/
int PrintArg(unsigned char* command_data, int* cn_p, FILE* output_file)
{
    unsigned char cmd_data = command_data[0];
    unsigned char arg_data = command_data[1];
    if ((cmd_data & REG_FLAG) && (cmd_data & RAM_FLAG)) // [rax]
    {
        fprintf(output_file, " [%s]", register_list[arg_data]);
        *cn_p += 1;
    }
    else if (cmd_data & REG_FLAG) // rax
    {
        fprintf(output_file, " %s", register_list[arg_data]);
        *cn_p += 1;
    }
    else if (cmd_data & RAM_FLAG) // [10]
    {
        fprintf(output_file, " [%i]", arg_data);
        *cn_p += 1;
    }
    else if ((cmd_data & 15) == 1) // pop (empty)
    {
        // nothing
    }
    else if ((cmd_data & 15) == 0) // push (double)
    {
        double* num_p = (double*) &command_data[1];
        fprintf(output_file, " %lg", *num_p);
        *cn_p += 8;
    }
    else
    {
        // nothing
    }
    fprintf(output_file, "\n");
    return 0;
}

// signature = ')' (41), version, length (in bytes)
int main(int argc, char** argv)
{
    const int local_version = 1;
    const char* in_file_name = NULL;
    const char* out_file_name = NULL;

    if (argc < 2)
    {
        printf(KRED Kbright "Please specify the name of the executable file like this:\n"
                    KNRM Kbright "%s Compiled.txt\n" KNRM, argv[0]);
        return 1;
    }
    in_file_name = argv[1];

    if (argc > 2)
    {
        out_file_name = argv[2];
    }
    else
    {
        out_file_name = "Decompiled.txt";
    }

    FILE* output_file = fopen(out_file_name, "wb");
    FILE* input_file = fopen(in_file_name, "rb");

    if (output_file == NULL)
    {
        printf(KRED Kbright "Cannot create file <%s>\n" KNRM, out_file_name);
        return 1;
    }
    if (input_file == NULL)
    {
        printf(KRED Kbright "Cannot find file <%s>\n" KNRM, in_file_name);
        return 1;
    }

    char signature = 0, version = 0;
    int length = 0;

    if (fscanf(input_file, "%c", &signature) != 1 || signature != 41)
    {
        printf("Incompatable file type \"%s\"\n", in_file_name);
        fclose(input_file);
        fclose(output_file);
        return 1;
    }

    if (fscanf(input_file, "%c", &version) != 1 || version != local_version)
    {
        // make numbers in version after '.' random
        if (version == 0)
        {
            printf("Incompatable versions\nAssembler version: unknown, Disassembler version: %i.35-2\n", local_version);
        }
        else
        {
            printf("Incompatable versions\nAssembler version: %i.27-1, Disassembler version: %i.35-2\n", version, local_version);
        }
        fclose(input_file);
        fclose(output_file);
        return 1;
    }

    fscanf(input_file, "%i", &length);

    unsigned char* input = (unsigned char*) calloc(length + 1, sizeof(*input));
    fread(input, sizeof(*input), length, input_file);

    int cn = 0;
    while (cn < length)
    {
        if (0)
        {
        }
        #include "command.h" // defined check for all commands from "command.h"
        else
        {
            printf("Unknown command 0x%02x (bit %i)\n", input[cn], cn + 1);
            fclose(input_file);
            fclose(output_file);
            free(input);
            return 1;
        }
    }



    fclose(input_file);
    fclose(output_file);
    free(input);
    printf(KGRN Kbright"Successfully decompilated in file <%s>\n" KNRM, out_file_name);
}
