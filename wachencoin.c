#define _POSIX_C_SOURCE 200809L
#define _GNU_SOURCE

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lista.h"
#include "pila.h"
#include "strutil.h"
#include "lectura.h"
#define CANTIDAD_USUARIOS 1000


typedef struct usuario{
	size_t id_usuario;
	char* coordenadas;
	double saldo;
} usuario_t;

typedef struct pago{
	size_t id_pago;
	char* codigo; //Codigo_pila
	double monto;
} pago_t;


	

/*********************************************************************
*                     Funciones Auxiliares                          *
*********************************************************************/
lista_t* crear_cola_procesamientos(){
	lista_t* procesamientos = lista_crear();
	return procesamientos;
}

void destruir_dato(void* dato){
	free(dato);
		
}
char *substr(const char *str, size_t n);

/*char *substr(const char *str, size_t n){
	size_t len = strlen(str);	
	char* texto = malloc(sizeof(char)*(n+1));
	if (texto == NULL) return NULL;
	if (len != 0){
		strncpy(texto,str,n);
	}
	texto[n] = '\0';	
	return texto;
}*/


//Funcion que lee por entrada stdin o por archivo.
//Pos: Devuelve un puntero a la cadena recibida.
char** lector_entrada(){
	char* linea = leer_linea(stdin);
	if( !(*linea) )
			return NULL;
	char **linea_procesada = split(linea,' ');
	return linea_procesada;
}


//Funcion que carga los datos de un usuario.
//Pos: devuelve una estructura usuario con los datos cargados.
//     cuando no se necesiten los usuarios es necesario usar free() para liberaros.
usuario_t* agregar_usuario(size_t id, char* coordenadas, double saldo){
	usuario_t* nuevo_usuario = malloc (sizeof (usuario_t));
	if (nuevo_usuario == NULL){
		return NULL;
	}
	nuevo_usuario->id_usuario = id;
		//size_t coord_sz = strlen(coordenadas);
		//int coord_sin_final = 
		//char* coord = substr(coordenadas,10); //Para sacar el '\n'
	nuevo_usuario->coordenadas = coordenadas;
	nuevo_usuario->saldo = saldo;
	

	return nuevo_usuario;
}

char *substr(const char *str, size_t n);



