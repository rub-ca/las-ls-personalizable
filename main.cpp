#include <iostream>
#include <stdlib.h>
#include <string>
#include "coleccion.hpp"
using namespace std;

int main(int argc, char *argv[])
{
	// El orden de los argumentos pasados al c++ debe ser el siguiente:
	// 0 - nombre programa  1 - nombre_directorio
	// 2 - size (1b-1g/n)   3 - tipos (-ld7/n)
	// 4 - recursivo(r/n)   5 - grep (hola/'')

	bool bool_recur = false;

	if (*argv[4] == 'r')
	{
		bool_recur = true;
	}

	// Ponemos / al final de la ruta si no lo lleva
	string ruta_acc = argv[1];
	if (ruta_acc.at(ruta_acc.length() - 1) != '/')
	{
		ruta_acc += "/";
	}

	if (argc == 6)
	{
		Coleccion p1(ruta_acc, argv[2], argv[3], bool_recur, argv[5]);
		p1.print_vector_archivos();
	}
	else if (argc == 5)
	{
		Coleccion p1(ruta_acc, argv[2], argv[3], bool_recur, " ");
		p1.print_vector_archivos();
	}
	else
	{
		cout << "Error en el programa";
	}

	return 0;
}
