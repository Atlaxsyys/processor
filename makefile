.PHONY: all assembler processor

all: assembler processor

assembler:
	make -C assembler

assembler-build:
	make -C assembler build

assembler-run:
	make -C assembler run

assembler-squaresolver:
	make -C assembler squaresolver

assembler-factorial:
	make -C assembler factorial
	
processor:
	make -C processor

processor-build:
	make -C processor build

processor-squaresolver:
	make -C processor squaresolver

processor-factorial:
	make -C processor factorial
