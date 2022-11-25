#include "asmb_func.h"
#include "command+.h"
#include "command++.h"

int JumpNum(const char* str)
{
    CHECKUS(str != NULL, 1);
    int jump_amount = sizeof(jump_list) / sizeof(*jump_list);
    for (int i = 0; i < jump_amount; i++)
    {
        if (strcmp(str, jump_list[i]) == 0)
        {
            return i;
        }
    }
    return -1;
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

int AssembleAll(char** indexPtr, unsigned char* result, FILE* listing_file, label_struct* label_list, int* label_counter_p, int line_amount, int passage, int* byte_amount)
{
    int line = 0, result_pointer = 0, do_return = 0;

    printf(KRED Kbright);

    while (line < line_amount || do_return != 0)
    {
        if (do_return != 0)
        {
            return 1;
        }
        else if (indexPtr[line][strlen(indexPtr[line]) - 1] == ':') // label:
        {
            //printf("Label found: \"%s\"\n", indexPtr[line]);
            if (IsLabel(indexPtr[line]))
            {
                if (passage != 0)
                {
                    line += 1;
                }
                else if (FindLabel(indexPtr[line], label_list, *label_counter_p) < 0)
                {
                    //printf("Label added: \"%s\"\n", indexPtr[line]);
                    (label_list[*label_counter_p]).name = indexPtr[line];
                    (label_list[*label_counter_p]).adress = result_pointer;
                    *label_counter_p += 1;
                    line += 1;
                    // do not increace result_pointer
                }
                else
                {
                    printf("Multiple declaration for label \"%s\" (second declaration in word number %i)\n", indexPtr[line], line + 1);
                    return 1;
                }
            }
            else
            {
                printf("Not allowed name for label \"%s\" (word number %i)\n", indexPtr[line], line + 1);
                return 1;
            }
        }
        else if (JumpNum(indexPtr[line]) >= 0 && CommandNum(indexPtr[line]) >= 0) // jump label
        {
            if (passage == 0)
            {
                //printf("Jump skipped \"%s\"\n", indexPtr[line + 1]);
                result_pointer += 2;
                line += 2;
            }
            else
            {
                int i = FindLabel(indexPtr[line + 1], label_list, *label_counter_p);
                if (i < 0)
                {
                    printf("Cannot find jump label \"%s\" (word number %i)\n", indexPtr[line + 1], line + 2);
                    return 1;
                }
                result[result_pointer] = CommandNum(indexPtr[line]);
                result[result_pointer + 1] = i;
                PrintListing(listing_file, &result[result_pointer], 2);
                result_pointer += 2;
                line += 2;
            }
        }

        #include "command.h" // defined check for all commands from "command.h"

        else // error
        {
            printf("Unknown command \"%s\" (word number %i)\n", indexPtr[line], line + 1);
            return 1;
        }
    }
    *byte_amount = result_pointer;

    printf(KNRM);
    return 0;
}
#undef DEF_CMD



int IsLabel(const char* str)
{
    CHECKUS(str != NULL, 1);
    int i = 0, k = strlen(str) - 1;
    if (str[k] != ':')
    {
        return 0;
    }
    while (i < k)
    {
        if (!(isalnum(str[i])))
        {
            return 0;
        }
        i++;
    }
    return 1;
}

int FindLabel(const char* str, const label_struct* label_list, const int label_amount)
{
    CHECKUS(str != NULL, 1);
    CHECKUS(label_list != NULL, 1);

    int label_length = 0, max_length = 0, str_length = strlen(str);

    if (str[str_length - 1] == ':')
    {
        str_length -= 1;
    }

    for (int i = 0; i < label_amount; i++)
    {
        label_length = strlen(label_list[i].name);
        if (label_list[i].name[label_length - 1] == ':')
        {
            label_length -= 1;
        }

        max_length = (label_length > str_length) ? label_length : str_length;

        if (strncmp(str, label_list[i].name, max_length) == 0)
        {
            return label_list[i].adress;
        }
    }
    return -1;
}

int IsAllowedDigitsInt(const char* str)
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

int IsAllowedDigitsDouble(const char* str)
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

    printf(KRED Kbright);

    if (command_num == 1 && (CommandNum(str1) >= 0 || str1[argument_length - 1] == ':')) // empty (pop)
    {
        PrintListing(listing_file, command_data_p, 1);
        return 0;
    }
    else if (command_num == 0 && IsAllowedDigitsDouble(str1) && sscanf(str1, "%lf", &double_buf_num) == 1) // double (push)
    {
        memcpy(command_data_p + 1, &double_buf_num, 8);
        PrintListing(listing_file, command_data_p, 9);
        *result_pointer_p += 8;
        *line_p += 1;
    }
    else if (str1[0] == '['  &&  str1[argument_length - 1] == ']') // [rax] or [10] (+ RAM_FLAG)
    {
        char* str_buf = strndup(str1 + 1, argument_length - 2);
        *command_data_p += RAM_FLAG;

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
        else if (RegNum(str_buf) >= 0) // rax (+ REG_FLAG)
        {
            *(command_data_p + 1) = RegNum(str_buf);
            *command_data_p += REG_FLAG;
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
    else if (RegNum(str1) >= 0) // rax (+ REG_FLAG)
    {
        *(command_data_p + 1) = RegNum(str1);
        *command_data_p += REG_FLAG;
        PrintListing(listing_file, command_data_p, 2);
        *result_pointer_p += 1;
        *line_p += 1;
    }
    else // error
    {
        printf("Wrong argument \"%s\" in command \"%s\" (word number %i)\n", str1, str0, line + 1);
        return 1;
    }

    printf(KNRM);
    return 0;
}

