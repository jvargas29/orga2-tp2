#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include<pthread.h>
#include<sys/time.h>


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

pthread_mutex_t lock;


int loadFile(char *fileName, int dimensions, unsigned char *buffer){
     //pthread_mutex_lock(&lock);

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
   // pthread_mutex_unlock(&lock);

    return header.offset;
}


void saveFile(char *fileName, int dimensions, unsigned char *buffer){
    pthread_mutex_lock(&lock);

    FILE *file;
    file = fopen(fileName, "w");
    fwrite(buffer, 1, dimensions, file);
    fclose(file);
    pthread_mutex_unlock(&lock);

}

void enmascarar_c(unsigned char *img1pixels, unsigned char *img2pixels, unsigned char *maskPixels, int width, int heigth)
{
    int r,g,b;
 
    for(int x = 0; x < heigth* width; x++)
  {
      b = maskPixels[3 * x];
      g = maskPixels[(3 * x) + 1];
      r = maskPixels[(3 * x)+2];
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
    char *imagen1 = actual_args->img1;
    char *imagen2 = actual_args->img2;
    char *mask = actual_args->mascara;
    char *result = actual_args->result;


    int width = actual_args->width;//atoi(argv[4]);
    int heigth = actual_args->heigth;//atoi(argv[5]);
    int colorsxPixel = 3; 
    int dimensions = width * heigth * colorsxPixel;


    // memory reservation
    unsigned char *maskData = malloc(dimensions);
    unsigned char *img1Data = malloc(dimensions);  
    unsigned char *img2Data = malloc(dimensions);
 
    int offset1 = loadFile(imagen1, dimensions, img1Data);
    int offset2 = loadFile(imagen2, dimensions, img2Data);
    int offset3 = loadFile(mask, dimensions, maskData); 


    enmascarar_c(img1Data, img2Data, maskData, width, heigth);
    //CMAIN((img1Data+offset1), (img2Data+offset2), (maskData+offset3), dimensions);   


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
    pthread_t hilo3;
    clock_t t_ini, t_fin;
    double secs = 0.0;

     if (pthread_mutex_init(&lock, NULL) != 0)
    {
        printf("Mutex initialization failed.\n");
        return 1;
    }

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


 //t_ini = time (NULL);
     double sum = 0;
   struct timeval begin, end;
    gettimeofday(&begin, 0);
 
    if(pthread_create(&hilo, NULL, principal,  (void *)args)){
        //free(args);
    }
   if(pthread_create(&hilo2, NULL, principal,  (void *)args2)){
        //free(args2);
   }
   if(pthread_create(&hilo3, NULL, principal,  (void *)args3)){
       // free(args3);
   }
    pthread_join(hilo, NULL);
    pthread_join(hilo2, NULL);
    pthread_join(hilo3, NULL);

    pthread_mutex_destroy(&lock);
    gettimeofday(&end, NULL);

    long seconds = end.tv_sec - begin.tv_sec;
    long microseconds = end.tv_usec - begin.tv_usec;
    double elapsed = seconds + microseconds*1e-6;
        
    printf("Time measured: %.3f seconds.\n", elapsed);



    return 0;
}