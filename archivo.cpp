#include "archivo.hpp"

using namespace std;

#define BLACK "\033[1m\033[30m"   /* Bold Black */
#define RED "\033[1m\033[31m"     /* Bold Red */
#define GREEN "\033[1m\033[32m"   /* Bold Green */
#define YELLOW "\033[1m\033[33m"  /* Bold Yellow */
#define BLUE "\033[1m\033[34m"    /* Bold Blue */
#define MAGENTA "\033[1m\033[35m" /* Bold Magenta */
#define CYAN "\033[1m\033[36m"    /* Bold Cyan */
#define WHITE "\033[1m\033[37m"   /* Bold White */
#define RESET "\033[0m"

/*
    Configuracion de los colores

    [0] color de los directorios, el caracter 'd' y el nombre
    [1] color para el resto de archivos
    [2] color para el '->' y link

    [3] color para la cantidad de enlaces y la fecha

    [4] color para user y sus permisos
    [5] color para grupo y sus permisos
    [6] color para otros y sus permisos

    [7] color archivos pequenos
    [8] color archivos medianos
    [9] color archivos grandes

    [10] color permiso ejecucion

*/
void print_color(int index_color)
{
    switch (index_color)
    {
    case 0:
        cout << RESET;
        break;
    case 1:
        cout << BLACK;
        break;
    case 2:
        cout << RED;
        break;
    case 3:
        cout << GREEN;
        break;
    case 4:
        cout << YELLOW;
        break;
    case 5:
        cout << BLUE;
        break;
    case 6:
        cout << MAGENTA;
        break;
    case 7:
        cout << CYAN;
        break;
    case 8:
        cout << WHITE;
        break;
    }
}

// 0 Permisos y tipo 	// 1 Enlaces
// 2 User 	// 3 Group 	// 4 Size
// 5 6 7 Access  // 8 Nombre
Archivo::Archivo(const vector<string> &datos_nuevo, Coleccion *padre)
{
    // A partir del vector con strings debemos inicializar las variables
    padre_peticion = padre;
    subdirectorios = nullptr;

    tipo_archivo = datos_nuevo[0].at(0);

    permisos_user[0] = datos_nuevo[0].at(1);
    permisos_user[1] = datos_nuevo[0].at(2);
    permisos_user[2] = datos_nuevo[0].at(3);

    permisos_grupo[0] = datos_nuevo[0].at(4);
    permisos_grupo[1] = datos_nuevo[0].at(5);
    permisos_grupo[2] = datos_nuevo[0].at(6);

    permisos_otros[0] = datos_nuevo[0].at(7);
    permisos_otros[1] = datos_nuevo[0].at(8);
    permisos_otros[2] = datos_nuevo[0].at(9);

    enlaces = datos_nuevo[1];

    user = datos_nuevo[2];
    grupo = datos_nuevo[3];

    tamanno = stol(datos_nuevo[4]);

    mes = datos_nuevo[5];
    dia = datos_nuevo[6];
    anno_hora = datos_nuevo[7];

    // El nombre del archivo puede tener espacios
    // por tanto debemos de buscar el simbolo ->
    // Si no lo hay todo es nombre
    // Si lo hay es nombre hasta esos simbolos
    // nueva carpeta -> link
    //   8     9      10   11
    // enlace -> link
    //   8      9  10

    bool poniendo_nombre = true;
    enlace_archivo = "";
    nombre_archivo = "";

    for (int x = 8; x < datos_nuevo.size(); x++)
    {
        if (datos_nuevo[x] == "->")
        {
            poniendo_nombre = false;
        }
        else
        {
            if (poniendo_nombre)
            {
                nombre_archivo += datos_nuevo[x];
                nombre_archivo += " ";
            }
            else
            {
                enlace_archivo += datos_nuevo[x];
                enlace_archivo += " ";
            }
        }
    }

    // Eliminamos los espacios en blanco al final
    nombre_archivo = nombre_archivo.substr(0, nombre_archivo.size() - 1);
    enlace_archivo = enlace_archivo.substr(0, enlace_archivo.size() - 1);
}

