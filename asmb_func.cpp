#include "asmb_func.h"

const char* register_list[] =
{
    "rax", "rbx", "rcx", "rdx"
};

#define DEF_CMD(name, num, ...)             \
else if (strcmp(str, #name) == 0)           \
{                                           \
    return num;                             \
}

int CommandNum(const char* str)
{
    if (str == NULL)
    {
        return -1;
    }
    #include "command.h"
    else
    {
        return -1;
    }
}
#undef DEF_CMD


int IsAllowedDigitsInt(char* str)
{
    CHECKUS(str != NULL, 1);
    int i = 0;
    while (str[i] != '\0')
    {
        if (!(isdigit(str[i])))
        {
            return 0;
        }
        i++;
    }
    return 1;
}

int IsAllowedDigitsDouble(char* str)
{
    CHECKUS(str != NULL, 1);
    int i = 0;
    while (str[i] != '\0')
    {
        if (!(isdigit(str[i]) || str[i] == '.' || str[i] == '-'))
        {
            return 0;
        }
        i++;
    }
    return 1;
}


int BytewiseDouble(unsigned char* data, double n)
{
    CHECKUS(data != NULL, 1);
    unsigned char* n_p = (unsigned char*) &n;
    for (int i = 0; i < 8; i++)
    {
        data[i] = n_p[i];
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


int GetArg(unsigned char* command_data_p, int* result_pointer_p, char** indexPtr, int* line_p, FILE* listing_file)
{
    int line = *line_p;
    int command_num = (int) *command_data_p;
    char* str0 = indexPtr[line];
    char* str1 = indexPtr[line + 1];
    int argument_length = strlen(str1);
    double double_buf_num = 0;
    int int_buf_num = 0;
    if (0)
    {
        // for jump
    }
    else if (command_num == 1 && CommandNum(str1) >= 0) // empty (pop)
    {
        PrintListing(listing_file, command_data_p, 1);
        return 0;
    }
    else if (command_num == 0 && IsAllowedDigitsDouble(str1) && sscanf(str1, "%lf", &double_buf_num) == 1) // double (push)
    {
        BytewiseDouble(command_data_p + 1, double_buf_num);
        PrintListing(listing_file, command_data_p, 9);
        *result_pointer_p += 8;
        *line_p += 1;
    }
    else if (str1[0] == '['  &&  str1[argument_length - 1] == ']') // [rax] or [10] (+ 64)
    {
        char* str_buf = strndup(str1 + 1, argument_length - 2);
        *command_data_p += 64;

        if (IsAllowedDigitsInt(str_buf) == 1) // 10 (+ 0)
        {
            sscanf(str_buf, "%i", &int_buf_num);
            if (int_buf_num > 255 || int_buf_num < 0)
            {
                printf("Invalid RAM cell number in argument \"%s\" in command \"%s\" (word number %i)\n"
                       "Allowed values: 0 - 255\n", str1, str0, line + 1);
                free(str_buf);
                return 1;
            }
            *(command_data_p + 1) = int_buf_num;
            *result_pointer_p += 1;
            *line_p += 1;
        }
        else if (RegNum(str_buf) >= 0) // rax (+ 32)
        {
            *(command_data_p + 1) = RegNum(str_buf);
            *command_data_p += 32;
            *result_pointer_p += 1;
            *line_p += 1;
        }
        else // error for []
        {
        printf("Wrong content of [] in argument \"%s\" in command \"%s\", "
        "only positive integers are allowed (word number %i)\n", str1, str0, line + 1);
        free(str_buf);
        return 1;
        }
        PrintListing(listing_file, command_data_p, 2);
        free(str_buf);
    }
    else if (RegNum(str1) >= 0) // rax (+ 32)
    {
        *(command_data_p + 1) = RegNum(str1);
        *command_data_p += 32;
        PrintListing(listing_file, command_data_p, 2);
        *result_pointer_p += 1;
        *line_p += 1;
    }
    else // error
    {
        printf("Wrong argument \"%s\" in command \"%s\" (word number %i)\n", str1, str0, line + 1);
        printf("CommandNum(str1): %i\n", CommandNum(str1));
        return 1;
    }
    return 0;
}


int RegNum(const char* str)
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
