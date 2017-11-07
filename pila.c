//ALUMNA: MACARENA AMIGO, PADRON:98255, CORRECTOR: JORGE COLLINET, ENTREGA: PILA.

#include "pila.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define TAM_INICIAL 20
#define REDIMENSION_CRECER 2
#define REDIMENSION_ACHICAR 2
#define PORCENTAJE_UTILIZADO 4
/* Definición del struct pila proporcionado por la cátedra.
 */
struct pila {
    void **datos;
    size_t capacidad; // Cantidad de elementos almacenados.
    size_t cantidad;  // Capacidad del arreglo 'datos'.
};
/* *****************************************************************
 *                    Funciones auxiliares
 * *****************************************************************/

bool redimensionador(pila_t *pila, size_t tamanio){
	void* dats = realloc (pila -> datos,(tamanio) * sizeof(void*) );
	if (dats == NULL) return false;		
	pila -> datos = dats;
	pila -> capacidad = tamanio;
	return true;
}			

/* *****************************************************************
 *                    PRIMITIVAS DE LA PILA
 * *****************************************************************/

// Crea una pila.
// Post: devuelve una nueva pila vacía.
pila_t* pila_crear(){
	pila_t* pila = malloc (sizeof (pila_t) );
	if (pila == NULL) return NULL;
	pila -> datos = malloc (sizeof(void*) * TAM_INICIAL );
	if (pila -> datos == NULL) {
		free (pila);	
		return NULL;
	}
	pila -> capacidad = TAM_INICIAL;
	pila -> cantidad = 0;	
	return pila;
}

// Destruye la pila.
// Pre: la pila fue creada.
// Post: se eliminaron todos los elementos de la pila.
void pila_destruir(pila_t *pila){
	free (pila -> datos);
	free (pila);
}

// Devuelve verdadero o falso, según si la pila tiene o no elementos apilados.
// Pre: la pila fue creada.
bool pila_esta_vacia(const pila_t *pila){
	return pila -> cantidad == 0;
}

// Agrega un nuevo elemento a la pila. Devuelve falso en caso de error.
// Pre: la pila fue creada.
// Post: se agregó un nuevo elemento a la pila, valor es el nuevo tope.
bool pila_apilar(pila_t *pila, void* valor){
	if ( (pila -> cantidad) == pila -> capacidad){
		bool estado_redimension = (redimensionador(pila,pila->capacidad*REDIMENSION_CRECER));
		if (!estado_redimension) return false;
	}
	pila->datos [pila->cantidad] = valor;
	pila->cantidad ++;
	return true;
}

// Obtiene el valor del tope de la pila. Si la pila tiene elementos,
// se devuelve el valor del tope. Si está vacía devuelve NULL.
// Pre: la pila fue creada.
// Post: se devolvió el valor del tope de la pila, cuando la pila no está
// vacía, NULL en caso contrario.
void* pila_ver_tope(const pila_t *pila){
	if (pila_esta_vacia(pila)) return NULL;
	return (pila -> datos[(pila -> cantidad)-1]);
}

// Saca el elemento tope de la pila. Si la pila tiene elementos, se quita el
// tope de la pila, y se devuelve ese valor. Si la pila está vacía, devuelve
// NULL.
// Pre: la pila fue creada.
// Post: si la pila no estaba vacía, se devuelve el valor del tope anterior
// y la pila contiene un elemento menos.
void* pila_desapilar(pila_t *pila){
	if (pila_esta_vacia(pila)) return NULL;
	if (pila -> cantidad > TAM_INICIAL && pila -> cantidad < pila -> capacidad/PORCENTAJE_UTILIZADO){
		redimensionador(pila,pila -> capacidad/REDIMENSION_CRECER);
	}
	void* tope = pila -> datos[(pila -> cantidad)-1];
	pila -> cantidad--;	
	return tope;
}

