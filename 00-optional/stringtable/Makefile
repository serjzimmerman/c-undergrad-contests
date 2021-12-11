CC=g++

CFLAGS=-Wall -O0 -g -pedantic
CLANGF=-style='{BasedOnStyle: LLVM, IndentWidth: 2,\
								AlignConsecutiveMacros: None,\
								AllowShortBlocksOnASingleLine: Never,\
								AllowShortIfStatementsOnASingleLine: Never,\
								AllowShortFunctionsOnASingleLine: None}'

TARGET=main

all:
	$(CC) $(CFLAGS) src/$(TARGET).cpp -o $(TARGET).elf

clean:
	rm $(TARGET).elf

clang-format:
	clang-format $(CLANGF) -i --verbose src/$(TARGET).cpp
