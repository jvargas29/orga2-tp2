nasm -f elf32 -o tp2.o tp2.asm;
gcc -m32 -o hilos tp2.o hilos.c -lpthread;
./hilos;