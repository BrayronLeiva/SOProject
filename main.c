#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>




/*
 *
 * //MODO DE TRAFICO

#define N 5 // Número de automóviles
#define LARGO_PUENTE 5 // Longitud del puente
#define K_OaE 5
#define K_EaO 5

pthread_mutex_t mutexes[LARGO_PUENTE]; // Array de mutex para representar el puente
pthread_mutex_t mutex; //mutex para manejar acceso de puente
pthread_cond_t cond_cruzar; //condición para que los automóviles esperen a cruzar el puente
int carrosEnPuente = 0;
int carrosOaE = 0;
int carrosEaO = 0;
int traficoOaE = 0;
int traficoEaO = 0;

int turno = 0; // 0 para los que van de este a oeste y 1 al reves

// Estructura para representar un automóvil
struct Automovil {
    int id;
    char sentido; // o 'hacia oeste', e hacia 'este'
    bool estado; //espera, cruzando, finalizado
};

// Función para que un automóvil cruce el puente
void cruzar_puente(struct Automovil *automovil) {
    printf("SE CRUZO PUENTE DESDE CARRO %d \n", automovil->id);
    printf("Sentido: %d \n ", automovil->sentido);
    automovil->estado=true; //ya cruzo

}

// Función que simula el comportamiento de un automóvil
void *comportamiento_automovil(void *arg) {
    struct Automovil *automovil = arg; //agarra de args su estructura de datos que es como su entidad
    pthread_t id_hilo = pthread_self(); // Obtener el identificador del hilo
    bool cont = true;

    pthread_mutex_lock(&mutex);

        //|| automovil->estado=='o'&& carrosOaE>1 && carrosEaO==0 || automovil->estado =='e'&& carrosEaO>1 && carrosOaE==0
    while (traficoEaO > K_EaO && turno==0 && automovil->sentido=="e" || traficoOaE > K_OaE && turno==1) { //no va = por que traficos empiezan en 0
        // Esperar a que el puente esté disponible o haya espacio en el sentido adecuado
        //printf("E");
        pthread_cond_wait(&cond_cruzar, &mutex); //razon de problema de arreglo de mutexes verificar si condicion debe ser distinta
        if (turno==0){
            traficoEaO = 0;
            turno = 1;
        }else{
            traficoOaE = 0;
            turno = 0;
        }
    }
                if(automovil->sentido=='o'){traficoOaE++;} //viene del oeste va hacia el este
                if(automovil->sentido=='e'){traficoEaO++;} //viene del este va hacia el oeste


                carrosEnPuente++;
                if(automovil->sentido=='o'){carrosOaE++;} //viene del oeste va hacia el este
                if(automovil->sentido=='e'){carrosEaO++;} //viene del este va hacia el oeste
                pthread_mutex_unlock(&mutex);

                sleep(rand() % 5); // Simula el tiempo entre llegadas de automóviles



                cruzar_puente(automovil);

                printf("2: carros en puente: %d, ID del carro: %d\n \n", carrosEnPuente, automovil->id );
                printf("2: carros en puente de este a oeste: %d , ID del carro: %d\n\n", carrosEaO, automovil->id );
                printf("2: carros en puente de oeste a este: %d, ID del carro: %d\n\n", carrosOaE, automovil->id );

                pthread_mutex_lock(&mutex);//razon de problema con arreglo de mutexes
                carrosEnPuente--;
                if(automovil->sentido=='o'){carrosOaE--;}
                if(automovil->sentido=='e'){carrosEaO--;}
                pthread_cond_signal(&cond_cruzar); // Señalar a otros automóviles que puedan cruzar
                pthread_mutex_unlock(&mutex);

    //return NULL;
}

int main() {
    pthread_t threads[N];

    pthread_cond_init(&cond_cruzar, NULL); // Inicializar la condición para cruzar el puente

    pthread_mutex_init(&mutex, NULL);
    // Inicializar otras variables del puente

    // Crear hilos para los automóviles
    for (int i = 0; i < N; i++) {
        static int ejecutado = 0; // Variable estática como indicador

        struct Automovil *automovil = malloc(sizeof(automovil));
        automovil->id = i;
        automovil->estado = false;
        if(rand() % 2 == 0){
            automovil->sentido = 'e';
        }else{automovil->sentido = 'o';}

        printf("=============================CARRO SENTIDO : %c, ID del carro: %d\n\n", automovil->sentido , automovil->id );

        pthread_create(&threads[i], NULL, comportamiento_automovil, (void *)automovil);
    }

    // Esperar a que todos los hilos terminen
    for (int i = 0; i < N; i++) {
        printf("HACIENDO JOIN A EL HILO %d \n\n", i);
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    // Destruir los mutex del puente
    for (int i = 0; i < LARGO_PUENTE; i++) {
        pthread_mutex_destroy(&mutexes[i]);
    }
    pthread_cond_destroy(&cond_cruzar); // Destruir la condición para cruzar el puente
    return 0;
}


*/




