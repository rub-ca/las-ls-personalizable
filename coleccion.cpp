#include "coleccion.hpp"

using namespace std;

string global_ruta_file = "/opt/las.github.rub-ca.las/tempss.txt";
string global_ruta_file_conf = "/opt/las.github.rub-ca.las/ftxt";

// Variables globales introducidas por argumento
unsigned long long filtro_size_max = 0;
unsigned long long filtro_size_min = 0;
bool filtro_size = false;
bool recursivamente = false; // En todos los archivos
string filtro_grep = "";
string filtro_tipos = "";

// Variables globales del archivo configuracion
string str_conf_columnas;
int int_conf_max_user;
int int_conf_max_group;
unsigned long long pequeno_file_size;
unsigned long long mediano_file_size;
int conf_colores[11];

vector<string> my_spt_str(const string &text, const string &delimiter)
{
	// Object return
	vector<string> r;

	// We convert to char *
	char *ctext = new char[text.length() + 1];
	strcpy(ctext, text.c_str());
	char *cdeli = new char[delimiter.length() + 1];
	strcpy(cdeli, delimiter.c_str());

	char *saver;
	saver = strtok(ctext, cdeli);
	while (saver != NULL)
	{
		r.push_back(saver);
		saver = strtok(NULL, cdeli);
	}

	ctext = nullptr;
	cdeli = nullptr;
	saver = nullptr;

	return r;
}

bool is_number(char mirando)
{
	if (mirando == '0')
	{
		return true;
	}
	if (mirando == '1')
	{
		return true;
	}

	if (mirando == '2')
	{
		return true;
	}

	if (mirando == '3')
	{
		return true;
	}

	if (mirando == '4')
	{
		return true;
	}

	if (mirando == '5')
	{
		return true;
	}

	if (mirando == '6')
	{
		return true;
	}

	if (mirando == '7')
	{
		return true;
	}

	if (mirando == '8')
	{
		return true;
	}

	if (mirando == '9')
	{
		return true;
	}

	return false;
}

bool is_mag_letter(char mirando)
{
	switch (mirando)
	{
	case 'b':
		return true;
	case 'k':
		return true;
	case 'm':
		return true;
	case 'g':
		return true;
	case 't':
		return true;
	default:
		return false;
	}
	return false;
}

bool is_letter_type_file(char mirando)
{
	switch (mirando)
	{
	case '-':
		return true;
	case 'd':
		return true;
	case 'l':
		return true;
	case 'p':
		return true;
	case 'c':
		return true;
	case 'b':
		return true;
	case 's':
		return true;
	default:
		return false;
	}
	return false;
}

void f_error(const string &reporte)
{
	cout << "Entrada erronea. Use 'man las' para obtener más informacion";
	cout << endl;
	cout << "Informacion interna: " << reporte << endl;
	exit(2);
}

bool check_grep(string nombre_mirando)
{
	// Si el filtro es " " significa que esta desactivado
	if (filtro_grep == " ")
	{
		return true;
	}
	bool fg_start = false;
	bool fg_end = false;

	string filtro_virgen = filtro_grep;
	if (filtro_grep.at(0) == '/')
	{
		fg_start = true;
		filtro_virgen = filtro_virgen.substr(1, filtro_virgen.length() - 1);
	}

	if (filtro_grep.at(filtro_grep.length() - 1) == '/')
	{
		fg_end = true;
		filtro_virgen = filtro_virgen.substr(0, filtro_virgen.length() - 1);
	}

	// Si el filtro es mas largo no puede pasar el filtro
	// 'nombre-buscando' no cabe en 'nombre'
	if (filtro_virgen.length() > nombre_mirando.length())
	{
		return false;
	}

	// Empieza por el filtro
	if (fg_start)
	{
		string str_short = nombre_mirando.substr(0, filtro_virgen.length());
		if (str_short != filtro_virgen)
		{
			return false;
		}
	}

	// Acaba por el filtro
	if (fg_end)
	{
		string str_short = nombre_mirando.substr(nombre_mirando.length() - filtro_virgen.length(), filtro_virgen.length());
		if (str_short != filtro_virgen)
		{
			return false;
		}
	}

	// Miramos si contiene o no el filtro
	if (nombre_mirando.find(filtro_virgen) == std::string::npos)
	{
		return false;
	}

	return true;
}