//Funcion que carga los usuarios registrados en un determinado archivo csv, a un array. 
//Pos: devuelve un puntero al array usuarios.
usuario_t** cargador_usuarios(int argc, char* argv[], usuario_t** usuarios){ //vector_t* usuarios, FILE* archivo);
	FILE *archivo;
	if((archivo = fopen(argv[1], "r")) == NULL) {
		fprintf(stderr, "No pudo abrirse las cuentas de los usuarios \"%s\"!\n", argv[1]);
		return NULL;
	}
	if (usuarios == NULL){
		fprintf(stderr,"No se pudo cargar las cuentas de los usuarios");
		fclose(archivo);
		return NULL;
	}	
	char *linea = NULL;
	size_t capacidad = 0;
	int cant = 0;
	while (getline(&linea,&capacidad,archivo) >= 0){
		char** resultado = split(linea, ','); 
		int id = atoi (resultado[0]);
		double monto = atof(resultado[1]);
		size_t largo_codigo = strlen(resultado[2])-1;
		char* cod_sin_salto_linea = substr(resultado[2],largo_codigo); 
		size_t id_ = (size_t) id;
		usuarios[cant] = agregar_usuario(id_, cod_sin_salto_linea,monto);
		free_strv(resultado);
		linea = NULL;
		cant++;	
		
	}
	for(int i = cant; i < CANTIDAD_USUARIOS; i++){
		usuarios[cant] = NULL;	
	}
	
	free(linea);
	fclose(archivo);
	return usuarios;
}
//Funcion que realiza el movimiento de fondos de un pago de la cola de pagos pendientes.
void movilizador_de_fondos(lista_t* procesamientos, usuario_t** usuarios){
	pago_t* pago_actual = lista_borrar_primero(procesamientos);
	if(pago_actual == NULL)return;
	size_t id_pago_act = pago_actual->id_pago;
	char* codigo_pago = pago_actual->codigo;
	char** codigo_pago_procesado = split(codigo_pago,';');
	size_t id_usuario_1 = (size_t) atoi(codigo_pago_procesado[0]);
	size_t id_usuario_2 = (size_t) atoi(codigo_pago_procesado[1]);
	double monto_pagar = pago_actual->monto;
	usuario_t* us_1 = usuarios[id_usuario_1];
	usuario_t* us_2 = usuarios[id_usuario_2];
	double saldo_us_1 = us_1->saldo;
	double saldo_us_2 = us_2->saldo;
	double monto_final_u1 = (saldo_us_1 - monto_pagar);
	double monto_final_u2 = (saldo_us_2 + monto_pagar);
	usuarios[id_usuario_1]->saldo = monto_final_u1;
	usuarios[id_usuario_2]->saldo = monto_final_u2;
	if(!lista_esta_vacia(procesamientos)){
		pago_t* actual = lista_ver_primero(procesamientos);
		if(actual->id_pago == id_pago_act){
			movilizador_de_fondos(procesamientos, usuarios);
		}
	}
	free(pago_actual);
	//free(us_1);
	//free(us_2);
	free_strv(codigo_pago_procesado);
	return;
}
bool validar_largo_entrada (char** linea){
	char* comandos[] = {"agregar_pago","pagos_pendientes","procesar", "guardar_cuentas","finalizar"};
	size_t comando_agregar = 4;
	size_t comando_pago_s_p = 1;
	size_t comando_procesar = 2;
	size_t comando_guardar = 2;
	size_t comando_finalizar = 1;
	if(strcmp(linea[0],comandos[0])==0){
		for(int pos = 0; pos < 4; pos++){
			//if(linea[pos]==NULL){
			//	fprintf(stderr, "Validar-largoAGError en comando <%s>\n",comandos[0] );	
			//	return false;
			//}
		}
		if(linea[comando_agregar]!=NULL ){
			fprintf(stderr, "Error en comando <%s>\n",comandos[0] );
			return false;
		}
		
	}else if (strcmp(linea[0],comandos[1])==0){
		if(linea[comando_pago_s_p] != NULL){
			fprintf(stderr, "Error en comando <%s>\n",comandos[1] );	
			return false;
		}
	}
	else if (strcmp(linea[0],comandos[2])==0){
		if(linea[comando_procesar] != NULL){
			fprintf(stderr, "Error en comando <%s>\n",comandos[2] );	
			return false;
		}
	}
	else if (strcmp(linea[0],comandos[3])==0){
		if(linea[comando_guardar] != NULL){
			fprintf(stderr, "Error en comando <%s>\n",comandos[3] );	
			return false;
		}
	}
	else if (strcmp(linea[0],comandos[4])==0){
		if(linea[comando_finalizar] != NULL){
			fprintf(stderr, "Error en comando <%s>\n",comandos[4] );	
			return false;
		}
	}
	else{
		fprintf(stderr, "Error en comando <%s>\n","NO EXISTE" );
		return false;
	}
	return true;
}


/*********************************************************************
*                            Codigo de Pago                          *
*********************************************************************/

bool validar_usuario(pila_t* script, usuario_t** usuarios,int id){
	char* coord_usuario = pila_desapilar(script); 
	size_t id_usuario_1 = (size_t)  atoi(pila_desapilar(script)); //desapila id usuario
	usuario_t* us1 =usuarios[id_usuario_1]; 
	if (id_usuario_1 < CANTIDAD_USUARIOS && usuarios[id_usuario_1]!= NULL){
		if ( strcmp(us1->coordenadas,coord_usuario) != 0){
			fprintf(stderr, "Error en pago <%d>\n", id );  
			free(coord_usuario);
			return false;
		}
	}		
	else{
		free(coord_usuario);
		return false;
	}	
	//free(coord_usuario);
	return true;
}

