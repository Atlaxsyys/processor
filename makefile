
all:
	@g++ asm.cpp -o asm.exe
	@g++ processor.cpp hash.cpp stack.cpp -o processor.exe
run:
	@./asm.exe commands.txt converted_commands.bin
	@./processor.exe converted_commands.bin