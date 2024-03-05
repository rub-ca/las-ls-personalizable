#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <fstream>
#include <termios.h>
#include <stdio.h>
#include <unistd.h>

using namespace std;

string global_ruta_file_conf = "/opt/las.github.rub-ca.las/ftxt";

string str_configurador;

int max_length_user;
int max_length_group;

unsigned long long max_size_peque;
unsigned long long max_size_mediano;

int conf_colores[11];

#define BLACK "\033[1m\033[30m"   /* Bold Black */
#define RED "\033[1m\033[31m"     /* Bold Red */
#define GREEN "\033[1m\033[32m"   /* Bold Green */
#define YELLOW "\033[1m\033[33m"  /* Bold Yellow */
#define BLUE "\033[1m\033[34m"    /* Bold Blue */
#define MAGENTA "\033[1m\033[35m" /* Bold Magenta */
#define CYAN "\033[1m\033[36m"    /* Bold Cyan */
#define WHITE "\033[1m\033[37m"   /* Bold White */
#define RESET "\033[0m"

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

int keypress()
{
    struct termios estadoAnterior;
    struct termios estadoNuevo;
    unsigned char echo_bit = ECHO; // =ECHO no print; =0 lo imprime
    int c;

    // Guardamos una copia del estado anterior
    if (-1 == tcgetattr(STDIN_FILENO, &estadoAnterior))
    {
        return EOF; // Error
    }

    estadoNuevo = estadoAnterior;

    // use canonical input and set echo
    estadoNuevo.c_lflag &= ~(echo_bit | ICANON);
    //  set minimal input to 1
    estadoNuevo.c_cc[VMIN] = 1;

    if (-1 == tcsetattr(STDIN_FILENO, TCSANOW, &estadoNuevo))
    {
        return EOF; // Error
    }

    // Espera hasta recibir una entrada
    c = getchar();

    // Guardamos el estado anterior de la terminal
    if (-1 == tcsetattr(STDIN_FILENO, TCSANOW, &estadoAnterior))
    {
        return EOF; // Error
    }
    return c;
}

void f_insert_any_key(string mensaje)
{
    cout << mensaje << endl
         << "Inserta cualquier caracter para continuar" << endl;

    int c;
    // Vaciamos el buffer
    while ((c != '\n') && (c != EOF))
    {
        c = getchar();
    }
    c = keypress();
}

void init_variables()
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
                str_configurador = linea_leida;
                break;
            // length max de user y group
            case 1:
                max_length_user = stol(linea_leida);
                break;
            case 2:
                max_length_group = stol(linea_leida);
                break;
            // size pequeno y mediano
            case 3:
                max_size_peque = stol(linea_leida);
                break;
            case 4:
                max_size_mediano = stol(linea_leida);
                break;
            // Colores
            default:
                conf_colores[contador_while - 5] = stol(linea_leida);
                break;
            }
            ++contador_while;
        }
        file.close();
    }
}

void guardar_variables()
{
    ofstream file(global_ruta_file_conf);
    if (file.is_open())
    {
        file << str_configurador << endl;

        file << max_length_user << endl;
        file << max_length_group << endl;

        file << max_size_peque << endl;
        file << max_size_mediano << endl;

        for (int x = 0; x < 11; x++)
        {
            file << conf_colores[x];
            if (x < 10)
            {
                file << endl;
            }
        }

        // Close the file
        file.close();
    }
    else
    {
        cout << "ha habido un error abriendo el archivo de configuracion" << endl
             << "Saliendo del programa...";
        exit(1);
    }
}

bool check_str_conf(string mirando)
{
    for (int x = 0; x < mirando.length(); x++)
    {
        char caracter = mirando.at(x);
        switch (caracter)
        {
        case '0':
            break;
        case 't':
            break;
        case 'e':
            break;
        case 'p':
            break;
        case 'u':
            break;
        case 'k':
            break;
        case 'g':
            break;
        case 'j':
            break;
        case 'o':
            break;
        case 's':
            break;
        case 'd':
            break;
        case 'n':
            break;
        case 'l':
            break;
        case 'r':
            break;
        default:
            return false;
        }
    }

    return true;
}

