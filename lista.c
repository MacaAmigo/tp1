//  ALUMNA: MACARENA AMIGO, PADRON:98255, CORRECTOR: JORGE COLLINET, ENTRAGA: TDA LISTA   //
#include "lista.h"
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
 

/* Declaracion de estructuras y Funciones Auxiliares*/

struct nodo{
	void* dato;
	struct nodo* prox;
};

typedef struct nodo nodo_t;

struct lista{
	nodo_t* prim;
	nodo_t* ult;
	size_t largo;
};
struct lista_iter{
	lista_t*  lista;	
	nodo_t* ant;
	nodo_t* act;
};

static nodo_t* crear_nodo (void* dato, nodo_t* proximo){
	nodo_t* nodo_creado = malloc (sizeof (nodo_t));
	if (nodo_creado != NULL){
		nodo_creado->dato = dato;
		nodo_creado->prox = proximo;
	}
	return nodo_creado;
}

/* *****************************************************************
 *                    PRIMITIVAS DE LA LISTA
 * *****************************************************************/

// Crea una lista enlazada.
// Pos: Devuelve una nueva lista enlazada vacia.

lista_t *lista_crear(void){
	lista_t* lista = malloc (sizeof (lista_t));
	if (lista == NULL) return NULL;
	lista->prim = NULL;
	lista->ult = NULL;
	lista->largo = 0;
	return lista;
}


// Devuelve verdadero o falso, según si la lista enlazada posee nodos enlazados.
// Pre: la lista enlazada fue creada.

bool lista_esta_vacia(const lista_t *lista){
	if(lista->largo == 0 && !lista->prim && !lista->ult) return true;
	return false;
}

// Inserta un nodo en la posicion primera de la lista enlazada.
// Pre: La lista enlazada fue creada.
// Pos: Devuelve un booleano de acuerdo a si se pudo insertar en dicho lugar o no.

bool lista_insertar_primero(lista_t *lista, void *dato){
	nodo_t* nodo_nuevo;	
	nodo_nuevo = crear_nodo(dato,NULL);
	if (nodo_nuevo == NULL) return false;
	if (lista_esta_vacia(lista)){
		lista->prim = nodo_nuevo;
		lista->ult = nodo_nuevo;
	}
	else{
		nodo_nuevo->prox = lista->prim;
		nodo_t* nodo_ex_prim = lista->prim;
		lista->prim = nodo_nuevo;
		lista->prim->prox = nodo_ex_prim;

	}	
	lista->largo++;	
	return true;
}
// Inserta un nodo en la ultima posicion de la lista enlazada.
// Pre: La lista enlazada fue creada.
// Pos: Devuelve un booleano de acuerdo a si se pudo insertar en dicho lugar o no.

bool lista_insertar_ultimo(lista_t *lista, void *dato){
	nodo_t* nuevo_nodo = crear_nodo(dato,NULL);
	if (nuevo_nodo == NULL) return false;
	
	if (lista_esta_vacia(lista)){
		lista->prim = nuevo_nodo;
	}
	else{
		lista->ult->prox = nuevo_nodo;
	}
	lista->ult = nuevo_nodo;	
	lista->largo++;
	return true;
}

// Elimina el primer elemento de la lista enlazada. Si la lista enlazada posee elementos, se borrará el 
// que se encuentre en el primer lugar, devolviendo su valor, sino se devuelve NULL.
// Pre: la lista enlada fue creada.
// Pos: Devuelve el dato que existía en la primera posicion. La lista enlazada tiene menos elementos, si 
// no esta vacía.
void *lista_borrar_primero(lista_t *lista){
	if (lista_esta_vacia(lista)) return NULL;
	nodo_t* primero = lista->prim;
	lista->prim = primero->prox;
	void* dato = primero->dato;
	if (primero->prox == NULL) lista->ult = NULL;
	free(primero);
	lista->largo--;	
	return dato;
}

// Muestra el primer elemento de la lista enlazada. Si la lista enlazada tiene elementos, se devuelve el 
// valor de la primera posicion, si esta vacía devuelve NULL.
// Pre:la lista fue creada.
// Pos:Devuelve el dato que se encuenta en la primera posición, cuando no está vacía.

void *lista_ver_primero(const lista_t *lista){
	if ( lista_esta_vacia(lista)) return NULL;
	return lista->prim->dato;
}

// Muestra el ultimo elemento de la lista enlazada. Si la lista enlazada tiene elementos, se devuelve el 
// valor de la ultima posicion, si esta vacía devuelve NULL.
// Pre:la lista fue creada.
// Pos:Devuelve el dato que se encuenta en la ultima posición, cuando no está vacía.

void *lista_ver_ultimo(const lista_t *lista){
	if (lista_esta_vacia(lista)) return NULL;
	return lista->ult->dato;
}

// Muestra el largo de la lista enlazada.
// Pre: La lista enlazada fue creada.
// Pos: devuelve el valor de la cantidad de elementos que posee la lista enlazada.

size_t lista_largo(const lista_t *lista){
	return lista->largo;
}