bool validar_pago(pila_t* script, usuario_t** usuarios,int id_usuario){
	char* monto_pagar =  pila_desapilar(script);
	double monto_p = atof(monto_pagar);
	size_t id_usuario_1 = (size_t) atoi(pila_desapilar(script));
	usuario_t* usuario_actual = usuarios[id_usuario_1];
	double fondos = usuario_actual->saldo;
	double fondos_restantes = (fondos - monto_p);
	if(fondos_restantes <= 0.0 ){
		fprintf(stderr, "Error en pago <%d>\n", id_usuario ); 
		return false;
	}
	return true;
}
bool generar_pago(lista_t* procesamientos, size_t id_pago, double monto, char* cod_pag);
bool generar_pago(lista_t* procesamientos, size_t id_pago, double monto, char* cod_pag){
	pago_t* pago = malloc(sizeof(pago_t));
	if(!pago){
		fprintf(stderr, "Error en pago <%zu>\n",id_pago);
		return false;
	}
	pago->id_pago = id_pago;
	pago->monto = monto;
	pago->codigo = strdup(cod_pag);
	if(!lista_insertar_ultimo(procesamientos, pago)){
		fprintf(stderr, "Error en pago <%zu>\n",id_pago);
		return false;
	}
	return true;
}

bool pagar(lista_t* procesamientos, pila_t* pila_scripts,int id){
			if(!procesamientos || !pila_scripts) return false;	
			char cod_pago[3];
			char* id_usuario_1 = pila_desapilar(pila_scripts);
			char* id_usuario_2 = pila_desapilar(pila_scripts);
			strncat(cod_pago,id_usuario_1,2);
			strncat(cod_pago,";",2);
			strncat(cod_pago,id_usuario_2,2);
			char* monto_pagar =  pila_desapilar(pila_scripts);
			double monto_p = atof(monto_pagar);
			size_t id_pago = (size_t)id;
			bool genero = generar_pago(procesamientos, id_pago, monto_p, cod_pago);
			return genero;
}
/*********************************************************************
*                              Interfaz                              *
*********************************************************************/
		
//Funcion que agregará a la cola de procesamiento la transacción. El monto es siempre positivo.
void agregar_pago(lista_t* procesamientos, usuario_t** us, int id, double monto, char* codigo_procesado){
	pila_t* pila_scripts = pila_crear();
	char** codigo= split(codigo_procesado, ';');
	for(int pos = 0; codigo[pos] != NULL; pos++){
		if(strcmp(codigo[pos],"validar_usuario") == 0){
			if (!validar_usuario(pila_scripts, us,id)){
				break;
			}
		}else if(strcmp(codigo[pos],"validar_pago") == 0){
			if (!validar_pago(pila_scripts,us,id)){
				break;
			}
		}else if(strcmp(codigo[pos],"pagar") == 0){
			if (!pagar(procesamientos,pila_scripts,id)){
				break;
			}
			
		}else{
			pila_apilar(pila_scripts, codigo[pos]);	
		}
	}
	pila_destruir(pila_scripts);
	free_strv(codigo);
}


// Funcion que devuelve por stdout, la cantidad y monto total de las transacciones sin realizar.
void pagos_pendientes(lista_t* procesamientos){
	lista_iter_t* iterador = lista_iter_crear(procesamientos);
	//size_t largo = lista_largo(procesamientos);
	double monto_total = 0.0; 
	while(!lista_iter_al_final(iterador)){
		usuario_t* actual = lista_iter_ver_actual(iterador);
		monto_total = (monto_total + actual->saldo);
		lista_iter_avanzar(iterador);
	}
	lista_iter_destruir(iterador);
	
}

//Funcion que procesa hasta un número no negativo de pagos pendientes.
void procesar_pagos(lista_t* procesamientos, int numero, usuario_t** usuarios){
	while(numero > 0){
		if(lista_esta_vacia(procesamientos)) break;
		movilizador_de_fondos(procesamientos, usuarios);
		numero--;
	}
	//free(pago);
		
}	
//Funcion que guarda en un archivo el estado actual de las cuentas.
//  Si no existe lo debe crear, y si existe lo sobreescribe.
void guardar_cuentas(usuario_t** us, char* archivo){
	FILE* destino = fopen(archivo,"w+");
	if ( destino == NULL ) {
		fprintf(stderr, "Error al abrir el archivo destino");
	}
	size_t pos = 0;
	while(us[pos]!=NULL){
		size_t id_us = us[pos]->id_usuario;
		char* coord =  us[pos]->coordenadas;
		double saldo = us[pos]->saldo;
		fprintf(destino, "%zu, %s,%3.f\n",id_us,coord,saldo);
		pos++;
	}
	fclose(destino);
}
	
