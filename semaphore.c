#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/wait.h> 
#include <sys/syscall.h> 
#include <fcntl.h>

#define BUFFERSIZE 10

typedef char ruta[100];


pthread_t tid; 
pthread_attr_t attr; 
void * responderTarea();
void * calificador();
char * respuesta[100];

int ranking[10];


sem_t *fullRespuesta, *emptyRespuesta;

pthread_mutex_t mutex;
pthread_mutex_t mutexRespuesta;
int contador = 0;




int main(){

	pthread_attr_init(&attr);
	
	pthread_mutex_init(&mutex,NULL);
	fullRespuesta = sem_open("/fullRespuesta", O_CREAT, 0666, 0);
	emptyRespuesta = sem_open("/emptyRespuesta", O_CREAT, 0666, BUFFERSIZE);

	
	sem_init(emptyRespuesta,0,1);
	sem_init(fullRespuesta,0,0);

	

	char teclado[10];
	int participantes = 0;
	int i;

	printf("Ingresa el numero de participantes: \n");
	system( gets(teclado) );

	participantes = atoi(teclado);
	
	

	printf("Ingresaste: %d, comenzando la creacion de los hilos...\n", participantes);
	//sleep(1);
	

	/* Creacion de los hilos */
	for(i=0; i<participantes; i++){
		pthread_create(&tid,&attr,responderTarea, NULL);
	}

	for(i=0; i<participantes; i++){
		pthread_create(&tid,&attr,calificador, NULL);
	}
	
	sleep(1);


return 0;

}


/*

El calificador toma es un proceso que siempre esta activo a la espera de que haya algo que calificar.
Cuando hay algo que calificar el ejecuta su algoritmo de Fibonacci con unos parametros y compara su respuesta
con la respuesta obtenida con los mismos parametros al codigo que le envio el usuario.

*/
void * calificador(){
	//while(1){
		sem_wait(fullRespuesta);
				pthread_mutex_lock(&mutex);
					printf("=== Calificador ===\n");
					printf("Alumno: %d enviando tarea...\n",contador);
					contador++;
				pthread_mutex_unlock(&mutex);
		sem_post(emptyRespuesta);
	//}
}



void * responderTarea(){
	
	//while(1){
		sem_wait(emptyRespuesta);
			pthread_mutex_lock(&mutex);
				printf("=== Respondiendo tarea ===\n");
				printf("Alumno: %d enviando tarea...\n",contador);
				contador++;
			pthread_mutex_unlock(&mutex);
		sem_post(fullRespuesta);
	//}
}
