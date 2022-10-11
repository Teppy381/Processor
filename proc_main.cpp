#include "onegin_func.h"
#include "stack_func.h"


int main()
{
    double n;
    CHECKUS(scanf("[%lf]", &n) == 1, 1);

    printf("%lf\n", n);
    printf("BOOOOM BEACH PROCESSED !!!!!!!\n");
    return 0;
}