//Finaliza el programa (si quedan pagos sin procesar, no se procesan).
void finalizar(lista_t* procesamientos){
	lista_destruir(procesamientos, destruir_dato);
}

	 

//Funcion que procesa comandos.
//Pos: Devuelve 1 si proceso algun error, devuelve 0 sino lo hizo.
int comandos(lista_t* lista_transacciones, usuario_t** usuarios, char** linea_procesada){
	bool largo_ok = validar_largo_entrada(linea_procesada);
	if(!largo_ok) return 1;
	if ( strcmp(linea_procesada[0],"agregar_pago") == 0){
		int id_usuario_procesado = atoi(linea_procesada[1]);
		if (id_usuario_procesado != atoi(linea_procesada[1]) ){
			fprintf(stderr, "Error en comando <%s>\n",linea_procesada[0] );
			return 1;
		}
		if (linea_procesada[2] != NULL && linea_procesada[3]!= NULL){
			if(! atof(linea_procesada[2])){
				fprintf(stderr, "Error en comando <%s>\n",linea_procesada[0]);
				return 1;
			}
			double monto_procesado = atof(linea_procesada[2]);
			agregar_pago(lista_transacciones,usuarios,id_usuario_procesado,monto_procesado,linea_procesada[3]);		
		}
		else{
			fprintf(stderr, "Error en comando <%s>\n",linea_procesada[0] );
			return 1;
		}
	}else if (strcmp(linea_procesada[0],"pagos_pendientes") == 0){
		pagos_pendientes(lista_transacciones);
	}else if (strcmp(linea_procesada[0],"procesar") == 0){
		int cant_procesar;
		if(linea_procesada[1] != NULL){
			cant_procesar = atoi(linea_procesada[1]);
			if( cant_procesar != atoi(linea_procesada[1]) ){
				fprintf(stderr, "Error en comando <%s>\n",linea_procesada[0] );
				return 1;
			}
			if(cant_procesar <= 0){
				fprintf(stderr, "Error en comando <%s>\n",linea_procesada[0] );
				return 1;
			}
			procesar_pagos(lista_transacciones, cant_procesar, usuarios);
		}
		else{
			fprintf(stderr, "Error en comando <%s>\n",linea_procesada[0] );
			return 1;	
		}
		
	}else if (strcmp(linea_procesada[0],"guardar_cuentas") == 0){
		if(! linea_procesada[1]){
			fprintf(stderr, "Error en comando <%s>\n",linea_procesada[0] );
			return 1;
		}
		guardar_cuentas(usuarios,linea_procesada[1]);
	}else if (strcmp(linea_procesada[0],"finalizar") == 0){
		finalizar(lista_transacciones);
	}
	else{
		fprintf(stderr, "Error en comando <%s>\n",linea_procesada[0] );
		return 1;
	}
	free_strv(linea_procesada);
	return 0;
}

void interfaz(lista_t* procesamientos, usuario_t** usuarios){
	char** linea_procesada = lector_entrada();
	if (linea_procesada == NULL) return ;
	if(strcmp(linea_procesada[0],"finalizar") == 0){
		free_strv(linea_procesada);
		return;
	}
	int loop = comandos(procesamientos,usuarios,linea_procesada);
	if(loop == 0) printf("OK\n");
	else{
		free_strv(linea_procesada);
		return;
	}
	interfaz(procesamientos, usuarios);
}


void destruir_usuarios(usuario_t** usuarios){
	for(int i = 0; i < CANTIDAD_USUARIOS; i++){
		if(usuarios[i] != NULL){
			destruir_dato(usuarios[i]);
		}
	}
	return;
}

int main(int argc,char *argv[]){
	if(argc != 2){
		fprintf(stderr, "Uso: %s <archivo>\n", argv[0]);
		return 1;
	}
	

	usuario_t**  usuarios = malloc(sizeof(usuario_t*)*CANTIDAD_USUARIOS); //= malloc(sizeof(usuario_t)*CANTIDAD_USUARIOS);
	if(usuarios == NULL) return 0;
	usuarios = cargador_usuarios(argc,argv,usuarios);  
	if(usuarios == NULL) return 0;
	lista_t* procesamientos = crear_cola_procesamientos();  	
	interfaz(procesamientos,usuarios);
	destruir_usuarios(usuarios);
	free(usuarios);
	return 0;
}

	return linea_procesada;
}


