#include  <pthread.h>
#include  <stdio.h>
#include  <stdlib.h>
#include  <stdbool.h>
#include  <unistd.h>
#include  <time.h>

#define MAX_pila 20

bool  done = false; //bandera que indica que hemos acabado
int  numero = 0 ; //buffer contador simulacion
int  cont = 0 ; //contador buffer LIFO
int  pila[MAX_pila]= { 11 }; ; //buffer LIFO 
pthread_mutex_t  numero_mutex; // variable mutex
pthread_cond_t numero_cond; //variable condicional

#define  MSGLEN  32

struct  datos_hilo {
	int id ;
	char  message[MSGLEN];
};

void *productor(void *t){

	//int tid=*(int*)t;

	struct  datos_hilo  *pData = (struct  datos_hilo  *)t ;
	int tiempo_dormir_ms=1;

	printf("%s.\n Hilo_productor %d comienza.\n", pData ->message , pData ->id);

		while(!done){

			tiempo_dormir_ms=rand()% 1000 + 1;
			usleep(tiempo_dormir_ms*1000);
			pthread_mutex_lock (& numero_mutex );
			if (cont == MAX_pila){
				printf ("productor [%d]: pila ya al max\n", pData ->id);
			}
			else{

				
				numero=rand()% 10 + 1;
				pila[cont]=numero;
				cont++;
				pthread_cond_signal (&numero_cond );
				printf("productor [%d] introduce el numero = %d, numeros en pila: %d \n", pData ->id, numero,cont );
				/*prueba -------------------
					int i;
					for (i=0; i<MAX_pila; i++) {
        			 printf( "%d ;", pila[i] );
    				}
    				printf("\n");
    			//---------------------------------------*/
			}
			pthread_mutex_unlock (& numero_mutex );
		}

	printf("productor thread %d done.\n",pData ->id);
	pthread_exit(NULL);
}

void *consumidor(void *t){
		
	struct  datos_hilo  *pData = (struct  datos_hilo  *)t ;
	int tiempo_dormir_ms=1;
	int numero_f;
	printf("%s.\nHilo_consumidor %d comienza .\n", pData ->message , pData ->id);

	while(!done) {
		
		tiempo_dormir_ms=rand()% 1000 + 1;
		usleep(tiempo_dormir_ms*1000);
		pthread_mutex_lock (& numero_mutex );
		
		if (cont <= 0){
			
			printf ("consumidor[%d] esperando al buffer\n", pData ->id);
			pthread_cond_wait  (&numero_cond , &numero_mutex );
			printf ("consumidor[%d] dejo de esperar\n", pData ->id);
		}
		if (!done){
			
			numero=pila[cont-1];
			cont--;
			numero_f=factorial(numero);
			printf("consumidor [%d] extrae el numero = %d y calcula  %d != %i , numeros en pila: %d \n", pData ->id, numero,numero,numero_f,cont);
		}
		else{

			printf ("Consumidor[%d] Broadcast received with done flag set\n", pData ->id);
		}
		pthread_mutex_unlock (& numero_mutex );
	}

	printf("consumidor thread %d done.\n",pData ->id);
	pthread_exit(NULL);
}

int  main (int argc , char *argv []){
	
	if(argc<=2) {
        printf("introduce 2 argumentos por favor \n ");
        exit(1);
    } 
      //otherwise continue on our merry way....
    else{
			
		int n_prod = atoi(argv[1]);  //argv[0] is the program name
	    int n_cons= atoi(argv[2]);
	

		printf("productores: %i consumidores:%i \n",n_prod , n_cons);
						//prueba -------------------
		/*printf("pila LIFO:");
					int i;
					for (i=0; i<MAX_pila; i++) {
						pila[i]=0;
        			 printf( "%d ;", pila[i] );
    				}
    				printf("\n");
    			//---------------------------------------*/
		sleep(3);

		int thread_id [n_prod  + n_cons];
		pthread_t  thread[n_prod  + n_cons];
		pthread_attr_t  attr;
		int t;
		void *status;
		struct  datos_hilo  *pthreadData = malloc(sizeof(struct  datos_hilo ) *(n_prod + n_cons));
		
		if(!pthreadData) {
			fprintf(stderr , "Cannot allocate thread data!\r\n");
			return  255 ;
		}
		
		/*  Initialize  mutex  and  condition  variable  objects  */
		pthread_mutex_init (&numero_mutex , NULL);
		pthread_cond_init  (&numero_cond , NULL);
		/*  Initialize  and  set  thread  detached  attribute  */
		pthread_attr_init (&attr);
		pthread_attr_setdetachstate (&attr , PTHREAD_CREATE_JOINABLE );

		/* Spawn the threads */

		for(t=0; t<n_prod; t++) {
			
			pthreadData[t].id = t ;
			snprintf(pthreadData[t].message , MSGLEN ,
			"Welcome to productor %c", 'A'+t) ;
			pthread_create (& thread[t], &attr , productor, (void  *)& pthreadData[t]);
		}

		for(t=0; t<n_cons; t++) {
			
			pthreadData[t+n_prod].id = t+n_prod ;
			snprintf(pthreadData[t+n_prod].message , MSGLEN ,
			"Welcome to consumidor  %c", 'Z'-t) ;
			pthread_create (& thread[t+n_prod], &attr , consumidor,
			(void  *)& pthreadData[t+n_prod ]);
		}

		sleep (3); //30seg
		done = true ;

		/* Free  attribute  and  wait  for  the  other  threads  */

		for(t=0; t<n_prod+n_cons; t++)
			pthread_join(thread[t], &status );
		
		printf("Main: program completed. Exiting. numero = %d\n",numero );
		printf("Main: program completed. tamaño final pila = %d\n",cont );
		//printf("Main: program completed. Finish \n");
		
		/* Clean  up and  exit */
		if(pthreadData) free(pthreadData );
		pthread_attr_destroy (&attr);
		pthread_mutex_destroy (& numero_mutex );
		pthread_cond_destroy (& numero_cond );
		pthread_exit(NULL);
	}
}


int factorial(int n)
{
    if (n == 1||n==0)
        return 1;
    else
        return n * factorial(n - 1);
}