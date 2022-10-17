CXX_FLAGS = -Wall -Wextra -fsanitize=address -g -ggdb3

all: processor assembler disassembler



processor: o/proc_main.o o/proc_func.o o/stack_func.o
	g++ o/proc_main.o o/proc_func.o o/stack_func.o $(CXX_FLAGS) -o X-processor.out

o/proc_main.o: proc_main.cpp
	g++ -c -o o/proc_main.o proc_main.cpp $(CXX_FLAGS)

o/stack_func.o: stack_func.cpp
	g++ -c -o o/stack_func.o stack_func.cpp	$(CXX_FLAGS)

o/proc_func.o: proc_func.cpp
	g++ -c -o o/proc_func.o proc_func.cpp	$(CXX_FLAGS)




assembler: o/asmb_main.o o/asmb_func.o o/onegin_func.o
	g++ o/asmb_main.o o/asmb_func.o o/onegin_func.o $(CXX_FLAGS) -o X-assembler.out

o/asmb_main.o: asmb_main.cpp
	g++ -c -o o/asmb_main.o asmb_main.cpp $(CXX_FLAGS)

o/asmb_func.o: asmb_func.cpp
	g++ -c -o o/asmb_func.o asmb_func.cpp $(CXX_FLAGS)

o/onegin_func.o: onegin_func.cpp
	g++ -c -o o/onegin_func.o onegin_func.cpp	$(CXX_FLAGS)




disassembler: o/disasmb_main.o
	g++ o/disasmb_main.o $(CXX_FLAGS) -o X-disassembler.out

o/disasmb_main.o: disasmb_main.cpp
	g++ -c -o o/disasmb_main.o disasmb_main.cpp $(CXX_FLAGS)
