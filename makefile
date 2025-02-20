
all:
	@g++ asm.cpp -o asm.exe
	@g++ processor.cpp hash.cpp stack.cpp -o processor.exe