void Archivo::print(const string &str_conf, int cantidad_espacios)
{

    /* FUNCIONALIDAD DESACTIVADA
     * Comprobamos si hay algun filtro activo
     * Si lo hay y dentro de el no hay ningun
     * archivo no queremos mostrarlo
     * Esto deberia impedir mostrar las carpetas cuando no hay archivos
     * el problema es que si busco si hay archivos y solo las muestros si los hay
     * una carpeta que se llame asd si mi filtro es un grep asd no se mostrara
     * si no contiene ningun archivo
     */

    // Ponemos la sangria necesaria
    for (int ces = 0; ces < cantidad_espacios; ces++)
    {
        cout << " ";
    }
    const int s_str = str_conf.length();
    for (int x = 0; x < s_str; x++)
    {
        switch (str_conf[x])
        {
        case '0':
            cout << " ";
            break;
        case 't':
            print_color_tipo();
            cout << tipo_archivo;
            print_color(0);
            break;
        case 'e':
            print_e_enlaces();
            break;
        case 'p':
            print_p_permi_user();
            break;
        case 'u':
            print_u_user();
            break;
        case 'k':
            print_k_permi_grupo();
            break;
        case 'g':
            print_g_grupo();
            break;
        case 'j':
            print_j_permi_otros();
            break;
        case 'o':
            print_color(conf_colores[6]);
            cout << "o";
            print_color(0);
            break;
        case 's':
            print_s_size();
            break;
        case 'd':
            print_d_fecha();
            break;
        case 'n':
            print_n_nombre();
            break;
        case 'l':
            print_l_nombre_link();
            break;
        case 'r':
            print_r_ruta();
            break;
        }
    }
    cout << endl;

    // Comprobamos que no este vacio para no imprimir saltos de linea innecesarios
    if (subdirectorios != nullptr && !subdirectorios->archivos.empty())
    {
        cout << endl;
        subdirectorios->print_vector_archivos();
        cout << endl;
    }
}

void Archivo::print_p_permi_user()
{
    print_color(conf_colores[4]);
    cout << permisos_user[0] << permisos_user[1];

    // Si no es carpeta y tiene ejecucion cambiamos color
    if (permisos_user[2] == 'x')
    {
        print_color(conf_colores[10]);
    }
    cout << permisos_user[2];
    print_color(0);
}

void Archivo::print_k_permi_grupo()
{
    print_color(conf_colores[5]);
    cout << permisos_grupo[0] << permisos_grupo[1];
    // Si no es carpeta y tiene ejecucion cambiamos color
    if (permisos_grupo[2] == 'x')
    {
        print_color(conf_colores[10]);
    }
    cout << permisos_grupo[2];
    print_color(0);
}

void Archivo::print_j_permi_otros()
{
    print_color(conf_colores[6]);
    cout << permisos_otros[0] << permisos_otros[1];
    // Si no es carpeta y tiene ejecucion cambiamos color
    if (permisos_otros[2] == 'x')
    {
        print_color(conf_colores[10]);
    }
    cout << permisos_otros[2];
    print_color(0);
}

void Archivo::print_e_enlaces()
{
    print_color(conf_colores[3]);
    int maximo = (*padre_peticion).length_enlaces_max;
    int espacios = maximo - enlaces.length();
    for (int x = 0; x < espacios; x++)
    {
        cout << " ";
    }
    cout << enlaces;
    print_color(0);
}

void Archivo::print_u_user()
{
    print_color(conf_colores[4]);
    if (int_conf_max_user > user.length())
    {
        int maximo = padre_peticion->length_user_max;
        int espacios = maximo - user.length();
        cout << user;
        for (int x = 0; x < espacios; x++)
        {
            cout << " ";
        }
    }
    else
    {
        for (int x = 0; x < int_conf_max_user; x++)
        {
            cout << user.at(x);
        }
    }
    print_color(0);
}

