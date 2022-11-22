#include "proc_func.h"


int CheckSignature(FILE* input_file, const int local_version, const char* input_file_name)
{
    char signature = 0, version = 0;

    if (fscanf(input_file, "%c", &signature) != 1 || signature != 41)
    {
        printf("Incompatable file type \"%s\"\n", input_file_name);
        fclose(input_file);
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
        return 1;
    }
    return 0;
}

void ClearInput()
{
    while (getchar() != '\n') {};
}

int IsEqual(Elem_t x, Elem_t y)
{
    if (fabs(x - y) < ZERO_ACCURACY)
    {
        return 1;
    }
    return 0;
}

int ExecuteCode(Proc_struct* proc_p)
{
    int do_return = 0;
    while (proc_p->cn < proc_p->length)
    {
        do_return = ExecuteCommand(proc_p);

        if (do_return == 1) // error
        {
            return 1;
        }
        else if (do_return == 2) // halt
        {
            return 2;
        }
        proc_p->cn += 1;
    }
    return 0; // run out of code
}



#define DEF_CMD(name, num, need_arg, proc_task)                 \
case num:                                                       \
    {proc_task}


int ExecuteCommand(Proc_struct* proc_p)
{
    int num = (int) proc_p->input_data[proc_p->cn];

    if (num == 0 || num == 32 || num == 64 || num == 96)
    {
        num = 0;
    }
    else if (num == 1 || num == 33 || num == 65 || num == 97)
    {
        num = 1;
    }

    // (15 = 00001111) (240 = 11110000)

    // printf("ExecuteCommand num: %i\n", num);
    switch (num)
    {
        #include "command.h"
        default:
        {
            printf("Unknown argument for 0x%02x (bit %i)\n", proc_p->input_data[proc_p->cn], proc_p->cn + 1);
            return 1;
        }
    }
}
#undef DEF_CMD


/*
push number (double) = 00 | 09 90 38 91 38 29 83 29
push rbx             = 20 | 01
push [10]            = 40 | 0a
push [rcx]           = 60 | 02

pop (empty) = 01 |
pop rbx     = 21 | 01
pop [10]    = 41 | 0a
pop [rdx]   = 61 | 03
*/

int ExecutePush(Proc_struct* proc_p)
{
    switch ((int) proc_p->input_data[proc_p->cn] >> 4)
    {
        case 0: // push number (double)
        {
            Elem_t x = 0;
            for (unsigned long i = 0; i < sizeof (Elem_t); i++)
            {
                proc_p->cn++;
                ((unsigned char*) &x)[i] = proc_p->input_data[proc_p->cn];
            }
            Push(proc_p->stack_p, x);
            return 0;
        }
        case 2: // push rbx
        {
            proc_p->cn += 1;
            Elem_t x = proc_p->reg[proc_p->input_data[proc_p->cn]];
            Push(proc_p->stack_p, x);
            return 0;
        }
        case 4: // push [10]
        {
            proc_p->cn += 1;
            Elem_t x = proc_p->RAM[proc_p->input_data[proc_p->cn]];
            Push(proc_p->stack_p, x);
            return 0;
        }
        case 6: // push [rcx]
        {
            proc_p->cn += 1;
            Elem_t x = proc_p->RAM[(int) proc_p->reg[proc_p->input_data[proc_p->cn]]];
            Push(proc_p->stack_p, x);
            return 0;
        }
        default:
        {
            return 1;
        }
    }
    return 0;
}

int ExecutePop(Proc_struct* proc_p)
{
    switch ((int) proc_p->input_data[proc_p->cn] >> 4)
    {
        case 0: // pop (empty)
        {
            Elem_t x = 0;
            return Pop(proc_p->stack_p, &x);
        }
        case 2: // pop rbx
        {
            proc_p->cn += 1;
            Elem_t* x_p = &(proc_p->reg[proc_p->input_data[proc_p->cn]]);
            return Pop(proc_p->stack_p, x_p);
        }
        case 4: // pop [10]
        {
            proc_p->cn += 1;
            Elem_t* x_p = &(proc_p->RAM[proc_p->input_data[proc_p->cn]]);
            return Pop(proc_p->stack_p, x_p);
        }
        case 6: // pop [rdx]
        {
            proc_p->cn += 1;
            Elem_t* x_p = &(proc_p->RAM[(int) proc_p->reg[proc_p->input_data[proc_p->cn]]]);
            return Pop(proc_p->stack_p, x_p);
        }
        default:
        {
            return 1;
        }
    }
    return 0;
}