//CARNAGE



#define N 5 // Número de automóviles
#define LARGO_PUENTE 5 // Longitud del puente
pthread_mutex_t mutexes[LARGO_PUENTE]; // Array de mutex para representar el puente
pthread_mutex_t mutex; //mutex para manejar acceso de puente
pthread_cond_t cond_cruzar; //condición para que los automóviles esperen a cruzar el puente
int carrosEnPuente = 0;
int carrosOaE = 0;
int carrosEaO = 0;


// Estructura para representar el puente
struct Puente{
    bool estado;
    char lado_actual[10];
};
struct Puente puente; //aca declaro mi puente

// Estructura para representar un automóvil
struct Automovil {
    int id;
    char sentido; // o 'hacia oeste', e hacia 'este'
    bool estado; //espera, cruzando, finalizado
};

// Función para que un automóvil cruce el puente
void cruzar_puente(struct Automovil *automovil) {
    printf("SE CRUZO PUENTE DESDE CARRO %d \n", automovil->id);
    printf("Sentido: %d \n ", automovil->sentido);
    automovil->estado=true; //ya cruzo

}

// Función que simula el comportamiento de un automóvil
void *comportamiento_automovil(void *arg) {
    struct Automovil *automovil = arg; //agarra de args su estructura de datos que es como su entidad
    pthread_t id_hilo = pthread_self(); // Obtener el identificador del hilo
    bool cont = true;
    struct Puente* miPuente = &puente;

        pthread_mutex_lock(&mutex);

        //|| automovil->estado=='o'&& carrosOaE>1 && carrosEaO==0 || automovil->estado =='e'&& carrosEaO>1 && carrosOaE==0
    while (carrosEnPuente > 0 && (automovil->sentido == 'o' && carrosOaE == 0 && carrosEaO >= 1) || carrosEnPuente > 0 && (automovil->sentido == 'e' && carrosEaO == 0 && carrosOaE >= 1)) {
        // Esperar a que el puente esté disponible o haya espacio en el sentido adecuado
        printf("E");
        pthread_cond_wait(&cond_cruzar, &mutex);
    }
                carrosEnPuente++;
                if(automovil->sentido=='o'){carrosOaE++;} //viene del oeste va hacia el este
                if(automovil->sentido=='e'){carrosEaO++;} //viene del este va hacia el oeste
                pthread_mutex_unlock(&mutex);

                sleep(rand() % 5); // Simula el tiempo entre llegadas de automóviles


                int inicio, fin;
                if (automovil->sentido == 'o') {
                    inicio = 0;
                    fin = LARGO_PUENTE - 1;
                } else {
                    inicio = LARGO_PUENTE - 1;
                    fin = 0;
                }// Cruzar el puente
                for (int i = inicio; i != fin; i += (inicio < fin) ? 1 : -1) { //si voy hacia el este voy sumando y sino voy al oeste restando
                    pthread_mutex_lock(&mutexes[i]); // Adquirir el mutex del segmento del puente
                    if (i != inicio) {
                        pthread_mutex_unlock(&mutexes[i - ((inicio < fin) ? 1 : -1)]); // Liberar el mutex del segmento anterior
                    }
                }
                cruzar_puente(automovil);

                printf("2: carros en puente: %d, ID del carro: %d\n \n", carrosEnPuente, automovil->id );
                printf("2: carros en puente de este a oeste: %d , ID del carro: %d\n\n", carrosEaO, automovil->id );
                printf("2: carros en puente de oeste a este: %d, ID del carro: %d\n\n", carrosOaE, automovil->id );

                pthread_mutex_lock(&mutex);
                carrosEnPuente--;
                if(automovil->sentido=='o'){carrosOaE--;}
                if(automovil->sentido=='e'){carrosEaO--;}
                pthread_cond_broadcast(&cond_cruzar); // Señalar a otros automóviles que puedan cruzar
                pthread_mutex_unlock(&mutex);

    //return NULL;
}

