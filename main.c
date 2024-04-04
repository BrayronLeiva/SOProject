#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>

#define N 5 // Número de automóviles
pthread_mutex_t mutex; //mutex para manejar acceso de puente
//char flag;
int carrosEnPuente = 0;
int carrosOaE = 0;
int carrosEaO = 0;


// Estructura para representar el puente
struct Puente{
    bool estado;
    char lado_actual[10];
    // Definir otras variables y estructuras de datos necesarias
};
struct Puente puente; //aca declaro mi puente

// Estructura para representar un automóvil
struct Automovil {
    int id;
    char sentido; // o 'hacia oeste', e hacia 'este'
    bool estado; //espera, cruzando, finalizado
    // Definir otras variables necesarias
};

// Función para que un automóvil cruce el puente
void cruzar_puente(struct Automovil *automovil) {
    printf("CRUZANDO PUENTE DESDE CARRO %d \n", automovil->id);
    printf("Sentido: %d \n ", automovil->sentido);
    automovil->estado=true; //ya cruzo

}

// Función que simula el comportamiento de un automóvil
void *comportamiento_automovil(void *arg) {
    struct Automovil *automovil = arg; //agarra de args su estructura de datos que es como su entidad
    pthread_t id_hilo = pthread_self(); // Obtener el identificador del hilo
    bool cont = true;
    struct Puente* miPuente = &puente;
    while (cont) {

        pthread_mutex_lock(&mutex);
        if(carrosEnPuente>0){miPuente->estado=true;}//puente ocupado
        if(carrosEnPuente==0){miPuente->estado=false;}//habilito el puente

        //|| automovil->estado=='o'&& carrosOaE>1 && carrosEaO==0 || automovil->estado =='e'&& carrosEaO>1 && carrosOaE==0
        if(!miPuente->estado || automovil->sentido=='o'&& carrosOaE>=1 && carrosEaO==0 || automovil->sentido =='e' && carrosEaO>=1 && carrosOaE==0){ //hay que implementar para que si los carros NO van en el mismo sentido se bloquee
            //ademas lo del carro puede ir en otro sitio ya que como ya cruzo el hilo podria ya ser eliminado o tal vez dependiente si el carro puede regresar???
            printf("El valor del booleano es: %s, ID del carro: %d\n", miPuente->estado ? "ocupado" : "libre",automovil->id, "\n");
            printf("carros en puente: %d, ID del carro: %d\n\n", carrosEnPuente , automovil->id );
            printf("carros en puente de este a oeste: %d, ID del carro: %d\n\n", carrosEaO , automovil->id );
            printf("carros en puente de oeste a este: %d , ID del carro: %d\n\n", carrosOaE , automovil->id );
            if(!automovil->estado) {

                carrosEnPuente++;
                if(automovil->sentido=='o'){carrosOaE++;} //viene del oeste va hacia el este
                if(automovil->sentido=='e'){carrosEaO++;} //viene del este va hacia el oeste
                pthread_mutex_unlock(&mutex);
                sleep(rand() % 5); // Simula el tiempo entre llegadas de automóviles
                cruzar_puente(automovil);


                printf("2: carros en puente: %d, ID del carro: %d\n \n", carrosEnPuente, automovil->id );
                printf("2: carros en puente de este a oeste: %d , ID del carro: %d\n\n", carrosEaO, automovil->id );
                printf("2: carros en puente de oeste a este: %d, ID del carro: %d\n\n", carrosOaE, automovil->id );
                printf("2: El valor del booleano es: %s, ID del carro: %d\n", miPuente->estado ? "ocupado" : "libre",  automovil->id, "\n");

                pthread_mutex_lock(&mutex);
                carrosEnPuente--;
                if(automovil->sentido=='o'){carrosOaE--;}
                if(automovil->sentido=='e'){carrosEaO--;}
                pthread_mutex_unlock(&mutex);
                printf("TERMINANDO CARRO HILO, desde carro %d\n", automovil->id);
                cont = false;
            }//else{ //VERIFICAR POR QUE ESTO NO SIRVIO
            //printf("CAYENDO A ELSE");
            //cont = false;
            //}//si ya cruzo bye bye

        }else{}
        pthread_mutex_unlock(&mutex);

        //if(carrosEnPuente==0){
        //if(flag=='o'){flag='e';}
        //if(flag=='e'){flag='o';}//}

    }//while
    printf("\t\t\t\t\t\tSaliendo de metodo\n\n");
    return NULL;
}

int main() {
    pthread_t threads[N];
    puente.estado = false; //comienza vacio

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


        //if (!ejecutado) {
        //para decir el primero que llega al puente
        //flag = automovil->sentido; // Actualiza el indicador para que no se vuelva a ejecutar
        //}

        pthread_create(&threads[i], NULL, comportamiento_automovil, (void *)automovil);
    }

    // Esperar a que todos los hilos terminen
    for (int i = 0; i < N; i++) {
        printf("HACIENDO JOIN A EL HILO %d \n\n", i);
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    return 0;
}