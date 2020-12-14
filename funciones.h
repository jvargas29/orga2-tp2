
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
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

void *principal(void *args)
{
  struct parameter_struct *actual_args;
  actual_args = (struct parameter_struct *)args;

  char *imagen1 = actual_args->img1;
  char *imagen2 = actual_args->img2;
  char *mask = actual_args->mascara;
  char *result = actual_args->result;

  int width = actual_args->width;
  int heigth = actual_args->heigth;
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

  saveFile(result, dimensions, img1Data);

  free(maskData);
  free(img1Data);
  free(img2Data);
}

void *principal_asm(void *args)
{
  struct parameter_struct *actual_args;
  actual_args = (struct parameter_struct *)args;

  char *imagen1 = actual_args->img1;
  char *imagen2 = actual_args->img2;
  char *mask = actual_args->mascara;
  char *result = actual_args->result;

  int width = actual_args->width;
  int heigth = actual_args->heigth;
  int colorsxPixel = 3; 
  int dimensions = width * heigth * colorsxPixel;

  // memory reservation
  unsigned char *maskData = malloc(dimensions);
  unsigned char *img1Data = malloc(dimensions);  
  unsigned char *img2Data = malloc(dimensions);
 
  int offset1 = loadFile(imagen1, dimensions, img1Data);
  int offset2 = loadFile(imagen2, dimensions, img2Data);
  int offset3 = loadFile(mask, dimensions, maskData); 

  CMAIN(img1Data, img2Data, maskData, dimensions);

  saveFile(result, dimensions, img1Data);

  free(maskData);
  free(img1Data);
  free(img2Data);
}