int get_iteraciones_mag(char magn)
{
	switch (magn)
	{
	case 'b':
		return 0;
	case 'k':
		return 1;
	case 'm':
		return 2;
	case 'g':
		return 3;
	case 't':
		return 4;
	}
	return -1;
}

void init_config_vars()
{
	string linea_leida;
	ifstream file(global_ruta_file_conf);

	if (file.is_open())
	{
		// Indice de las lineas leidas
		int contador_while = 0;
		while (getline(file, linea_leida))
		{
			switch (contador_while)
			{
			// str configurador columnas
			case 0:
				str_conf_columnas = linea_leida;
				break;
			// length max de user y group
			case 1:
				int_conf_max_user = stol(linea_leida);
				break;
			case 2:
				int_conf_max_group = stol(linea_leida);
				break;
			// size pequeno y mediano
			case 3:
				pequeno_file_size = stol(linea_leida);
				break;
			case 4:
				mediano_file_size = stol(linea_leida);
				break;
			// colores
			default:
				conf_colores[contador_while - 5] = stol(linea_leida);
				break;
			}
			++contador_while;
		}
		if (contador_while != 16)
		{
			f_error("contador while != 16 void init_config_vars();");
		}
		file.close();
	}
	else
	{
		f_error("Error leyendo archivo");
	}
}

Coleccion::Coleccion(const string &p_ruta, const string &f_size, const string &f_tipo, bool f_recu, const string &f_grep)
{
	// Esta es la primera peticion que se llama
	// El constructor para los subdirectorios es el otro

	// Load global variables que controlan los filtros
	init_filtros_size(f_size);
	filtro_grep = f_grep;
	filtro_tipos = f_tipo;
	recursivamente = f_recu;

	length_enlaces_max = 0;
	length_user_max = 0;
	length_grupo_max = 0;
	length_name_link_max = 0;
	nombre_directorio = p_ruta;
	ruta_total = p_ruta;

	cantidad_espacios = 0;

	init_config_vars();

	if (recursivamente)
	{
		cargar_subdirectorios();
	}
	else
	{
		cargar_directorios();
	}
}

Coleccion::Coleccion(const string &p_ruta, int ces, string b)
{
	length_enlaces_max = 0;
	length_user_max = 0;
	length_grupo_max = 0;
	length_name_link_max = 0;
	nombre_directorio = p_ruta;

	cantidad_espacios = ces * 8;

	ruta_total = b;
}

