#define _POSIX_C_SOURCE 200809L

#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "strutil.h"

char *substr(const char *str, size_t n);

char *substr(const char *str, size_t n){
	size_t len = strlen(str);	
	char* texto = malloc(sizeof(char)*(n+1));
	if (texto == NULL) return NULL;
	if (len != 0){
		strncpy(texto,str,n);
	}
	texto[n] = '\0';	
	return texto;
}

char **split(const char *str, char sep){
	if (!str || !sep) return NULL;
	size_t cant = 0;
	for (size_t i=0;i<(strlen(str));i++){
		if (str[i] == sep) cant++;
	}
	char** cadenas = malloc(sizeof(char*)*(cant+2));
	if(cadenas == NULL){
		return NULL; 
	}
	size_t pos = 0;
	if (cant == 0){
		size_t l = strlen(str);
		cadenas[pos] = substr(str,l);
		pos++;
		cadenas[pos] = NULL;
		return cadenas;
	}
	char* token = strchr(str, sep);
	size_t nn = (size_t) (token-str);	
	
  	cadenas[pos] = substr(str,nn); 
	pos++;
		while (token != NULL){
		char* token_viejo = token;
		token = strchr(token+1,sep); 
		if(token == NULL){
			size_t nnn = strlen(token_viejo);			
			nn = (size_t) (nnn-1);
			cadenas[pos] = substr(token_viejo+1,nn);
			break;
		}
		nn = (size_t) (token - token_viejo-1);
		cadenas[pos] = substr(token_viejo+1,nn);
		pos++;
	}
	pos++;	
	cadenas[pos] = NULL;
	return cadenas;
	
}
				


void free_strv(char *strv[]){
	if(!strv) return;
	int pos = 0;
	while(strv[pos] != NULL){
		free(strv[pos]);
		pos++;
	}
	free(strv);
}