void Archivo::print_g_grupo()
{
    print_color(conf_colores[5]);
    if (int_conf_max_group > grupo.length())
    {
        int maximo = padre_peticion->length_grupo_max;
        int espacios = maximo - grupo.length();
        cout << grupo;
        for (int x = 0; x < espacios; x++)
        {
            cout << " ";
        }
    }
    else
    {
        for (int x = 0; x < int_conf_max_group; x++)
        {
            cout << grupo.at(x);
        }
    }
    print_color(0);
}

void Archivo::print_n_nombre()
{
    print_color_tipo();
    int maximo = padre_peticion->length_name_max;
    int espacios = maximo - nombre_archivo.length();
    cout << nombre_archivo;
    for (int x = 0; x < espacios; x++)
    {
        cout << " ";
    }
    print_color(0);
}

void Archivo::print_s_size()
{
    if (tamanno > mediano_file_size)
    {
        // Large files
        print_color(conf_colores[9]);
    }
    else if (tamanno > pequeno_file_size)
    {
        // Normal files
        print_color(conf_colores[8]);
    }
    else
    {
        // Small files
        print_color(conf_colores[7]);
    }
    double resultado = 0;
    char magnitud = ' ';
    if (tamanno > 5000000000000)
    { // TB
        resultado = tamanno / 1024;
        resultado = resultado / 1000;
        resultado = resultado / 1000;
        resultado = resultado / 1000;
        magnitud = 't';
    }
    else if (tamanno > 5000000000)
    { // GB
        resultado = tamanno / 1024;
        resultado = resultado / 1000;
        resultado = resultado / 1000;
        magnitud = 'g';
    }
    else if (tamanno > 5000000)
    { // MB
        resultado = tamanno / 1024;
        resultado = resultado / 1000;
        magnitud = 'm';
    }
    else if (tamanno > 5000)
    { // KB
        resultado = tamanno / 1024;
        magnitud = 'k';
    }
    else
    { // B
        resultado = tamanno;
        magnitud = 'b';
    }

    // Dependiendo de los digitos de resultado pondremos mas espacios o menos
    if (resultado >= 100)
    {
        if (resultado < 1000)
        {
            cout << " ";
        }
    }
    else if (resultado >= 10)
    {
        cout << "  ";
    }
    else if (resultado >= 0)
    {
        cout << "   ";
    }
    int casteado = resultado;
    cout << casteado << magnitud;

    print_color(0);
}

void Archivo::print_d_fecha()
{
    print_color(conf_colores[3]);
    cout << mes << " ";
    if (dia.length() == 1)
    {
        cout << "0";
    }

    cout << dia << " ";

    if (anno_hora.length() == 4)
    {
        cout << " ";
    }
    cout << anno_hora;
    print_color(0);
}

void Archivo::print_l_nombre_link()
{
    print_color_tipo(); // color depende del tipo de archivo

    int maximo = padre_peticion->length_name_link_max;
    int espacios = maximo - nombre_archivo.length();
    espacios -= enlace_archivo.length();
    cout << nombre_archivo;

    // Ponemos el color del link
    print_color(conf_colores[2]);

    if (enlace_archivo.length() > 0)
    {
        cout << " -> " << enlace_archivo;
        espacios -= 2;
    }
    else
    {
        cout << "  ";
    }

    for (int x = 0; x < espacios; x++)
    {
        cout << " ";
    }
    print_color(0); // reset color
}

void Archivo::print_r_ruta()
{
    // La ruta solo se imprime en recursivo
    if (recursivamente)
    {
        print_color_tipo(); // color depende del tipo de archivo
        cout << padre_peticion->ruta_total;
        print_color(0);
    }
}

void Archivo::print_color_tipo()
{
    if (tipo_archivo == 'd')
    {
        print_color(conf_colores[0]);
    }
    else
    {
        print_color(conf_colores[1]);
    }
}