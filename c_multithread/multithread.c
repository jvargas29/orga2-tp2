#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>

void loadFile(char *fileName, int dimensions, unsigned char *buffer){
    FILE *file;
    file = fopen(fileName, "r");
    if(file == NULL){
        printf("Error loading file\n");
    } else {
        fread(buffer, dimensions, 1, file);
    }
    fclose(file);
}

void saveFile(char *fileName, int dimensions, unsigned char *buffer){
    FILE *file;
    file = fopen(fileName, "w");
    fwrite(buffer, 1, dimensions, file);
    fclose(file);
}

void enmascarar_c(unsigned char *img1Data, unsigned char *img2Data, unsigned char *maskData, int width, int heigth){

    int y,x;
    int r,g,b;

    for(x = 0; x < heigth; x++)
    {
        for(y = 0; y < width; y++)
        {       
            b = maskData[3 * (y + x * width)];
            g = maskData[3 * (y + x * width) + 1];
            r = maskData[3 * (y + x * width) + 2];

            if(b == 0 && g== 0 && r == 0){
                img1Data[3 * (y + x * width)] = img2Data[3 * (y + x * width)];
                img1Data[3 * (y + x * width)+1] = img2Data[3 * (y + x * width)+1];
                img1Data[3 * (y + x * width)+2] = img2Data[3 * (y + x * width)+2];
            }
        }    
    }

}

void generateMaskedImage(char *image1, char *image2, char *mask, int width, int heigth, char *resultFileName){

   // width = atoi(argv[4]);
    //heigth = atoi(argv[5]);
    int colorsxPixel = 3; 
    int dimensions = width * heigth * colorsxPixel;

    // memory reservation
    unsigned char *data_mask = malloc(dimensions);
    unsigned char *data_img1 = malloc(dimensions);  
    unsigned char *data_img2 = malloc(dimensions);
 
    loadFile(image1, dimensions, data_img1);
    loadFile(image2, dimensions, data_img2);
    loadFile(mask, dimensions, data_mask); 
 
    enmascarar_c(data_img1, data_img2, data_mask, width, heigth);

    saveFile(resultFileName, dimensions, data_img1);

    free(data_mask);
    free(data_img1);
    free(data_img2);

}

int main(int arg, char *argv[])
{
    DIR* dir;
    FILE *entry_file;
    struct dirent *in_file;
    char files[1000];
    int i;

    //params
   /*  char *imagen1 = argv[1];
    char *imagen2 = argv[2];
    char *mask = argv[3]; */

    char actualDir[PATH_MAX];
    getcwd(actualDir, sizeof(actualDir));

    dir = opendir(actualDir);

    if(dir==NULL) {
        printf("Error! Unable to read directory");
        exit(1);
    }

    int countSet1 = 0; 
    int countSet2 = 0; 
    int countSet3 = 0; 
    int countSet4 = 0; 

    char *img1 = NULL;
    char *img2 = NULL;
    char *mask = NULL;

    char *pic1 = NULL;
    char *pic2 = NULL;
    char *mask2 = NULL;

    char *image1 = NULL;
    char *image2 = NULL;
    char *mask3 = NULL;    

    char *photo1 = NULL;
    char *photo2 = NULL;
    char *mask4 = NULL;    

    while((in_file=readdir(dir)) != NULL) {
        
        if (!strcmp (in_file->d_name, "."))
            continue;
        if (!strcmp (in_file->d_name, ".."))
            continue;
        entry_file = fopen(in_file->d_name, "r");
        if (entry_file != NULL) {

            if (strcmp(in_file->d_name,"img1.bmp") == 0 || strcmp(in_file->d_name,"img2.bmp") == 0 || strcmp(in_file->d_name,"mask.bmp") == 0 )
            {
                if(strcmp(in_file->d_name,"mask.bmp") == 0){
                    mask = in_file->d_name;
                    countSet1++;
                }else if(img1 == NULL){
                    img1=in_file->d_name;
                    countSet1++;
                } else
                {
                    img2 = in_file->d_name;
                    countSet1++;   
                }
                if (countSet1 == 3)
                {   
                    generateMaskedImage(img1, img2, mask, 500, 490, "result.bmp");
                }
            }

            if (strcmp(in_file->d_name,"image1.bmp") == 0 || strcmp(in_file->d_name,"image2.bmp") == 0 || strcmp(in_file->d_name,"mask3.bmp") == 0 )
            {
                if(strcmp(in_file->d_name,"mask3.bmp") == 0){
                    mask3 = in_file->d_name;
                    countSet2++;
                }else if(image1 == NULL){
                    image1=in_file->d_name;
                    countSet2++;
                } else
                {
                    image2 = in_file->d_name;
                    countSet2++;
                }

                if (countSet2 == 3)
                {
                    generateMaskedImage(image1, image2, mask3, 900, 512, "result2.bmp");
                }
            }

            if (strcmp(in_file->d_name,"pic1.bmp") == 0 || strcmp(in_file->d_name,"pic2.bmp") == 0 || strcmp(in_file->d_name,"mask2.bmp") == 0 )
            {
                if(strcmp(in_file->d_name,"mask2.bmp") == 0){
                    mask2 = in_file->d_name;
                    countSet3++;
                }else if(pic1 == NULL){
                    pic1 = in_file->d_name;
                    countSet3++;
                } else
                {
                    pic2 = in_file->d_name;
                    countSet3++;
                }

                if (countSet3 == 3)
                {
                    generateMaskedImage(pic1, pic2, mask2, 800, 600, "result3.bmp");
                }
            } 

            if (strcmp(in_file->d_name,"photo1.bmp") == 0 || strcmp(in_file->d_name,"photo2.bmp") == 0 || strcmp(in_file->d_name,"mask4.bmp") == 0 )
            {
                if(strcmp(in_file->d_name,"mask4.bmp") == 0){
                    mask4 = in_file->d_name;
                    countSet4++;
                }else if(photo1 == NULL){
                    photo1 = in_file->d_name;
                    countSet4++;
                } else
                {
                    photo2 = in_file->d_name;
                    countSet4++;
                }

                if (countSet4 == 3)
                {
                    generateMaskedImage(photo1, photo2, mask4, 1280, 960, "result4.bmp");
                }
            }

            fclose(entry_file);

        }
    }

    closedir(dir);

    return 0;
}