//Funcion que carga los datos de un usuario.
//Pos: devuelve una estructura usuario con los datos cargados.
//     cuando no se necesiten los usuarios es necesario usar free() para liberaros.
usuario_t agregar_usuario(size_t id, char* coordenadas, double saldo){
	usuario_t* nuevo_usuario = malloc (sizeof (usuario_t));
	if (nuevo_usuario != NULL){
		nuevo_usuario->id_usuario = id;
		nuevo_usuario->coordenadas = coordenadas;
		nuevo_usuario->saldo = saldo;
	}
	return *nuevo_usuario;
}
//Funcion que carga los usuarios registrados en un determinado archivo csv, a un array. 
//Pos: devuelve un puntero al array usuarios.
usuario_t* cargador_usuarios(int argc, char* argv[], usuario_t* usuarios){ //vector_t* usuarios, FILE* archivo);
	printf("entro en caragar usuarios.\n");
	FILE *archivo;
	if((archivo = fopen(argv[1], "r")) == NULL) {
		fprintf(stderr, "No pudo abrirse las cuentas de los usuarios \"%s\"!\n", argv[1]);
		return NULL;
	}
	//void* usuarios[CANTIDAD_USUARIOS]; //= malloc(sizeof(usuario_t)*CANTIDAD_USUARIOS);
	if (usuarios == NULL){
		fprintf(stderr,"No se pudo cargar las cuentas de los usuarios");
		fclose(archivo);
		return NULL;
	}	
	char *linea = NULL;
	size_t capacidad = 0;
	int cant = 0;
	while (getline(&linea,&capacidad,archivo) >= 0){
		char** resultado = split(linea, ','); //aplicar split
		size_t id = (size_t) resultado[0];
		double monto = atof(resultado[1]);
		usuarios[cant] = agregar_usuario(id, resultado[2],monto);
		
		linea = NULL;
		cant++;	
		free_strv(resultado);
	}
	
	free(linea);
	fclose(archivo);
	return usuarios;
}
//Funcion que lee por entrada stdin.
//Pos: Devuelve un puntero a la cadena recibida.
/*char** lector_entrada(){
	char* linea_entrada [BUFFER];
	fscanf("%s",linea_entrada);
	if (!(*linea_entrada)){
		fprintf(stderr, "Error 2, faltan parametros.\n");
	}
	char* linea_procesada[] = split(linea_entrada," ");
	return linea_procesada;
}
*/

//Funcion que realiza el movimiento de fondos de un pago de la cola de pagos pendientes.
void movilizador_de_fondos(lista_t* procesamientos, usuario_t* usuarios[]){
	pago_t* pago_actual = lista_borrar_primero(procesamientos);
	size_t id_pago_act = pago_actual->id_pago;
	char* codigo_pago = pago_actual->codigo;
	size_t id_usuario_1 = (size_t) codigo_pago[0];
	size_t id_usuario_2 = (size_t) codigo_pago[2];
	double monto_pagar = pago_actual->monto;
	usuario_t* us_1 = usuarios[id_usuario_1];
	usuario_t* us_2 = usuarios[id_usuario_2];
	double saldo_us_1 = us_1->saldo;
	double saldo_us_2 = us_2->saldo;
	double monto_final_u1 = (saldo_us_1 - monto_pagar);
	double monto_final_u2 = (saldo_us_2 + monto_pagar);
	us_1->saldo = monto_final_u1;
	us_2->saldo = monto_final_u2;
	usuarios[id_usuario_1] = us_1;
	usuarios[id_usuario_2] = us_2;
	pago_t* actual = lista_ver_primero(procesamientos);
	if(actual->id_pago == id_pago_act){
		movilizador_de_fondos(procesamientos, usuarios);
	}
	free(pago_actual);
}
bool validar_largo_entrada (char** linea){
	char* comandos[] = {"agregar_pago","pagos_pendientes","procesar", "guardar_cuentas","finalizar"};
	size_t comando_agregar = 4;
	size_t comando_pagosp = 1;
	size_t comando_procesar = 2;
	size_t comando_guardar = 2;
	size_t finalizar = 1;
	if(strcmp(linea[0],comandos[0])==0){
		for(int pos = 0; pos < 5; pos++){
			if(linea[pos]==NULL){
				fprintf(stderr, "Error en comando <%s>\n",comandos[0] );	
				return false;
			}
		}
		if(linea[comando_agregar+1]!=NULL){
			fprintf(stderr, "Error en comando <%s>\n",comandos[0] );
			return false;
		}
	}else if (strcmp(linea[0],comandos[1])==0){
		if(linea[comando_pagosp+1] != NULL){
			fprintf(stderr, "Error en comando <%s>\n",comandos[1] );	
			return false;
		}
	}else if (strcmp(linea[0],comandos[2])==0){
		if(linea[comando_procesar+1] != NULL){
			fprintf(stderr, "Error en comando <%s>\n",comandos[2] );	
			return false;
		}
	}else if (strcmp(linea[0],comandos[3])==0){
		if(linea[comando_guardar+1] != NULL){
			fprintf(stderr, "Error en comando <%s>\n",comandos[3] );	
			return false;
		}
	}else if (strcmp(linea[0],comandos[4])==0){
		if(linea[finalizar+1] != NULL){
			fprintf(stderr, "Error en comando <%s>\n",comandos[4] );	
			return false;
		}
	}
	else{
		fprintf(stderr, "Error en comando <%s>\n","NO EXISTE" );
		return false;
	}
	return true;
}