void Coleccion::init_filtros_size(const string &parametro)
{
	if (parametro == "n")
	{
		filtro_size = false;
		filtro_size_max = 0;
		filtro_size_min = 0;
		return;
	}
	filtro_size = true;
	// 1b-1g  desde 1 byte hasta 1 gb
	// 0g-1g  hasta 1 gb
	// 1m-0b  desde 1 mb
	// 123b-10g desde 123 bytes hasta 10 gb

	// Hay varios estados
	// Leyendo num min
	// Leyendo '-'
	// Leyendo num max
	string estad = "num min";

	// Variables necesarias para guardar los filtros
	string str_size_min = "0";
	char char_mag_min = ' ';
	string str_size_max = "0";
	char char_mag_max = ' ';

	const int s_p = parametro.length();
	for (int i = 0; i < s_p; i++)
	{
		if (estad == "num min")
		{
			if (is_number(parametro[i]))
			{
				str_size_min += parametro[i];
			}
			else if (is_mag_letter(parametro[i]))
			{
				estad = "-";
				char_mag_min = parametro[i];
			}
			else
			{
				f_error("void Coleccion::init_filtros_size(const string &parametro)  -> estad=num min");
			}
		}
		else if (estad == "-")
		{
			estad = "num max";

			if (parametro[i] != '-')
			{
				f_error("void Coleccion::init_filtros_size(const string &parametro)  -> estad='-'");
			}
		}
		else if (estad == "num max")
		{
			if (is_number(parametro[i]))
			{
				str_size_max += parametro[i];
			}
			else if (is_mag_letter(parametro[i]))
			{
				char_mag_max = parametro[i];

				if (i != s_p - 1)
				{
					f_error("void Coleccion::init_filtros_size(const string &parametro)  -> estad=num max");
				}
			}
			else
			{
				f_error("void Coleccion::init_filtros_size(const string &parametro)  -> estad=num max");
			}
		}
	}

	// Si no llegamos al estado num max el filtro estaba mal
	if (estad != "num max")
	{
		f_error("void Coleccion::init_filtros_size(const string &parametro)");
	}

	filtro_size_min = stol(str_size_min);
	filtro_size_max = stol(str_size_max);

	int x_iteraciones = get_iteraciones_mag(char_mag_min);
	for (int x = 0; x < x_iteraciones; x++)
	{
		filtro_size_min *= 1024;
	}
	x_iteraciones = get_iteraciones_mag(char_mag_max);
	for (int x = 0; x < x_iteraciones; x++)
	{
		filtro_size_max *= 1024;
	}
}

void Coleccion::procesar_linea(const string &linea)
{
	/*
	Esta funcion se encarga de crear un Archivo y annadirlo
	al vector de archivos.

	Tambien actualiza las variables length_max
	*/

	// Obtenemos el vector de strings desde la linea leida
	vector<string> data_new_archivo = my_spt_str(linea, " ");

	// Actualizamos las variables length_max
	if (data_new_archivo[1].length() > length_enlaces_max)
		length_enlaces_max = data_new_archivo[1].length();

	if (data_new_archivo[2].length() > length_user_max)
		length_user_max = data_new_archivo[2].length();

	if (data_new_archivo[3].length() > length_grupo_max)
		length_grupo_max = data_new_archivo[3].length();

	if (data_new_archivo[8].length() > length_name_max)
		length_name_max = data_new_archivo[8].length();

	// Para obtener la length total del nombre y enlace debemos realizar
	// un bucle que recorra todos los string que hay desde el nombre
	// hasta el final. En esta linea, por ejemplo, seria:
	//  									EMPIEZA -> -> -> -> ACABA
	//                                          |                  |
	// lrwxrwxrwx  1 root root  10 ene 18 18:36 libx32 -> usr/libx32
	int nuevo_name_link_max = 0;
	for (int x = 8; x < data_new_archivo.size(); x++)
	{
		nuevo_name_link_max += data_new_archivo[x].length();
	}

	if (nuevo_name_link_max > length_name_link_max)
	{
		length_name_link_max = nuevo_name_link_max;
	}

	// Creamos un archivo
	Archivo a1(data_new_archivo, this);

	// Si es modo recursivo y es un directorio se salta los filtros
	if (recursivamente && a1.tipo_archivo == 'd')
	{
		// Las carpetas '.' y '..' no se guardan
		if (a1.nombre_archivo == "." || a1.nombre_archivo == "..")
		{
			return;
		}
		archivos.push_back(a1);
		return;
	}

	// Filtro size
	if (filtro_size)
	{
		if (filtro_size_max != 0 && filtro_size_max < a1.tamanno)
		{
			return;
		}

		if (filtro_size_min != 0 && filtro_size_min > a1.tamanno)
		{
			return;
		}
	}

	// Filtro tipos
	if (filtro_tipos.at(0) != 'n')
	{
		for (int x = 0; x < filtro_tipos.length(); x++)
		{
			if (!is_letter_type_file(filtro_tipos.at(x)))
			{
				f_error("if (!is_letter_type_file(filtro_tipos.at(x))){");
			}

			if (a1.tipo_archivo == filtro_tipos.at(x))
			{
				break;
			}

			if (x == filtro_tipos.length() - 1)
			{
				return;
			}
		}
	}

	// Filtro grep
	if (!check_grep(a1.nombre_archivo))
	{
		return;
	}

	// Si ha pasado los filtros lo ponemos en el vector
	archivos.push_back(a1);
}