bool check_number(string str)
{
    for (int i = 0; i < str.length(); i++)
        if (!isdigit(str[i]))
            return false;
    return true;
}

void configurar_columnas()
{
    cout << endl;
    cout << "0 - espacio en blanco" << endl;
    cout << "t - tipo archivo" << endl;
    cout << "e - cantidad links" << endl;
    cout << "p - permisos user" << endl;
    cout << "u - nombre user" << endl;
    cout << "k - permisos grupo" << endl;
    cout << "g - nombre grupo" << endl;
    cout << "j - permisos otros" << endl;
    cout << "o - 'o' as 'otros'" << endl;
    cout << "s - tamanno" << endl;
    cout << "d - fecha" << endl;
    cout << "n - nombre" << endl;
    cout << "l - nombre y enlace" << endl;
    cout << "r - ruta previa (solo se muestra en recursivo)" << endl;
    cout << "  - ejemplo: t0e000p0u00k0g00j0o00d00s00rl" << endl;
    cout << "  - ejemplo: t00p0k0j00s00l" << endl
         << endl;
    cout << "Actual:" << str_configurador << endl;
    cout << endl
         << "    opcion -> ";

    string str_intro;
    cin >> str_intro;

    if (check_str_conf(str_intro))
    {
        str_configurador = str_intro;
    }
    else
    {
        f_insert_any_key("Formato incorrecto");
    }
}

void configurar_colores()
{
    // Menu seccion
    cout << endl
         << "Una entrada erronea te saca del menu" << endl
         << "Codigos de seccion:" << endl
         << "0 - Directorios" << endl
         << "1 - Resto de archivos" << endl
         << "2 - Enlace ->" << endl
         << "3 - Cantidad de enlaces y fechaEnlace" << endl
         << "4 - User y sus permisos" << endl
         << "5 - Grupo y sus permisos" << endl
         << "6 - Otros y sus permisos" << endl
         << "7 - Archivos pequenos" << endl
         << "8 - Archivos medianos" << endl
         << "9 - Archivos grandes" << endl
         << "10 - Permiso ejecucion 'x'" << endl
         << endl
         << "101 - Set colores 1 -> 4 5 6 0 3 7 2 5 6 2 5" << endl
         << endl;

    // Metemos la seccion y la comprobamos
    cout << "Codigo de seccion -> ";
    string s_cod_sec;
    cin >> s_cod_sec;

    if (s_cod_sec == "101")
    {
        // Ponemos el set por defecto y salimos de la funcion
        conf_colores[0] = 4;
        conf_colores[1] = 5;
        conf_colores[2] = 6;
        conf_colores[3] = 0;
        conf_colores[4] = 3;
        conf_colores[5] = 7;
        conf_colores[6] = 2;
        conf_colores[7] = 5;
        conf_colores[8] = 6;
        conf_colores[9] = 2;
        conf_colores[10] = 5;
        return;
    }

    if (!check_number(s_cod_sec))
    {
        f_insert_any_key("Debe ser un numero");
        return;
    }

    int i_cod_sec = stol(s_cod_sec);
    if (i_cod_sec < 0 || i_cod_sec > 10)
    {
        f_insert_any_key("Debe estar entre 0 y 10. ");
        return;
    }

    // Menu color
    cout << "Una entrada erronea te saca del menu" << endl
         << "Codigos de color: " << endl;
    print_color(0);
    cout << "0 - color por defecto" << endl;
    print_color(1);
    cout << "1 - negro" << endl;
    print_color(2);
    cout << "2 - rojo" << endl;
    print_color(3);
    cout << "3 - verde" << endl;
    print_color(4);
    cout << "4 - amarillo" << endl;
    print_color(5);
    cout << "5 - azul" << endl;
    print_color(6);
    cout << "6 - magenta" << endl;
    print_color(7);
    cout << "7 - cyan" << endl;
    print_color(8);
    cout << "8 - blanco" << endl;
    print_color(0);

    // Metemos el color y lo comprobamos
    cout << "Codigo de color -> ";
    string s_cod_col;
    cin >> s_cod_col;

    if (!check_number(s_cod_col))
    {
        f_insert_any_key("Debe ser un numero");
        return;
    }

    int i_cod_col = stol(s_cod_col);
    if (i_cod_col < 0 || i_cod_col > 8)
    {
        f_insert_any_key("Debe estar entre 0 y 8. ");
        return;
    }

    // Muestra los elegidos
    f_insert_any_key("Seccion elegida : " + to_string(i_cod_sec) + "   Color elegido : " + to_string(i_cod_col));
    conf_colores[i_cod_sec] = i_cod_col;
}

