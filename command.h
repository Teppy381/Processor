DEF_CMD(push, 0, 1,
{
    return ExecutePush(proc_p);
})
DEF_CMD(pop, 1, 1,
{
    return ExecutePop(proc_p);
})

DEF_CMD(add, 2, 0,
{
    Elem_t x = 0;
    Elem_t y = 0;
    if (Pop(proc_p->stack_p, &x) != 0)
        return 1;
    if (Pop(proc_p->stack_p, &y) != 0)
        return 1;
    Push(proc_p->stack_p, y + x);
    return 0;
})
DEF_CMD(sub, 3, 0,
{
    Elem_t x = 0;
    Elem_t y = 0;
    if (Pop(proc_p->stack_p, &x) != 0)
        return 1;
    if (Pop(proc_p->stack_p, &y) != 0)
        return 1;
    Push(proc_p->stack_p, y - x);
    return 0;
})
DEF_CMD(mul, 4, 0,
{
    Elem_t x = 0;
    Elem_t y = 0;
    if (Pop(proc_p->stack_p, &x) != 0)
        return 1;
    if (Pop(proc_p->stack_p, &y) != 0)
        return 1;
    Push(proc_p->stack_p, y * x);
    return 0;
})
DEF_CMD(div, 5, 0,
{
    Elem_t x = 0;
    Elem_t y = 0;
    if (Pop(proc_p->stack_p, &x) != 0)
        return 1;
    if (Pop(proc_p->stack_p, &y) != 0)
        return 1;
    Push(proc_p->stack_p, y / x);
    return 0;
})

DEF_CMD(out, 6, 0,
{
    Elem_t x = 0;
    if (Pop(proc_p->stack_p, &x) != 0)
        return 1;
    printf("%lg\n", x);
    return 0;
})
DEF_CMD(in, 7, 0,
{
    Elem_t x = 0;
    scanf("%lg", &x);
    Push(proc_p->stack_p, x);
    return 0;
})

DEF_CMD(hlt, 8, 0, {return 2;})
