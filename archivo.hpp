#ifndef ARCHIVO_H
#define ARCHIVO_H

#include <string>
#include <vector>
#include <iostream>
#include "coleccion.hpp"

// Necesario para la "inicializacion"
class Coleccion;

class Archivo
{
public:
    // Variables que se imprimiran
    char tipo_archivo;
    std::string enlaces;
    char permisos_user[3];
    std::string user;
    char permisos_grupo[3];
    std::string grupo;
    char permisos_otros[3];
    unsigned long long tamanno;
    std::string mes;
    std::string dia;
    std::string anno_hora;
    std::string nombre_archivo;
    std::string enlace_archivo;

    // Puntero a la peticion
    Coleccion *padre_peticion;

    // Coleccion que contiene los subdirectorios
    Coleccion *subdirectorios;

    // Constructor y metodos
    Archivo(const std::vector<std::string> &, Coleccion *);

    void print(const std::string &, int);

    void print_e_enlaces();
    void print_u_user();
    void print_p_permi_user();
    void print_k_permi_grupo();
    void print_j_permi_otros();
    void print_g_grupo();
    void print_n_nombre();
    void print_s_size();
    void print_d_fecha();
    void print_l_nombre_link();
    void print_r_ruta();

    void print_color_tipo();
};

#endif