#ifndef WACHENCOIN_H
#define WACHENCOIN_H
#include <stdbool.h>
#include <stdlib.h>

lista_t* crear_cola_procesamientos();

void destruir_dato(void* dato);

//Funcion que desapila la pila de instruccion y devuelve un char*
char* copiar_codigo_pago(pila_t* pila, char* codigo_pago);

//Funcion que carga los datos de un usuario.
//Pos: devuelve una estructura usuario con los datos cargados.
//     cuando no se necesiten los usuarios es necesario usar free() para liberaros.
usuario_t agregar_usuario(size_t id, char* coordenadas, double saldo);
//Funcion que carga los usuarios registrados en un determinado archivo csv, a un array. 
//Pos: devuelve un puntero al array usuarios.
usuario_t* cargador_usuarios(int argc, char* argv[], usuario_t* usuarios);
//Funcion que lee por entrada stdin.
//Pos: Devuelve un puntero a la cadena recibida.
char** lector_entrada();

//Funcion que procesa comandos.
//Pos: Devuelve 1 si proceso algun error, devuelve 0 sino lo hizo.
int comandos(lista_t* lista_transacciones, usuario_t* usuarios[], char** linea_procesada);
void destruir_usuarios(usuario_t* usuarios[]);


/*********************************************************************
*                              Interfaz                              *
*********************************************************************/
		
//Funcion que agregará a la cola de procesamiento la transacción. El monto es siempre positivo.

void agregar_pago(lista_t* procesamientos, usuario_t* us[], int id, double monto, char* codigo_procesado);
// Funcion que devuelve por stdout, la cantidad y monto total de las transacciones sin realizar.
void pagos_pendientes(lista_t* procesamientos);

//Funcion que procesa hasta un número no negativo de pagos pendientes.
void procesar_pagos(lista_t* procesamientos, int numero, usuario_t* usuarios[]);

//Funcion que guarda en un archivo el estado actual de las cuentas.
//  Si no existe lo debe crear, y si existe lo sobreescribe.
void guardar_cuentas(usuario_t* us[]);
	
//Finaliza el programa (si quedan pagos sin procesar, no se procesan).
void finalizar(lista_t* procesamientos);

/*********************************************************************
*                            Codigo de Pago                          *
*********************************************************************/

bool validar_usuario(pila_t* script, usuario_t* usuarios[],int id);
bool validar_pago(pila_t* script, usuario_t* usuarios[],int id_usuario);
void pagar(lista_t* procesamientos, size_t id_pago, double monto, char* cod_pag);
	   
	 


//Funcion que interactua con usuario o archivo de texto con indicaciones.

void interfaz(lista_t* procesamientos, usuario_t* usuarios[]);
		

//Funcion principal.
int main(int argc,char *argv[]);






#endif //WACHENCOIN.H










