#include "onegin_func.h"
#include "stack_func.h"

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

int main()
{
    double v = 23.34;
    unsigned char* str = (unsigned char*) calloc(20, sizeof(*str));
    BytewiseDouble(str, v);

    printf("%s\n", str);
    printf("BOOOOM BEACH PROCESSED !!!!!!!\n");
    return 0;
}