// Destruye la lista enlazada.Si se recibe la función destruir_dato por parámetro,
// para cada uno de los elementos de la lista enlazada llama a destruir_dato.
// Pre: La lista enlazada fue creada. destruir_dato es una función capaz de destruir
// los datos de la lista enlazada, o NULL en caso de que no se la utilice.
// Post: se eliminaron todos los elementos de la lista enlazada.

void lista_destruir(lista_t *lista, void destruir_dato(void *)){
	while (!lista_esta_vacia(lista)){
		void* dato = lista_borrar_primero(lista);
		if (destruir_dato != NULL){
			destruir_dato(dato);
		}
	}
	free(lista);
}


// Crea un iterador.
//Pos: devuelve un nuevo iterador.
lista_iter_t *lista_iter_crear(lista_t *lista){
	if (!lista) return NULL;
	lista_iter_t* iter = malloc (sizeof (lista_iter_t));
	if (!iter) return NULL;
	iter-> lista = lista;
	iter->ant = NULL;
	iter->act = lista->prim;	
	return iter;
}	

//Avanza de a un nodo de la lista enlazada.
//Pre: la lista enlazada y el iterador han sido creados.
//Pos: Devuelve un booleano si se pudo avanzar o no.

bool lista_iter_avanzar(lista_iter_t *iter){
	if (!iter || lista_iter_al_final(iter)) return false;
	iter->ant = iter->act;		
	iter->act = iter->act->prox;
	return true;
}

//Muestra el valor del elemento en que se encuentra situado el iterador. Si la lista
//enlazada no se encuentra vacia, devuelve el valor, caso contrario devuelve NULL.
//Pre: El iterador ha sido creado.
//Pos:Devuelve el valor en que se encuentra el iterador, si la lista no está vacía.

void *lista_iter_ver_actual(const lista_iter_t *iter){
	if (!iter || lista_iter_al_final(iter)) return NULL;
	void* dato = iter->act->dato;
	return dato;
}

//Avanza el iterador hasta el final de la lista enlazada.
//Pre: La lista enlazada y el iterador han sido creados.
//Pos: Devuelve un booleano dependiendo si cumplio el objetivo o no.

bool lista_iter_al_final(const lista_iter_t *iter){
	if (!iter) return false;	
	return (iter->act == NULL);
}

//Destruye el iterador de lista enlazada.
//Pre:El iterador ha sido creado.

void lista_iter_destruir(lista_iter_t *iter){	
	free(iter);
}

//Inserta un elemento en la lista enlazada utilizando el iterador.
//Pre: La lista enlazada e interador han sido creados.
//Pos: Devuelve un booleano si insertó o no.

bool lista_iter_insertar(lista_iter_t *iter, void *dato){
	if (!iter) return false;
	if (iter->act == iter->lista->prim){                   //Pos: Lista esta vacia, 
		if (lista_insertar_primero (iter->lista,dato)){		 // o hay un elemento, 
			iter->act = iter->lista->prim;                     // o siempre inserto con iter.
			return true;
		}
		else{
			return false;
		}
	}
	if (lista_iter_al_final(iter)){           //Pos:final de la lista.
		iter->ant = iter->lista->ult;
		if (lista_insertar_ultimo (iter->lista,dato)){
			iter->act = iter->lista->ult;
			return true;
		}
		else{
			return false;
		}
	}
    nodo_t* nodo_nuevo = crear_nodo(dato,iter->act);   //Pos:En medio de la lista.	
	iter->act = nodo_nuevo;
	iter->ant->prox = nodo_nuevo;
	iter->lista->largo++;
	return true;
}	



//Borra un elemento de la lista. Si la no lista esta vacia, devuelve el valor, sino devuelve NULL.
//Pre: El iterador ha sido creado.
//Pos: Devuelve el valor, o NULL.

void *lista_iter_borrar(lista_iter_t *iter){
	if( !iter || lista_iter_al_final(iter))return NULL;
	void* dato_borrar;	
	if(iter->act == iter->lista->prim){ //Lista esta vacia o hay un elemento.
		dato_borrar = lista_borrar_primero(iter->lista);
		iter->act = iter->lista->prim;
		iter->ant = NULL;	
	}
	else{
		nodo_t* nodo_borrar = iter->act; //Hay varios elementos.
		dato_borrar = nodo_borrar->dato;
		if(iter->act == iter->lista->ult){
			iter->lista->ult=iter->ant;
		}
		iter->ant->prox = iter->act->prox;
		iter->act = iter->act->prox;
		iter->lista->largo--;	
		free(nodo_borrar);
	}
	return dato_borrar;
}
//Iterador interno. Funcion"visitar" recibe el dato y un puntero extra, y devuelve true si se debe 
//seguir iterando, false en caso contrario.
//Pre: Se creó el iterador.

void lista_iterar(lista_t *lista, bool (*visitar)(void *dato, void *extra), void *extra){
	if(!lista || lista_esta_vacia(lista))return;
	nodo_t* primero = lista->prim;
	while(visitar (primero->dato,extra) &&(primero = primero->prox)){}
		
}
	