void Coleccion::print_vector_archivos()
{
	const int s_str = archivos.size();
	for (int x = 0; x < s_str; x++)
	{
		archivos[x].print(str_conf_columnas, cantidad_espacios);
	}
}

void Coleccion::cargar_subdirectorios()
{
	string linea_leida;
	ifstream file(global_ruta_file);
	bool recur_empezado = false;
	bool next_ruta = false;
	bool saltando_total = false;
	string current_ruta = "";
	Coleccion *apuntador;

	if (file.is_open())
	{
		int contador_while = 0;
		while (getline(file, linea_leida))
		{
			if (saltando_total)
			{
				saltando_total = false;
				continue;
			}
			// Empiezan a cargasrse los subdirectorios
			if (recur_empezado)
			{
				if (linea_leida == "")
				{
					next_ruta = true;
				}
				else if (next_ruta)
				{
					current_ruta = linea_leida;
					current_ruta = current_ruta.substr(nombre_directorio.length(), current_ruta.length() - nombre_directorio.length());
					next_ruta = false;
					saltando_total = true;
					// Buscamos el archivo directorio al que hace referencia
					// y hacemos en su puntero subdirectorios = new Coleccion
					// Actualizamos nuestro puntero apuntador a esa nueva Coleccion
					apuntador = crear_peticion(current_ruta);
				}
				else
				{
					// llamamos al metodo procesar_linea del puntero apuntador
					apuntador->procesar_linea(linea_leida);
				}
			}
			// Primer directorio, peticion original
			else if (contador_while >= 2)
			{
				if (linea_leida == "")
				{
					recur_empezado = true;
					next_ruta = true;
				}
				else
				{
					procesar_linea(linea_leida);
				}
			}
			++contador_while;
		}
		file.close();
	}
}

void Coleccion::cargar_directorios()
{
	string linea_leida;
	ifstream file(global_ruta_file);

	if (file.is_open())
	{
		int contador_while = 0;
		while (getline(file, linea_leida))
		{
			if (contador_while >= 1)
			{
				procesar_linea(linea_leida);
			}
			++contador_while;
		}
		file.close();
	}
}

Archivo *Coleccion::get_Archivo_by_name(const string &buscando)
{
	Archivo *pdevolver;
	int size_v = archivos.size();
	for (int x = 0; x < size_v; x++)
	{
		if (archivos[x].tipo_archivo == 'd')
		{
			if (archivos[x].nombre_archivo == buscando)
			{
				pdevolver = &archivos[x];
				return pdevolver;
			}
		}
	}
	f_error("RETURN NULLPTR; Archivo *Coleccion::get_Archivo_by_name(const string &buscando)");
	return nullptr;
}

Coleccion *Coleccion::get_Peticion_by_strings(const vector<string> &v_ss)
{
	const int ruta_strs_size = v_ss.size() - 1;
	Coleccion *pDevolver = this;

	for (int x = 0; x < ruta_strs_size; x++)
	{
		pDevolver = pDevolver->get_Archivo_by_name(v_ss[x])->subdirectorios;
	}

	return pDevolver;
}

Coleccion *Coleccion::crear_peticion(const string &buscand)
{
	string str_less1 = buscand.substr(0, buscand.length() - 1);
	vector<string> v_strs = my_spt_str(str_less1, "/");

	string buscando = v_strs[v_strs.size() - 1];
	string nueva_r_total = nombre_directorio + str_less1 + "/";
	get_Peticion_by_strings(v_strs)->get_Archivo_by_name(buscando)->subdirectorios = new Coleccion(buscando, v_strs.size(), nueva_r_total);
	return get_Peticion_by_strings(v_strs)->get_Archivo_by_name(buscando)->subdirectorios;
}
