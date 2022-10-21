#include "proc_func.h"

// signature = ')' (41), version, length (in bytes)


int main()
{
    const int local_version = 1;
    const int register_amount = 4;

    const char* input_file_name = "Y-compiled.txt";
    FILE* input_file = fopen(input_file_name, "rb");

    if (CheckSignature(input_file, local_version, input_file_name) != 0)
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
        printf("Program halted\n");
    }
    else // file ended
    {
        printf("Run out of code\n");
    }

    free(proc.stack_p->data);
    free(proc.stack_p);
    free(proc.RAM);
    free(proc.reg);
    free(proc.input_data);
    return 0;
}