/*********************************************************************
*                            Codigo de Pago                          *
*********************************************************************/

bool validar_usuario(pila_t* script, usuario_t* usuarios[],int id){
	char* coord_usuario = pila_desapilar(script); 
	size_t id_usuario_1 = (size_t)  atoi(pila_desapilar(script)); //desapila id usuario
	usuario_t* us1 =usuarios[id_usuario_1]; 
	if (id_usuario_1 < CANTIDAD_USUARIOS && usuarios[id_usuario_1]!= NULL){
		if ( strcmp(us1->coordenadas,coord_usuario) != 0){
			fprintf(stderr, "Error en pago <%d>\n", id );  
			return false;
		}
	}		
	else{
		fprintf(stderr, "Error en pago <%d>\n", id );  
		return false;
	}	
	return true;
}

bool validar_pago(pila_t* script, usuario_t* usuarios[],int id_usuario){
	//double monto_pagar = (double)pila_desapilar(script);
	double *monto_pagar = (double *) pila_desapilar(script);
	double monto_pagar_sin_castear = *monto_pagar;
	size_t id_usuario_1 = (size_t) atoi(pila_desapilar(script));
	usuario_t* usuario_actual = usuarios[id_usuario_1];
	double fondos = usuario_actual->saldo;
	double fondos_restantes = (fondos - monto_pagar_sin_castear);
	if(fondos_restantes <= 0.0 ){
		fprintf(stderr, "Error en pago <%d>\n", id_usuario ); 
		return false;
	}
	return true;
}

void pagar(lista_t* procesamientos, size_t id_pago, double monto, char* cod_pag){
	pago_t* pago = malloc(sizeof(pago_t));
	if(!pago){
		fprintf(stderr, "Error en pago <%zu>\n",id_pago);
	}
	pago->id_pago = id_pago;
	pago->monto = monto;
	pago->codigo = cod_pag;
	if(!lista_insertar_ultimo(procesamientos, pago)){
		fprintf(stderr, "Error en pago <%zu>\n",id_pago);
	}
}


/*********************************************************************
*                              Interfaz                              *
*********************************************************************/
		
