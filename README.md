# P2_SSOOII_HYR

## Práctica 2 - Programación Multihilo - Buscador SSOOIIGLE

Práctica realizada por los alumnos de Sistemas Operativos II de tercer curso Rubén Crespo Calcerrada y Héctor Alberca Sánchez-Quintanar, el objetivo de esta práctica es realizar un buscador de información (palabras) a través de la programación con múltiples hilos. El fichero se dividirá en tantas partes como hilos se indiquen. 

### Archivos de la práctica. 
  * Manager.cpp: código llevado a cabo en el lenguaje C++, cuyo propósito es ser el programa que gestiona los hilos, los crea, los pone en ejecución y los espera hasta que finalicen para completar su ejecución. Cada uno de estos hilos hará la búsqueda de la palabra indicada dentro de su rango de líneas. 
  * My_lib.h: archivo en el cual encontraremos todas las funciones genéricas, es decir, todas aquellas funciones que podrían ser utilizadas en cualquier otra práctica, ya que no son específicas para estar. 
  * Thread_struct.h: archivo con las diferentes estructuras que se van a utilizar a lo largo de la práctica. Hay dos estructuras en este caso, la de resultados, que se creará en el momento en que encontremos en el archivo una palabra igual a la que buscamos y una estructura de hilos, perteneciente respectivamente a cada uno de los hilos creados en el código, dentro de esta estructura tendremos un vector de las primeras estructuras con todas las soluciones encontradas.   
  

### Modo de Ejecución. 

Para ejecutar este código se ha llevado a cabo un Makefile para la realización más sencilla. Lo primero que debemos hacer es ejecutar el comando "make" dentro del directorio, de esta manera se compilarán todos los archivos necesarios para su ejecución, si hacemos algún cambio también debemos hacerlo para actualizar la compilación. Después, se lleva a cabo la ejecución de la instrucción "make test", así se llevará a cabo la ejecución del programa con los argumentos que tenemos dentro del makefile en el apartado de test. Si deseamos usarlo con diferentes argumentos, solamente debemos cambiar los argumentos en el make test. 
