nasm -f elf32 -o asm_lineal.o asm_lineal.asm;
gcc -m32 -o programa asm_lineal.o programa.c -lpthread;
./programa;