//Funcion que agregará a la cola de procesamiento la transacción. El monto es siempre positivo.
void agregar_pago(lista_t* procesamientos, usuario_t* us[], int id, double monto, char* codigo_procesado){
	pila_t* pila_scripts = pila_crear();
	char** codigo= split(codigo_procesado, ';');
	for(int pos = 0; codigo[pos] != NULL; pos++){
		if(strcmp(codigo[pos],"validar_usuario") != 0){
			if (!validar_usuario(pila_scripts, us,id)){
				break;
			}
			
		}else if(strcmp(codigo[pos],"validar_pago") != 0){
			if (!validar_pago(pila_scripts,us,id)){
				break;
			}
		}else if(strcmp(codigo[pos],"pagar") != 0){
			//size_t id_usuario_2 = (size_t) atoi(pila_desapilar(pila_scripts));
			//size_t id_usuario_1 = (size_t) atoi(pila_desapilar(pila_scripts));
			//double monto_pagar = double (pila_desapilar(pila_scripts));
			char cod_pago[2];
			char* id_usuario_1 = pila_desapilar(pila_scripts);
			char* id_usuario_2 = pila_desapilar(pila_scripts);
			strncat(cod_pago,id_usuario_1,2);
			strncat(cod_pago,id_usuario_2,2);
			double *monto_pagar = (double *) pila_desapilar(pila_scripts);
			double monto_pagar_sin_castear = *monto_pagar;
			size_t id_pago = (size_t)id;
			//char* codigo [2] = {id_usuario_1;id_usuario_2}
			//char* cod_pago[0] = id_usuario_1;
			//char* cod_pago[1] = id_usuario_2;
			pagar(procesamientos, id_pago, monto_pagar_sin_castear, cod_pago); 	
		}
		else{
			pila_apilar(pila_scripts, codigo[pos]);	
		}
	}
	pila_destruir(pila_scripts);
	free_strv(codigo);
}


// Funcion que devuelve por stdout, la cantidad y monto total de las transacciones sin realizar.
void pagos_pendientes(lista_t* procesamientos){
	lista_iter_t* iterador = lista_iter_crear(procesamientos);
	size_t largo = lista_largo(procesamientos);
	double monto_total = 0.0; 
	while(!lista_iter_al_final(iterador)){
		usuario_t* actual = lista_iter_ver_actual(iterador);
		monto_total = (monto_total + actual->saldo);
		lista_iter_avanzar(iterador);
	}
	lista_iter_destruir(iterador);
	printf("Cantidad de pagos sin procesar: %zu, Monto Total: %.3f.\n",largo,monto_total);

}

//Funcion que procesa hasta un número no negativo de pagos pendientes.
void procesar_pagos(lista_t* procesamientos, int numero, usuario_t* usuarios[]){
	if(numero < 0)
		fprintf(stderr, "Error en comando <%s>\n","procesar_pagos" );
	while(numero > 0){
		movilizador_de_fondos(procesamientos, usuarios);
		//pago_t*  pago = lista_borrar_primero(procesamientos);
		//char* cod_pago = pago->codigo;
		//pila_t* pila_scripts = pila_crear();
		//char* codigo[]= split(cod_pago, ';');
		//for(int pos = 0; codigo[pos] != NULL; pos++){
		//	if(codigo[pos] != "pagar")
		//		pila_apilar(pila_scripts, codigo[pos]);
		//	else{
		//		pagar(pila_scripts);        //procesar transaccion
		//		numero--;
			
		
	}
	//free(pago);
		
}	
//Funcion que guarda en un archivo el estado actual de las cuentas.
//  Si no existe lo debe crear, y si existe lo sobreescribe.
void guardar_cuentas(usuario_t* us[]){
	FILE* destino = fopen("cuentas_out.csv","w+");
	if ( destino == NULL ) {
		fprintf(stderr, "Error al abrir el archivo destino");
	}
	for(size_t pos = 0; pos < 100; pos++){
		usuario_t* usuario = us[pos];
		size_t id_us = usuario->id_usuario;
		char* coord =  usuario->coordenadas;
		double saldo = usuario-> saldo;
		fprintf(destino, "%zu, %s,%3.f\n",id_us,coord,saldo);
		//char* s = (char*)saldo;
		//char linea[6]; //= {id_us;coord;saldo};
		//strncat(linea,id_us,6);
		//strncat(linea,",",6);
		//strncat(linea,saldo,6);
		//strncat(linea,",",6);
		//strncat(linea,coord,6);
		//strncat(linea,"\n",6);
		//fwrite(linea, 1, sizeof(linea),destino);
		//escribir en archivo linea -verificar error

	}
	fclose(destino);
}
	
//Finaliza el programa (si quedan pagos sin procesar, no se procesan).
void finalizar(lista_t* procesamientos){
	lista_destruir(procesamientos, destruir_dato);
}

	 

