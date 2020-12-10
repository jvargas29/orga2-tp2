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


int main(int arg, char *argv[])
{

   char *imagen1 = argv[1];
    char *imagen2 = argv[2];
    char *mask = argv[3];


    int width = atoi(argv[4]);
    int heigth = atoi(argv[5]);
    int colorsxPixel = 3; 
    int dimensions = width * heigth * colorsxPixel;

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


    saveFile("result.bmp", dimensions, img1Data);

    free(maskData);
    free(img1Data);
    free(img2Data);

    return 0;

}