#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include <stdbool.h>
#include <time.h>

struct parameters {
    
    char *img1;
    char *img2;
    char *mask;

    int widthSet1;
     int heigthSet1;

    char *pic1 ;
    char *pic2 ;
    char *mask2;

    int widthSet2;
     int heigthSet2;

    char *image1;
    char *image2;
    char *mask3 ;    

    int widthSet3;
    int heigthSet3;

    char *photo1;
    char *photo2;
    char *mask4 ;  

    int widthSet4;
    int heigthSet4;

    bool set1;
    bool set2;
    bool set3;
    bool set4;

    bool threada_created;
    bool threadb_created;

};

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

    pthread_t hilo;

    free(data_mask);
    free(data_img1);
    free(data_img2);

}

void *thread_function(void *arg){
     struct parameters *p;
    p = (struct parameters *) arg;
 
    p->widthSet1 = 500;
    p->heigthSet1 = 490; 

    p->widthSet2 = 900;
    p->heigthSet2 = 512;

    p->widthSet3 = 800;
    p->heigthSet3 = 600;

    p->widthSet4 = 1280;
    p->heigthSet4 = 960;

    if(p->set1 && p->set2 ){
        generateMaskedImage(p->img1,p->img2,p->mask,p->widthSet1,p->widthSet2,"resultSet1.bmp");
        generateMaskedImage(p->image1,p->image2,p->mask3,p->widthSet2,p->heigthSet2,"resultSet2.bmp");
    }

    if (p->set3 && p->set4 )
    {
        generateMaskedImage(p->pic1,p->pic2,p->mask2,p->widthSet3,p->heigthSet3,"resultSet3.bmp");  
        generateMaskedImage(p->photo1,p->photo2,p->mask4,p->widthSet4,p->heigthSet4,"resultSet4.bmp");   
    }
}

int main(int arg, char *argv[])
{
    clock_t start_time, end_time;
    double seconds = 0.0;
    start_time = clock();

    DIR* dir;
    FILE *entry_file;
    pthread_t threada;
    pthread_t threadb;
    struct dirent *in_file;
    char files[1000];
    int i;

    struct parameters params;

    int countSet1 = 0; 
    int countSet2 = 0; 
    int countSet3 = 0; 
    int countSet4 = 0; 

    params.threada_created = false;
    params.threadb_created = false;

    char actualDir[PATH_MAX];
    getcwd(actualDir, sizeof(actualDir));

    dir = opendir(actualDir);

    if(dir==NULL) {
        printf("Error! Unable to read directory");
        exit(1);
    }

    params.img1 = NULL;
    params.img2 = NULL;
    params.mask = NULL;

    params.image1 = NULL;
    params.image2 = NULL;
    params.mask3 = NULL;

    params.pic1 = NULL;
    params.pic2 = NULL;
    params.mask2 = NULL;

    params.photo1 = NULL;
    params.photo2 = NULL;
    params.mask4 = NULL;

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
                    params.mask = in_file->d_name;
                    countSet1++;
                }else if(params.img1 == NULL){
                    params.img1=in_file->d_name;
                    countSet1++;
                } else
                {
                    params.img2 = in_file->d_name;
                    countSet1++;   
                }
                if (countSet1 == 3)
                {
                    params.set1 = true;
                }
            }

            if (strcmp(in_file->d_name,"image1.bmp") == 0 || strcmp(in_file->d_name,"image2.bmp") == 0 || strcmp(in_file->d_name,"mask3.bmp") == 0 )
            {
                if(strcmp(in_file->d_name,"mask3.bmp") == 0){
                    params.mask3 = in_file->d_name;
                    countSet2++;
                }else if(params.image1 == NULL){
                    params.image1=in_file->d_name;
                    countSet2++;
                } else
                {
                    params.image2 = in_file->d_name;
                    countSet2++;
                }

                if (countSet2==3)
                {
                    params.set2 = true;
                }
            }

            if (strcmp(in_file->d_name,"pic1.bmp") == 0 || strcmp(in_file->d_name,"pic2.bmp") == 0 || strcmp(in_file->d_name,"mask2.bmp") == 0 )
            {
                if(strcmp(in_file->d_name,"mask2.bmp") == 0){
                    params.mask2 = in_file->d_name;
                    countSet3++;
                }else if(params.pic1 == NULL){
                    params.pic1 = in_file->d_name;
                    countSet3++;
                } else
                {
                   params.pic2 = in_file->d_name;
                   countSet3++;
                }

                if (countSet3==true)
                {
                    params.set3=true;
                }
            } 

            if (strcmp(in_file->d_name,"photo1.bmp") == 0 || strcmp(in_file->d_name,"photo2.bmp") == 0 || strcmp(in_file->d_name,"mask4.bmp") == 0 )
            {
                if(strcmp(in_file->d_name,"mask4.bmp") == 0){
                    params.mask4 = in_file->d_name;
                    countSet4++;
                }else if(params.photo1 == NULL){
                    params.photo1 = in_file->d_name;
                    countSet4++;
                } else
                {
                    params.photo2 = in_file->d_name;
                    countSet4++;
                }

                if (countSet4==3)
                {
                    params.set4=true;
                }
            }

            if (countSet1 == 3 && countSet2 == 3 || countSet3 == 3 && countSet4 == 3)
            {
                if (countSet1 == 3 && countSet2 == 3 && params.threada_created == false)
                {
                    pthread_create(&threada, NULL, thread_function, (void *)&params);
                    params.threada_created = true;
                }
                
                if (countSet3 == 3 && countSet4 == 3 && params.threadb_created == false)
                {
                    pthread_create(&threadb, NULL, thread_function, (void *)&params);
                    params.threadb_created = true;
                }
            }

            fclose(entry_file);
        }

    }

    pthread_join(threada,NULL);
    pthread_join(threadb,NULL);
  
    closedir(dir);

    end_time = clock();

    seconds = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    printf("Time in miliseconds: %.16g\n", seconds * 1000.0);

    return 0;
}