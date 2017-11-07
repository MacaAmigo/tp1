//  ALUMNA: MACARENA AMIGO, PADRON:98255, CORRECTOR: JORGE COLLINET, ENTRAGA: TDA LISTA   //

#ifndef LISTA_H
#define LISTA_H
#include <stdbool.h>
#include <stdlib.h>

/* ******************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS
 * *****************************************************************/

/* La lista enlazada está planteada como una lista de punteros genéricos. */


typedef struct lista lista_t;
typedef struct lista_iter lista_iter_t;

/* ******************************************************************
 *                PRIMITIVAS BÁSICAS DE LA LISTA ENLAZADA
 * *****************************************************************/


// Crea una lista enlazada.
// Pos: Devuelve una nueva lista enlazada vacia.

lista_t *lista_crear(void);

// Devuelve verdadero o falso, según si la lista enlazada posee nodos enlazados.
// Pre: la lista enlazada fue creada.

bool lista_esta_vacia(const lista_t *lista);

// Inserta un nodo en la posicion primera de la lista enlazada.
// Pre: La lista enlazada fue creada.
// Pos: Devuelve un booleano de acuerdo a si se pudo insertar en dicho lugar o no.

bool lista_insertar_primero(lista_t *lista, void *dato);

// Inserta un nodo en la ultima posicion de la lista enlazada.
// Pre: La lista enlazada fue creada.
// Pos: Devuelve un booleano de acuerdo a si se pudo insertar en dicho lugar o no.

bool lista_insertar_ultimo(lista_t *lista, void *dato);

// Elimina el primer elemento de la lista enlazada. Si la lista enlazada posee elementos, se borrará el 
// que se encuentre en el primer lugar, devolviendo su valor, sino se devuelve NULL.
// Pre: la lista enlada fue creada.
// Pos: Devuelve el dato que existía en la primera posicion. La lista enlazada tiene menos elementos, si 
// no esta vacía.
void *lista_borrar_primero(lista_t *lista);

// Muestra el primer elemento de la lista enlazada. Si la lista enlazada tiene elementos, se devuelve el 
// valor de la primera posicion, si esta vacía devuelve NULL.
// Pre:la lista fue creada.
// Pos:Devuelve el dato que se encuenta en la primera posición, cuando no está vacía.
void *lista_ver_primero(const lista_t *lista);

// Muestra el ultimo elemento de la lista enlazada. Si la lista enlazada tiene elementos, se devuelve el 
// valor de la ultima posicion, si esta vacía devuelve NULL.
// Pre:la lista fue creada.
// Pos:Devuelve el dato que se encuenta en la ultima posición, cuando no está vacía.
void *lista_ver_ultimo(const lista_t *lista);

// Muestra el largo de la lista enlazada.
// Pre: La lista enlazada fue creada.
// Pos: devuelve el valor de la cantidad de elementos que posee la lista enlazada.

size_t lista_largo(const lista_t *lista);

// Destruye la lista enlazada.Si se recibe la función destruir_dato por parámetro,
// para cada uno de los elementos de la lista enlazada llama a destruir_dato.
// Pre: La lista enlazada fue creada. destruir_dato es una función capaz de destruir
// los datos de la lista enlazada, o NULL en caso de que no se la utilice.
// Post: se eliminaron todos los elementos de la lista enlazada.

void lista_destruir(lista_t *lista, void destruir_dato(void *));



/* ******************************************************************
 *                     PRIMITIVAS DE ITERACIÓN
 * *****************************************************************/

// Crea un iterador.
//Pos: devuelve unnuevo iterador.
lista_iter_t *lista_iter_crear(lista_t *lista);

//Avanza de a un nodo de la lista enlazada mientras que la lista enlazada tenga elementos.
//Pre: La lista enlazada y el iterador han sido creados.
//Pos: Devuelve un booleano si se pudo avanzar o no.

bool lista_iter_avanzar(lista_iter_t *iter);

//Muestra el valor del elemento en que se encuentra situado el iterador. Si la lista
//enlazada no se encuentra vacia, devuelve el valor, caso contrario devuelve NULL.
//Pre: El iterador ha sido creado.
//Pos:Devuelve el valor en que se encuentra el iterador, si la lista no está vacía.

void *lista_iter_ver_actual(const lista_iter_t *iter);

//Comprueba que el iterador se encuentre en la posicion final o no de la lista enlazada.
//Pre: La lista enlazada y el iterador han sido creados.
//Pos: Devuelve un booleano dependiendo si esta al final o no.

bool lista_iter_al_final(const lista_iter_t *iter);

//Destruye el iterador de lista enlazada.
//Pre: El iterador ha sido creado.
//Pos: El iterador ha sido destruido.

void lista_iter_destruir(lista_iter_t *iter);


//Inserta un elemento en la posicion en donde se encuentra el iterador en la lista enlazada.
//Pre: La lista enlazada e interador han sido creados.
//Pos: Devuelve un booleano si insertó o no.

bool lista_iter_insertar(lista_iter_t *iter, void *dato);


//Borra el elemento de la lista que se encuentre en la posicion del iterador.
//Si la no lista esta vacia, devuelve el valor, sino devuelve NULL.
//Pre: El iterador ha sido creado.
//Pos: Devuelve el valor, o NULL.

void *lista_iter_borrar(lista_iter_t *iter);



/* ******************************************************************
 *                 PRIMITIVAS DE ITERADOR INTERNO
 * *****************************************************************/

//Iterador interno. Funcion"visitar" recibe el dato y un puntero extra, y devuelve true si se debe 
//seguir iterando, false en caso contrario.
//Pre: Se creó el iterador.
//Pos: Recorrio toda la lista, llamando a la funcion visitar para cada elemento de la lista. 

void lista_iterar(lista_t *lista, bool (*visitar)(void *dato, void *extra), void *extra);


/* *****************************************************************
 *                      PRUEBAS UNITARIAS
 * *****************************************************************/

// Realiza pruebas sobre la implementación del alumno.

// Para la implementación de las pruebas se debe emplear la función
// print_test(), como se ha visto en TPs anteriores.

void pruebas_lista_alumno(void);


#endif // LISTA_H
