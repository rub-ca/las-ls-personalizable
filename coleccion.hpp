#ifndef COLECCION_h
#define COLECCION_h

#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <fstream>
#include "archivo.hpp"

/*
CONFIGURACION proveniente de fichero conf

string configurador (orden columnas)

cantidad maxima de caracteres propietario
cantidad maxima de caracteres grupo

size max para archivos pequenos
size max para archivos medianos
(para los archivos grandes no hace falta xke son el resto)

11 colores

*/

// Variables globales accesibles en todo el programa:
// Entraron por argumento:
extern bool recursivamente;
extern bool filtro_size;
extern std::string filtro_grep;
extern std::string filtro_tipos;
// Entraron por archivo configuracion:
extern std::string str_conf_columnas;
extern int int_conf_max_user;
extern int int_conf_max_group;
extern unsigned long long pequeno_file_size;
extern unsigned long long mediano_file_size;
extern int conf_colores[11];

class Archivo;

class Coleccion
{
public:
	// Length maxima que pueden alcanzar algunas variables
	// Necesarios para calcular la cantidad de espacios a imprimir
	// Se inicializan cuando la peticion carga sus resgistros
	int length_enlaces_max;
	int length_user_max;
	int length_grupo_max;
	int length_name_max;
	int length_name_link_max;

	// Cuando es recursivo, esto indica la sangria
	int cantidad_espacios;

	// Nombre del directorio
	std::string nombre_directorio;

	std::string ruta_total;

	// Vector que guarda los archivos
	std::vector<Archivo> archivos;

	// Constructores
	Coleccion(const std::string &, const std::string &, const std::string &, bool, const std::string &);
	Coleccion(const std::string &, int, std::string);

	// Metodos
	void init_filtros_size(const std::string &);
	void procesar_linea(const std::string &);
	void print_vector_archivos();
	void cargar_subdirectorios();
	void cargar_directorios();

	Coleccion *crear_peticion(const std::string &);
	Archivo *get_Archivo_by_name(const std::string &);
	Coleccion *get_Peticion_by_strings(const std::vector<std::string> &);
};

#endif