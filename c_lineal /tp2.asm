;%include "io.inc"

section .data
formato db "valor: %d", 10,13,0

mascara db 0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff
mascara2 db 1,2,3,4,5,6,7,8

img1 db 0
img2 db 0
masc db 0
mascaraBlanca db 0xff


section .text
global CMAIN
extern printf
CMAIN:
    ;mov ebp, esp; for correct debugging
    ;jmp fin

    push ebp ;enter 0,0
    mov ebp, esp; enter 0,0
    
    xor eax,eax
    xor ebx,ebx
    xor ecx,ecx
    xor edx,edx
    
    mov eax, [ebp +8];leer la direccion de memoria de la imagen 1
    mov edx, [ebp +12];leer la direccion de memoria de la imagen 2
    mov ebx, [ebp +16];leer la direccion de memoria de la mascara
    mov ecx, [ebp +20];leer la direccion de memoria de tamaño de imagen
    
     enmascarar:
    
   
     movq mm0, qword[ebx ]; leer 8 bytes de la mascara   
     movq mm2, qword[edx ]; leer 8 bytes de la imagen 2  
     movq mm1, qword[eax ]; leer 8 bytes de la imagen 1 
     movq mm3, qword[mascara] ;
          
               
     pand mm2,mm0; borro de la imagen 2 los canales de pixeles donde la mascara es negra
     pxor mm0,mm3; creo la negacion de la mascara
     pand mm1, mm0; borro de la imagen 1 los canales de pixeles donde la imagen es blanca
     por mm1, mm2; sumamos las dos imagenes alteradas y guardo el resultado en la imagen 1
     
     movq qword[eax],mm1; reemplaza los bytes de la imagen 1 modificados
    
     sub ecx,8
     cmp ecx,8
     JL resto
     
     add eax,8;muevo el puntero 8 bytes 
     add ebx,8
     add edx,8
     
     jmp enmascarar
     
     resto:
     jcxz final
      
     mov [img2],edx
     mov [masc],ebx
   
     and edx,[masc]
     xor ebx,[mascaraBlanca]
     and eax,ebx
     or eax,edx
     
     
     mov edx,[img2]
     mov ebx, [masc]
     

     add eax,1;muevo el puntero 1 byte
     add ebx,1
     add edx,1
     
    sub ecx,1

     
    jmp resto
    
   
     final:
                                
    mov ebp,esp ;leave
    pop ebp        ;leave
    ret