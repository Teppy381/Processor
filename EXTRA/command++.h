#ifndef COMMAND_PLUS_PLUS_H
#define COMMAND_PLUS_PLUS_H

/*
push number (double) = 00 | 09 90 38 91 38 29 83 29
push rbx             = 20 | 01
push [10]            = 40 | 0a
push [rcx]           = 60 | 02

0 32 64 96

pop (empty) = 01 |
pop rbx     = 21 | 01
pop [10]    = 41 | 0a
pop [rdx]   = 61 | 03
*/

#define REG_FLAG  32
#define RAM_FLAG  64

#endif
