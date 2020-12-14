## Trabajo practico 2 SIMD Paralelismo 

### Introduccion

El trabajo consiste en implementar un programa que tome dos imagenes y una mascara en blanco y negro,y sus dimensiones que lo que haga sea combinar ambas imagenes utlizando la mascara para lograr la combinacion de ambas.

Se realizo una aplicacion en el lenguaje C que toma de una carpeta una serie de imagenes y comienza a enmascararlas bajo distintos tipos de funciones, con el objetivo de probar la perfomrance que se logra al utilizar las distintas tecnicas.

Estas tecnicas son: 

* Una ejecucion lineal en lenguaje C.
* Una ejecucion lineal en lenguaje assembler implementando instrucciones SIMD.
* Una ejecucion implementando paralelismo en C.

### ¿Como ejecutar el programa? 

Se debe descargar el projecto desde el siguiente repositorio de git: [enlace en línea](https://github.com/jvargas29/orga2-tp2.git) y ejecutar el shell script que se encargar de compilar y ejecutar los archivos.

Para ejecutar el script se debe posicionarse sobre la carpeta del projecto y ejecutar el comando: **sudo sh script_enmascarar.sh**.

![Clonamos el proyecto y lo ejecutamos](/capturas readme/captura1.png)

Como resultado, la aplicacion indica cuanto tiempo le tomo ejecutar la misma operacion con distintas tecnicas, generando las imagenes de un conjunto de 3 imagenes enmascaradas de un total de 8 imagenes simples y 3 mascaras utilizadas en total. 

Estas imagenes se encuentran dentro de la carpeta del projecto.

### Analisis de los resultados:

Al ejecutar el programa este nos indica el resultado de procesar las 8 imagenes con disintintas tecnicas, el tiempo es medido en funcion de tics del tiempo transcurrido del procesador.

Aunque los tiempos pueden variar en cada ejecucion es notable que el procesamientos lineal de imagenes en lenguaje assembler con instrucciones SIMD va a ser el tiempo mas optimo seguido del procesamiento con hilos en el lenguaje C y por ultimo el procesamiento lineal en lenguaje C.

Ejemplo de la ejecucion del programa: 

![Ejemplo](/capturas readme/captura2.png)

En el siguiente cuadro se puede notar con mas facilidad la diferencia en la perfomances de las disitntas tecnicas: 

![Grafico de medicion de tiempos de ejecucion](/capturas readme/captura3.png)

Como conclusion ejecutar tareas con instrucciones SIMD tiene una performance bastante alta, frente a lo que es un procesamiento lineal o secuencial de una sola tarea o a una implementacion con paralelismo, a su vez que una implementacion con paralelismo es mas rapido que una ejecucion lineal o secuencial.

### Aclaraciones

Las imagenes y dimensiones alto y ancho son en esta version del trabajo definidas dentro del codigo del programa, por lo que si se quiere agregar nuevas imagenes y variar los tamaños, se debera modificar el codigo del porgrama. 

El programa trabaja unicamente con imagenes del formato **.bmp**