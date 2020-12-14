#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include<pthread.h>
#include <time.h>
#include"funciones.h"

long enmascarar_hilos(parameter_struct *args, parameter_struct *args2,parameter_struct *args3);
long enmascarar_lineal_C(parameter_struct *args, parameter_struct *args2,parameter_struct *args3);
long enmascarar_asm(parameter_struct *args, parameter_struct *args2,parameter_struct *args3);

int main()
{
 
  parameter_struct *args = malloc(sizeof *args);
  args->img1 = "photo1.bmp";
  args->img2 = "photo2.bmp";
  args->mascara = "mask4.bmp";
  args->width = 1280;
  args->heigth = 960;
  args->result = "result.bmp";

  parameter_struct *args2 = malloc(sizeof *args2);
  args2->img1 = "img1.bmp";
  args2->img2 = "img2.bmp";
  args2->mascara = "mask.bmp";
  args2->width = 500;
  args2->heigth = 490;
  args2->result = "result2.bmp";

  parameter_struct *args3 = malloc(sizeof *args3);
  args3->img1 = "w1.bmp";
  args3->img2 = "w2.bmp";
  args3->mascara = "mask5.bmp";
  args3->width = 1758;
  args3->heigth = 1172;
  args3->result = "result3.bmp";

long timeHilos = enmascarar_hilos(args,args2,args3);

long timeLineal = enmascarar_lineal_C(args,args2,args3);
long timeAsm= enmascarar_asm(args,args2,args3);

  free(args);
  free(args2);
  free(args3);

  FILE *fp;
    fp=fopen("tiempos.csv","w+");
    fprintf(fp,"C_LINEAL; C_HILOS; C_MMX");
    fprintf(fp,"\n%ld",timeLineal);
    fprintf(fp,"; %ld",timeHilos);
    fprintf(fp,"; %ld",timeAsm);
    fclose(fp);
   

  return 0;
}

long enmascarar_hilos(parameter_struct *args, parameter_struct *args2,parameter_struct *args3){
  pthread_t hilo;
  pthread_t hilo2;
  pthread_t hilo3;
  clock_t tiempo_inicio, tiempo_final;


    args->result = "result_hilos1.bmp";
    args2->result = "result_hilos2.bmp";
    args3->result = "result_hilos3.bmp";

  
    tiempo_inicio = clock();

 
    pthread_create(&hilo, NULL, principal,  (void *)args);
    pthread_create(&hilo2, NULL, principal,  (void *)args2);
    pthread_create(&hilo3, NULL, principal,  (void *)args3);

    pthread_join(hilo, NULL);
    pthread_join(hilo2, NULL);
    pthread_join(hilo3, NULL);

    tiempo_final = clock();

  unsigned long total = (tiempo_final - tiempo_inicio );
    printf("Tiempo de ejecucion HILOS: %ld unidades de clock.\n", total);
  return total;
}

long enmascarar_lineal_C(parameter_struct *args, parameter_struct *args2,parameter_struct *args3){
    clock_t tiempo_inicio, tiempo_final;

    args->result = "result_lineal1.bmp";
    args2->result = "result_lineal2.bmp";
    args3->result = "result_lineal3.bmp";

    tiempo_inicio = clock();

    principal(args);
    principal(args2);
    principal(args3);

    tiempo_final = clock();
    unsigned long total = (tiempo_final - tiempo_inicio );
    printf("Tiempo de ejecucion LINEAL: %ld unidades de clock.\n", total);

    return total;
}

long enmascarar_asm(parameter_struct *args, parameter_struct *args2,parameter_struct *args3){
   clock_t   tiempo_inicio, tiempo_final;

    args->result = "result_asm1.bmp";
    args2->result = "result_asm2.bmp";
    args3->result = "result_asm3.bmp";

       tiempo_inicio = clock();


    principal_asm(args);
    principal_asm(args2);
    principal_asm(args3);

    tiempo_final = clock();
    unsigned long total = (tiempo_final - tiempo_inicio );
    printf("Tiempo de ejecucion MMX: %ld unidades de clock.\n", total);

    return total;
}