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
    printf(KMGN Kbright "%lg\n" KNRM, x);
    return 0;
})

DEF_CMD(in, 7, 0,
{
    Elem_t x = 0;
    printf("Enter a number: " KMGN Kbright);
    if (scanf("%lg", &x) != 1)
    {
        printf(KNRM);
        printf(KRED "It's not a number, try again\n" KNRM);
        proc_p->cn -= 1;
        ClearInput();
    }
    else
    {
        printf(KNRM);
        ClearInput();
        Push(proc_p->stack_p, x);
    }
    return 0;
})

DEF_CMD(hlt, 8, 0,
{
    return 2;
})

DEF_CMD(jump, 9, 0,
{
    proc_p->cn = proc_p->input_data[proc_p->cn + 1] - 1;
    // printf(KRED "JUMPED\n" KNRM);
    return 0;
})

DEF_CMD(jb, 10, 0,
{
    Elem_t x = 0;
    Elem_t y = 0;
    if (Pop(proc_p->stack_p, &x) != 0)
        return 1;
    if (Pop(proc_p->stack_p, &y) != 0)
        return 1;
    if (y < x)
    {
        proc_p->cn = proc_p->input_data[proc_p->cn + 1] - 1;
    }
    else
    {
        proc_p->cn += 1;
    }
    return 0;
})

DEF_CMD(jbe, 11, 0,
{
    Elem_t x = 0;
    Elem_t y = 0;
    if (Pop(proc_p->stack_p, &x) != 0)
        return 1;
    if (Pop(proc_p->stack_p, &y) != 0)
        return 1;
    if (y <= x)
    {
        proc_p->cn = proc_p->input_data[proc_p->cn + 1] - 1;
    }
    else
    {
        proc_p->cn += 1;
    }
    return 0;
})

DEF_CMD(ja, 12, 0,
{
    Elem_t x = 0;
    Elem_t y = 0;
    if (Pop(proc_p->stack_p, &x) != 0)
        return 1;
    if (Pop(proc_p->stack_p, &y) != 0)
        return 1;
    if (y > x)
    {
        proc_p->cn = proc_p->input_data[proc_p->cn + 1] - 1;
        // printf(KRED "JUMPED ja\n" KNRM);
    }
    else
    {
        proc_p->cn += 1;
    }
    return 0;
})

DEF_CMD(jae, 13, 0,
{
    Elem_t x = 0;
    Elem_t y = 0;
    if (Pop(proc_p->stack_p, &x) != 0)
        return 1;
    if (Pop(proc_p->stack_p, &y) != 0)
        return 1;
    if (y >= x)
    {
        proc_p->cn = proc_p->input_data[proc_p->cn + 1] - 1;
    }
    else
    {
        proc_p->cn += 1;
    }
    return 0;
})

DEF_CMD(je, 14, 0,
{
    Elem_t x = 0;
    Elem_t y = 0;
    if (Pop(proc_p->stack_p, &x) != 0)
        return 1;
    if (Pop(proc_p->stack_p, &y) != 0)
        return 1;
    if (IsEqual(x, y))
    {
        proc_p->cn = proc_p->input_data[proc_p->cn + 1] - 1;
    }
    else
    {
        proc_p->cn += 1;
    }
    return 0;
})

DEF_CMD(jne, 15, 0,
{
    Elem_t x = 0;
    Elem_t y = 0;
    if (Pop(proc_p->stack_p, &x) != 0)
        return 1;
    if (Pop(proc_p->stack_p, &y) != 0)
        return 1;
    if (!(IsEqual(x, y)))
    {
        proc_p->cn = proc_p->input_data[proc_p->cn + 1] - 1;
    }
    else
    {
        proc_p->cn += 1;
    }
    return 0;
})

DEF_CMD(dup, 16, 0,
{
    Elem_t x = 0;
    if (Pop(proc_p->stack_p, &x) != 0)
    {
        return 1;
    }
    Push(proc_p->stack_p, x);
    Push(proc_p->stack_p, x);
    return 0;
})

DEF_CMD(round, 17, 0,
{
    Elem_t x = 0;
    if (Pop(proc_p->stack_p, &x) != 0)
    {
        return 1;
    }
    Push(proc_p->stack_p, round(x));
    return 0;
})

