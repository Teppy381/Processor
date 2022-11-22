CXX_FLAGS = -Wall -Wextra -fsanitize=address -g -ggdb3

DIR_FLAGS = -IASM -ICPU -IDISASM -IEXTRA

CXX_FLAGS += $(DIR_FLAGS)


all: processor assembler disassembler


processor: OBJECTS/proc_main.o OBJECTS/proc_func.o OBJECTS/stack_func.o
	g++ OBJECTS/proc_main.o OBJECTS/proc_func.o OBJECTS/stack_func.o $(CXX_FLAGS) -o CPU.out

OBJECTS/proc_main.o: CPU/proc_main.cpp
	g++ -c -o OBJECTS/proc_main.o CPU/proc_main.cpp $(CXX_FLAGS)

OBJECTS/stack_func.o: EXTRA/stack_func.cpp
	g++ -c -o OBJECTS/stack_func.o EXTRA/stack_func.cpp	$(CXX_FLAGS)

OBJECTS/proc_func.o: CPU/proc_func.cpp
	g++ -c -o OBJECTS/proc_func.o CPU/proc_func.cpp	$(CXX_FLAGS)




assembler: OBJECTS/asmb_main.o OBJECTS/asmb_func.o OBJECTS/onegin_func.o
	g++ OBJECTS/asmb_main.o OBJECTS/asmb_func.o OBJECTS/onegin_func.o $(CXX_FLAGS) -o ASM.out

OBJECTS/asmb_main.o: ASM/asmb_main.cpp
	g++ -c -o OBJECTS/asmb_main.o ASM/asmb_main.cpp $(CXX_FLAGS)

OBJECTS/asmb_func.o: ASM/asmb_func.cpp
	g++ -c -o OBJECTS/asmb_func.o ASM/asmb_func.cpp $(CXX_FLAGS)

OBJECTS/onegin_func.o: EXTRA/onegin_func.cpp
	g++ -c -o OBJECTS/onegin_func.o EXTRA/onegin_func.cpp	$(CXX_FLAGS)




disassembler: OBJECTS/disasmb_main.o
	g++ OBJECTS/disasmb_main.o $(CXX_FLAGS) -o DISASM.out

OBJECTS/disasmb_main.o: DISASM/disasmb_main.cpp
	g++ -c -o OBJECTS/disasmb_main.o DISASM/disasmb_main.cpp $(CXX_FLAGS)
