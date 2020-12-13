nasm -f elf32 -o tp2.o tp2.asm
gcc -m32 -o lineal tp2.o c_lineal.c -lpthread;
./lineal;