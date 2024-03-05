#!/bin/bash
ruta_fichero_temp="/opt/las.github.rub-ca.las/tempss.txt"

rm $ruta_fichero_temp 2>/dev/null # Vaciamos el fichero

# Se puede ejecutar con o sin parametros.
# Si se utiliza con parametros el primero siempre debe ser la ruta
# luego se pueden poner los parametros en cualquier orden
#
# l -> implementa opciones del propio comando ls  (requiere un segundo argumento)
#      opciones t - por fecha; S - por size; U - no sort;
#               r - al reves;
#      no se puede aplicar si se aplica el modo recursivo
#
# r -> se hace de forma recursiva
#      no se puede aplicar si se aplican opciones l

# a -> no muestra los archivos ocultos

# g -> se aplica un grep al nombre (requiere segundo argumento)

# s -> se aplica un filtro de size (requiere segundo argumento '5m-1g')

# + -> muestra los tipos de archivos especificos (requiere segundo argumento)

function f_error() {
    echo "Entrada erronea. Use 'man las' para obtener más informacion"
    echo "Informacion interna: $1"
    rm $ruta_fichero_temp 2>/dev/null
    exit
}

# Miramos si es configuracion
if [[ $1 == "///" ]]; then
    /opt/las.github.rub-ca.las/configcompilado
    exit
fi

# Estas variables son los parametros que recibe el c++
# se va actualiando en el bucle con los parametros introducidos
args_cpp_size="n"
args_cpp_tipos="n"
args_cpp_recur="n"
args_cpp_grep=" "
args_cpp_ruta_accion=" "

# Opciones para el comando ls
opcion_ls=""
oculto_ls_opcion="a"

# Ejecucion sin parametros
if [[ $# -eq 0 ]]; then
    ls -l$oculto_ls_opcion >$ruta_fichero_temp
    args_cpp_ruta_accion="."

#Ejecucion con parametros, el primero es la ruta
else
    forcontador=1
    args_cpp_ruta_accion="$1"

    # Bandera que comprueba si el siguiente argumento es una opcion del comando anterior
    # y no un nuevo comando
    banderaSaltar=false

    for var in "$@"; do
        # Opciones del comando
        # el primer argumento es la ruta, skipeamos ese iteracion

        if [[ forcontador -gt 1 ]]; then

            if [[ $banderaSaltar = true ]]; then # Bandera de no hacer nada
                banderaSaltar=false

            elif [[ $var == + ]]; then # ARGUMENTO + (filtrar por tipo)
                # Calculamos el indice del siguiente argumento
                # argbuscando guarda el indice del siguiente
                argbuscando=$((1 + forcontador))
                # Guardamos el siguiente argumento en la variable
                args_cpp_tipos="${@:$argbuscando:1}"
                # la siguiente iteracion se salta, ya hemos usado ese dato
                banderaSaltar=true

                # La opcion + necesita argumentos
                # si no los tiene error
                if [[ $args_cpp_tipos == "" ]]; then
                    f_error
                fi

            elif [[ $var == g ]]; then # ARGUMENTO g (grep)
                argbuscando=$((1 + forcontador))
                args_cpp_grep="${@:$argbuscando:1}"
                banderaSaltar=true

            elif [[ $var == s ]]; then # ARGUMENTO s (filtrar por size)
                argbuscando=$((1 + forcontador))
                args_cpp_size="${@:$argbuscando:1}"
                banderaSaltar=true

                if [[ $args_cpp_size == "" ]]; then
                    f_error
                fi

            elif [[ $var == l ]]; then # ARGUMENTO l
                argbuscando=$((1 + forcontador))
                opcion_ls="${opcion_ls}${@:$argbuscando:1}"
                banderaSaltar=true

            elif [[ $var == r ]]; then # ARGUMENTO r
                args_cpp_recur="r"
                opcion_ls="${opcion_ls}"R

            elif [[ $var == a ]]; then # ARGUMENTO a
                oculto_ls_opcion=""

            else
                f_error "main for"
            fi

        fi

        ((forcontador++))
    done
fi

# No esta permitido utilzar opciones de ordenacion del ls
# si se selecciona el modo recursivo
function f_check_r_ls() {
    if [[ $args_cpp_recur == r ]]; then
        if [[ $opcion_ls != "R" ]]; then
            f_error "f_check_r_ls()"
        fi
    fi
}
f_check_r_ls

# Comprobamos el input para la opcion de ordenacion del ls
function f_check_inputs_optls() {
    size_opciones_ls=${#opcion_ls}

    if [[ size_opciones_ls -gt 2 ]]; then
        f_error "function f_check_inputs_optls()"
    fi

    if [[ $args_cpp_recur == "r" ]]; then
        opciones_ls="-lR$oculto_ls_opcion"
        return
    fi

    bandera_ls_orden=false
    bandera_ls_reverse=false
    for ((i = 0; i < size_opciones_ls; i++)); do
        char="${opcion_ls:i:1}"

        case $char in
        S)
            if [[ $bandera_ls_orden = true ]]; then
                f_error "function f_check_inputs_optls()"
            fi
            bandera_ls_orden=true
            ;;
        t)
            if [[ $bandera_ls_orden = true ]]; then
                f_error "function f_check_inputs_optls()"
            fi
            bandera_ls_orden=true
            ;;
        U)
            if [[ $bandera_ls_orden = true ]]; then
                f_error "function f_check_inputs_optls()"
            fi
            bandera_ls_orden=true
            ;;
        r)
            if [[ $bandera_ls_reverse = true ]]; then
                f_error "function f_check_inputs_optls()"
            fi
            bandera_ls_reverse=true
            ;;
        *) f_error "function f_check_inputs_optls()" ;;
        esac

    done

    opciones_ls="-l$oculto_ls_opcion$opcion_ls"
}
f_check_inputs_optls

# Guardamos en el fichero el resultado del ls
ls $opciones_ls $1 >$ruta_fichero_temp

# El orden de los argumentos pasados al c++ debe ser el siguiente
# ruta_accion   size (1b-1g/n)   tipos (-ld7/n)   recursivo(r/n)   grep (hola/'')

alcpp="$args_cpp_ruta_accion $args_cpp_size $args_cpp_tipos $args_cpp_recur $args_cpp_grep"
/opt/las.github.rub-ca.las/lascompilado $alcpp
rm $ruta_fichero_temp 2>/dev/null