void configurar_max_user()
{
    cout << endl
         << "Una entrada no numerica te saca del menu" << endl
         << "Maxima length en el nombre del propietario (user) -> ";
    string s_leido;
    cin >> s_leido;

    if (!check_number(s_leido))
    {
        f_insert_any_key("Debe ser un numero");
        return;
    }

    max_length_user = stol(s_leido);

    f_insert_any_key("\nNueva length maxima: " + to_string(max_length_user));
}

void configurar_max_group()
{
    cout << endl
         << "Una entrada no numerica te saca del menu" << endl
         << "Maxima length en el nombre del grupo -> ";
    string s_leido;
    cin >> s_leido;

    if (!check_number(s_leido))
    {
        f_insert_any_key("Debe ser un numero");
        return;
    }

    max_length_group = stol(s_leido);

    f_insert_any_key("\nNueva length maxima: " + to_string(max_length_group));
}

void configurar_max_pequenos()
{
    cout << endl
         << "Una entrada no numerica te saca del menu" << endl
         << "Maximo size para archivos pequenos." << endl
         << "Solo influye en el color del size." << endl
         << "Nuevo size en bytes para pequenos -> ";
    string s_leido;
    cin >> s_leido;

    if (!check_number(s_leido))
    {
        f_insert_any_key("Debe ser un numero");
        return;
    }

    max_size_peque = stol(s_leido);

    f_insert_any_key("\nNueva size para pequenos: " + to_string(max_size_peque));
}

void configurar_max_medianos()
{
    cout << endl
         << "Una entrada no numerica te saca del menu" << endl
         << "Maximo size para archivos medianos." << endl
         << "Solo influye en el color del size." << endl
         << "Nuevo size en bytes para medianos -> ";
    string s_leido;
    cin >> s_leido;

    if (!check_number(s_leido))
    {
        f_insert_any_key("Debe ser un numero");
        return;
    }

    max_size_mediano = stol(s_leido);

    f_insert_any_key("\nNueva size para pequenos: " + to_string(max_size_mediano));
}

int main()
{
    init_variables();

    int opcion_menu = -1;

    while (opcion_menu != 0)
    {
        system("clear");
        cout << endl
             << "*** ** *   MENU   * ** ***" << endl
             << endl
             << "0 - Salir sin guardar" << endl
             << "1 - Guardar y salir" << endl
             << endl
             << "2 - Configurar columnas" << endl
             << "3 - Configurar colores" << endl
             << endl
             << "4 - Introducir max length de user" << endl
             << "5 - Introducir max length de grupo" << endl
             << endl
             << "6 - Introducir max size de archivos pequenos" << endl
             << "7 - Introducir max size de archivos medianos" << endl;

        cout << endl
             << "    Opcion -> ";
        string st_leido;
        cin >> st_leido;
        if (!check_number(st_leido))
        {
            continue;
        }
        opcion_menu = stol(st_leido);

        system("clear");

        switch (opcion_menu)
        {
        case 0:
            goto fuera_nosave;
            break;
        case 1:
            goto fuera_guardando;
            break;
        case 2:
            configurar_columnas();
            break;
        case 3:
            configurar_colores();
            break;
        case 4:
            configurar_max_user();
            break;
        case 5:
            configurar_max_group();
            break;
        case 6:
            configurar_max_pequenos();
            break;
        case 7:
            configurar_max_medianos();
            break;
        }
    }

fuera_guardando:;

    guardar_variables();

fuera_nosave:;

    return 0;
}