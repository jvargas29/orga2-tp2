#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include<pthread.h>
#include<time.h>


extern void CMAIN(unsigned char *img1pixels, unsigned char *img2pixels, unsigned char *maskPixels, int dimensions);

typedef struct bmpFileHeader
{
  /* 2 bytes de identificación */
  uint32_t size;        /* Tamaño del archivo */
  uint16_t resv1;       /* Reservado */
  uint16_t resv2;       /* Reservado */
  uint32_t offset;      /* Offset hasta hasta los datos de imagen */
} bmpFileHeader;

typedef struct parameter_struct{
   unsigned char *img1; 
   unsigned char *img2; 
   unsigned char *mascara; 
   int width; 
   int heigth; 
   unsigned char *result;
} parameter_struct;

int loadFile(char *fileName, int dimensions, unsigned char *buffer){
    FILE *file;
     bmpFileHeader header;    
  uint16_t type;   
    file = fopen(fileName, "r");
    if(file == NULL){
        printf("Error loading fiile\n");
    } else {
        fread(&type, sizeof(uint16_t), 1, file);
        fread(&header, sizeof(bmpFileHeader), 1, file);
        fseek(file,0 , SEEK_SET);
        fread(buffer, dimensions, 1, file);
    }
    fclose(file);
    return header.offset;
}


void saveFile(char *fileName, int dimensions, unsigned char *buffer){
    FILE *file;
    file = fopen(fileName, "w");
    fwrite(buffer, 1, dimensions, file);
    fclose(file);
}

void enmascarar_c(unsigned char *img1pixels, unsigned char *img2pixels, unsigned char *maskPixels, int heigth, int width)
{
    int r,g,b;
 
    for(int x = 0; x < heigth* width; x++)
  {
      b = maskPixels[3 * x];
      g = maskPixels[(3 * x) + 1];
      r = maskPixels[(3 * x)+2];
    printf("x pos %d",x);
    if(b == 0 && g== 0 && r == 0){
        img1pixels[3 * x] = img2pixels[3 * x];
        img1pixels[(3 * x)+1] = img2pixels[(3 * x)+1];
        img1pixels[(3 * x)+2] = img2pixels[(3 * x)+2];
      }
  }

}


//int principal(int arg, char *argv[])
//int *principal(unsigned char *img1, unsigned char *img2, unsigned char *mascara, int width, int heigth, unsigned char *result)
void *principal(void *args)
{

    struct parameter_struct *actual_args;
        actual_args = (struct parameter_struct *)args;
  /* char *imagen1 = argv[1];
    char *imagen2 = argv[2];
    char *mask = argv[3];
*/
printf("width %d", actual_args->width);
    char *imagen1 = actual_args->img1;
    char *imagen2 = actual_args->img2;
    char *mask = actual_args->mascara;
    char *result = actual_args->result;


    int width = actual_args->width;//atoi(argv[4]);
    int heigth = actual_args->heigth;//atoi(argv[5]);
    int colorsxPixel = 3; 
    int dimensions = width * heigth * colorsxPixel;

          printf(" \n  inicio %d", width);


    // memory reservation
    unsigned char *maskData = malloc(dimensions);
    unsigned char *img1Data = malloc(dimensions);  
    unsigned char *img2Data = malloc(dimensions);
 
    int offset1 = loadFile(imagen1, dimensions, img1Data);
    int offset2 = loadFile(imagen2, dimensions, img2Data);
    int offset3 = loadFile(mask, dimensions, maskData); 

    printf("entrando a enmascarar ");

    //enmascarar_c(img1Data, img2Data, maskData, width, heigth);
    CMAIN((img1Data+offset1), (img2Data+offset2), (maskData+offset3), dimensions);   


    saveFile(result, dimensions, img1Data);

    free(maskData);
    free(img1Data);
    free(img2Data);
    free(actual_args);

   // return 0;

}
int main()
{
    pthread_t hilo;
    pthread_t hilo2;
    clock_t t_ini, t_fin;
    double secs = 0.0;

     parameter_struct *args = malloc(sizeof *args);
     args->img1 = "image1.bmp";
     args->img2 = "image2.bmp";
     args->mascara = "mask3.bmp";
     args->width = 970;
     args->heigth = 518;
     args->result = "result.bmp";

     parameter_struct *args2 = malloc(sizeof *args2);
     args->img1 = "img1.bmp";
     args->img2 = "img2.bmp";
     args->mascara = "mask.bmp";
     args->width = 500;
     args->heigth = 490;
     args->result = "result2.bmp";

printf("args: %d",args->width);

 t_ini = clock();
 
    if(pthread_create(&hilo, NULL, principal,  (void *)args)){
        free(args);
    }
    if(pthread_create(&hilo2, NULL, principal,  (void *)args2)){
        free(args2);
    }

    //principal("image1.bmp", "image2.bmp", "mask3.bmp", 970, 518, "result.bmp");
    //principal("img1.bmp", "img2.bmp", "mask.bmp", 500, 490, "result2.bmp");


    pthread_join(hilo, NULL);
    pthread_join(hilo2, NULL);

  t_fin = clock();
   
 double inicio = (double) t_ini;
  double fin = (double) t_fin; 
  printf(" \n  inicio %.4f", inicio);
    printf(" \n  fin %.4f", fin);


  secs += (double)(t_fin - t_ini) / CLOCKS_PER_SEC;
  float secs1 = secs * 1000.0;
  printf(" \n  milisegundos %.4f", secs1);

    return 0;
}