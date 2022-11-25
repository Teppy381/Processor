#include "proc_func.h"
#include "command+.h"

// signature = ')' (41), version, length (in bytes)


int main(int argc, char** argv)
{
    const int local_version = 2;

    if (argc < 2)
    {
        printf(KRED Kbright "Please specify the name of the executable file like this:\n"
                    KNRM Kbright "%s Compiled.txt\n" KNRM, argv[0]);
        return 1;
    }

    FILE* input_file = fopen(argv[1], "rb");
    if (input_file == NULL)
    {
        printf(KRED Kbright "Cannot find file <%s>\n" KNRM, argv[1]);
        return 1;
    }

    if (CheckSignature(input_file, local_version, argv[1]) != 0)
    {
        return 1;
    }

    Proc_struct proc = {0};

    fscanf(input_file, "%i", &proc.length);

    proc.stack_p    =    (Stack*)      calloc(1, sizeof(*proc.stack_p));
    proc.RAM        =    (Elem_t*)     calloc(256, sizeof(*proc.RAM));
    proc.reg        =    (Elem_t*)     calloc(register_amount, sizeof(*proc.reg));
    proc.input_data = (unsigned char*) calloc(proc.length + 1, sizeof(*proc.input_data));

    StackCtor(proc.stack_p, 4);

    fread(proc.input_data, sizeof(*proc.input_data), proc.length, input_file);
    fclose(input_file);

    int i = ExecuteCode(&proc);

    if (i == 1) // error
    {
        free(proc.stack_p->data);
        free(proc.stack_p);
        free(proc.RAM);
        free(proc.reg);
        free(proc.input_data);
        printf("!!! ERROR !!!\n");
        return 1;
    }
    else if (i == 2) // halt
    {
        printf(KGRN Kbright "Program halted\n" KNRM);
    }
    else // file ended
    {
        printf(KYLW Kbright "Run out of code\n" KNRM);
    }

    free(proc.stack_p->data);
    free(proc.stack_p);
    free(proc.RAM);
    free(proc.reg);
    free(proc.input_data);
    return 0;
}

