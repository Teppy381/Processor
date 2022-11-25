## Processor (ASM, DISASM, CPU) ##
---
**Before start you need to use makefile to create ASM.out, CPU.out and DISASM.out**
  
---
### Example of use:
Here is the contents of the **My_Code.txt** file:
```
in
push 1
sub
dup
mul
out
hlt
```
This program represents formula **(x - 1)^2**
  
We can compile it in console:
```
./ASM.out My_Code.txt Compiled_Code.txt
```
This will create file **Compiled_Code.txt** in local folder
  
Now we can run it:
```
./CPU.out Compiled_Code.txt
```

Also we can decompile this file:
```
./DISASM.out Compiled_Code.txt My_Decompiled_Code.txt
```
That will create file **My_Decompiled_Code.txt** in local folder