//Funcion que procesa comandos.
//Pos: Devuelve 1 si proceso algun error, devuelve 0 sino lo hizo.
int comandos(lista_t* lista_transacciones, usuario_t* usuarios[], char** linea_procesada){
	bool largo_ok = validar_largo_entrada(linea_procesada);
	if(!largo_ok) return 1;
	if ( strcmp(linea_procesada[0],"agregar_pago") == 0){
		int id_usuario_procesado = atoi(linea_procesada[1]);
		if (id_usuario_procesado != atoi(linea_procesada[1]) ){
			fprintf(stderr, "Error en comando <%s>\n",linea_procesada[0] );
			return 1;
		}
		if (linea_procesada[2] != NULL && linea_procesada[3]!= NULL){
			if(! atof(linea_procesada[2])){
				fprintf(stderr, "Error en comando <%s>\n",linea_procesada[0]);
				return 1;
			}
			double monto_procesado = atof(linea_procesada[2]);
			agregar_pago(lista_transacciones,usuarios,id_usuario_procesado,monto_procesado,linea_procesada[3]);		
		}
		else{
			fprintf(stderr, "Error en comando <%s>\n",linea_procesada[0] );
			return 1;
		}
	}else if (strcmp(linea_procesada[0],"pagos_pendientes") == 0){
		pagos_pendientes(lista_transacciones);
	}else if (strcmp(linea_procesada[0],"procesar") == 0){
		int cant_procesar;
		if(linea_procesada[1] != NULL){
			cant_procesar = atoi(linea_procesada[1]);	
			if( cant_procesar != atoi(linea_procesada[1]) ){
				fprintf(stderr, "Error en comando <%s>\n",linea_procesada[0] );
				return 1;
			}
			if(cant_procesar < 0){
				fprintf(stderr, "Error en comando <%s>\n",linea_procesada[0] );
				return 1;
			}
			procesar_pagos(lista_transacciones, cant_procesar, usuarios);
		}
		fprintf(stderr, "Error en comando <%s>\n",linea_procesada[0] );
		return 1;
	}else if (strcmp(linea_procesada[0],"guardar_cuentas") == 0){
		if(! linea_procesada[1]){
			fprintf(stderr, "Error en comando <%s>\n",linea_procesada[0] );
			return 1;
		}
		guardar_cuentas(usuarios);
	}else if (strcmp(linea_procesada[0],"finalizar") == 0){
		finalizar(lista_transacciones);
	}
	else{
		fprintf(stderr, "Error en comando <%s>\n",linea_procesada[0] );
	}
	return 0;
}

//Funcion que interactua con usuario o archivo de texto con indicaciones.
void interfaz(lista_t* procesamientos, usuario_t* usuarios[]){
	char** linea_procesada = lector_entrada();
	if (linea_procesada == NULL) return;
	char* comando = linea_procesada[0];
	while ( strcmp(comando,"finalizar") != 0){
		int procesar = comandos(procesamientos,usuarios,linea_procesada);
		if (procesar == 0){
			printf("OK\n");
		}	
		char** linea_procesada = lector_entrada();
		comando = linea_procesada[0];
	}
	free_strv(linea_procesada);
	//PREGUNTAR:
	//¿como saber el la cantidad de parametros que me queda en el split?
	//Falta validar cuando hay parametros demas.
}

void destruir_usuarios(usuario_t* usuarios[]){
	for(int i = 0; i < CANTIDAD_USUARIOS; i++){
		destruir_dato(usuarios[i]);
	}
	return;
}

//int main(){
int main(int argc,char *argv[]){
	printf("entro en wachen.\n");
	if(argc != 2){
		fprintf(stderr, "Uso: %s <archivo>\n", argv[0]);
		return 1;
	}
	

	usuario_t*  usuarios = malloc(sizeof(usuario_t)*CANTIDAD_USUARIOS); //= malloc(sizeof(usuario_t)*CANTIDAD_USUARIOS);
	
	usuarios = cargador_usuarios(argc,argv,usuarios);  
	if(usuarios == NULL) return 0;
	lista_t* procesamientos = crear_cola_procesamientos();  	
	interfaz(procesamientos,&usuarios);
	destruir_usuarios(&usuarios);
	return 0;
}

