int main() {
    pthread_t threads[N];
    puente.estado = false; //comienza vacio
    pthread_cond_init(&cond_cruzar, NULL); // Inicializar la condición para cruzar el puente

    pthread_mutex_init(&mutex, NULL);

    // Inicializar mutexes del puente
    for (int i = 0; i < LARGO_PUENTE; i++) {
        pthread_mutex_init(&mutexes[i], NULL);
    }

    // Inicializar otras variables del puente

    // Crear hilos para los automóviles
    for (int i = 0; i < N; i++) {
        static int ejecutado = 0; // Variable estática como indicador

        struct Automovil *automovil = malloc(sizeof(automovil));
        automovil->id = i;
        automovil->estado = false;
        if(rand() % 2 == 0){
            automovil->sentido = 'e';
        }else{automovil->sentido = 'o';}

        printf("=============================CARRO SENTIDO : %c, ID del carro: %d\n\n", automovil->sentido , automovil->id );

        pthread_create(&threads[i], NULL, comportamiento_automovil, (void *)automovil);
    }

    // Esperar a que todos los hilos terminen
    for (int i = 0; i < N; i++) {
        printf("HACIENDO JOIN A EL HILO %d \n\n", i);
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    // Destruir los mutex del puente
    for (int i = 0; i < LARGO_PUENTE; i++) {
        pthread_mutex_destroy(&mutexes[i]);
    }
    pthread_cond_destroy(&cond_cruzar); // Destruir la condición para cruzar el puente
    return 0;
}
//2.0-------------------------------------------------------------------

/*


 #include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>



#define N 5 // Número de automóviles
#define LARGO_PUENTE 5 // Longitud del puente
pthread_mutex_t mutexes[LARGO_PUENTE]; // Array de mutex para representar el puente
pthread_mutex_t mutex; //mutex para manejar acceso de puente
pthread_cond_t cond_cruzar; //condición para que los automóviles esperen a cruzar el puente
int carrosEnPuente = 0;
int carrosOaE = 0;
int carrosEaO = 0;

// Estructura para representar un automóvil
struct Automovil {
    int id;
    char sentido; // o 'hacia oeste', e hacia 'este'
    bool estado; //espera, cruzando, finalizado
};

// Función para que un automóvil cruce el puente
void cruzar_puente(struct Automovil *automovil) {
    printf("SE CRUZO PUENTE DESDE CARRO %d \n", automovil->id);
    printf("Sentido: %d \n ", automovil->sentido);
    automovil->estado=true; //ya cruzo

}

// Función que simula el comportamiento de un automóvil
void *comportamiento_automovil(void *arg) {
    struct Automovil *automovil = arg; //agarra de args su estructura de datos que es como su entidad
    pthread_t id_hilo = pthread_self(); // Obtener el identificador del hilo
    bool cont = true;

        pthread_mutex_lock(&mutex);

        //|| automovil->estado=='o'&& carrosOaE>1 && carrosEaO==0 || automovil->estado =='e'&& carrosEaO>1 && carrosOaE==0
    while (carrosEnPuente > 0 && (automovil->sentido == 'o' && carrosOaE == 0 && carrosEaO >= 1) || carrosEnPuente > 0 && (automovil->sentido == 'e' && carrosEaO == 0 && carrosOaE >= 1)) {
        // Esperar a que el puente esté disponible o haya espacio en el sentido adecuado
        printf("E");
        pthread_cond_wait(&cond_cruzar, &mutex);
    }
                carrosEnPuente++;
                if(automovil->sentido=='o'){carrosOaE++;} //viene del oeste va hacia el este
                if(automovil->sentido=='e'){carrosEaO++;} //viene del este va hacia el oeste
                pthread_mutex_unlock(&mutex);

                sleep(rand() % 5); // Simula el tiempo entre llegadas de automóviles


                int inicio, fin;
                if (automovil->sentido == 'o') {
                    inicio = 0;
                    fin = LARGO_PUENTE - 1;
                } else {
                    inicio = LARGO_PUENTE - 1;
                    fin = 0;
                }// Cruzar el puente
                //for (int i = inicio; i != fin; i += (inicio < fin) ? 1 : -1) { //si voy hacia el este voy sumando y sino voy al oeste restando
                    //pthread_mutex_lock(&mutexes[i]); // Adquirir el mutex del segmento del puente
                    //if (i != inicio) {
                        //pthread_mutex_unlock(&mutexes[i - ((inicio < fin) ? 1 : -1)]); // Liberar el mutex del segmento anterior
                    //}
                //}
                cruzar_puente(automovil);

                printf("2: carros en puente: %d, ID del carro: %d\n \n", carrosEnPuente, automovil->id );
                printf("2: carros en puente de este a oeste: %d , ID del carro: %d\n\n", carrosEaO, automovil->id );
                printf("2: carros en puente de oeste a este: %d, ID del carro: %d\n\n", carrosOaE, automovil->id );

                pthread_mutex_lock(&mutex);
                carrosEnPuente--;
                if(automovil->sentido=='o'){carrosOaE--;}
                if(automovil->sentido=='e'){carrosEaO--;}
                pthread_cond_signal(&cond_cruzar); // Señalar a otros automóviles que puedan cruzar
                pthread_mutex_unlock(&mutex);

    //return NULL;
}

int main() {
    pthread_t threads[N];

    pthread_cond_init(&cond_cruzar, NULL); // Inicializar la condición para cruzar el puente

    pthread_mutex_init(&mutex, NULL);
    // Inicializar otras variables del puente

    // Crear hilos para los automóviles
    for (int i = 0; i < N; i++) {
        static int ejecutado = 0; // Variable estática como indicador

        struct Automovil *automovil = malloc(sizeof(automovil));
        automovil->id = i;
        automovil->estado = false;
        if(rand() % 2 == 0){
            automovil->sentido = 'e';
        }else{automovil->sentido = 'o';}

        printf("=============================CARRO SENTIDO : %c, ID del carro: %d\n\n", automovil->sentido , automovil->id );

        pthread_create(&threads[i], NULL, comportamiento_automovil, (void *)automovil);
    }

    // Esperar a que todos los hilos terminen
    for (int i = 0; i < N; i++) {
        printf("HACIENDO JOIN A EL HILO %d \n\n", i);
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    // Destruir los mutex del puente
    for (int i = 0; i < LARGO_PUENTE; i++) {
        pthread_mutex_destroy(&mutexes[i]);
    }
    pthread_cond_destroy(&cond_cruzar); // Destruir la condición para cruzar el puente
    return 0;
}


 */