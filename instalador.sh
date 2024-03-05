#!/bin/bash
clear

# Si no es root nos salimos
if [ "$EUID" -ne 0 ]; then
    echo "Ejecuta el instalador como root"
    exit
fi

# Si no existe la carpeta /opt nos salimos
if [ ! -d "/opt" ]; then
    echo "No existe el directorio /opt"
    echo "Es necesario para la instalacion"
    exit
fi

# Si no existe g++ nos salimos
g++ --version &> /dev/null
if [[ $? -ne 0 ]]; then
    echo "Es necesaria una version de g++ 11 o superior"
    exit
fi

# Decimos que luego puede borrar la carpeta
echo ""
echo "Se va a instalar el comando las"
echo ""
echo "Esta carpeta '.' contiene los archivos de instalacion"
echo "Se puede borrar tras completar la instalacion"
echo ""
sleep 1
echo "Pulse cualquier tecla para empezar la instalacion"
read -s -n 1

clear
echo ""
echo "Compilando..."
echo ""
echo "[#---------]  10% OK"
echo "[#---------]  10% OK"
echo ""

# Compilamos el config y si hay algun error nos salimos
g++ config.cpp -o configcompilado
if [[ $? -eq 0 ]]; then
    echo "Primera compilacion realizada."
else
    echo "Error compilando, es posible que la version g++ no sea 11 o superior"
    exit
fi

echo ""
echo "[######----]  60% OK"
echo "[######----]  60% OK"
echo ""

# Compilamos el comando y si hay algun error nos salimos
g++ main.cpp coleccion.cpp archivo.cpp -o lascompilado
if [[ $? -eq 0 ]]; then
    echo "Segunda compilacion realizada."
else
    echo "Error compilando, es posible que la version g++ no sea 11 o superior"
    exit
fi

echo ""
echo "[##########]  99% OK"
echo "[##########]  99% OK"
echo ""

# mkdir /opt/las.github.rub-ca.las/ y si no se puede nos salimos
install -m 777 -d /opt/las.github.rub-ca.las/
if [[ $? -ne 0 ]]; then
    echo "Error al crear el directorio /opt/las.github.rub-ca.las/"
    exit
fi
echo ""
echo "Directorio /opt/las.github.rub-ca.las/ creado con permisos 777"

# Copiamos los archivos a la carpeta /opt/las.github.rub-ca.las/
install -m 705 lascompilado /opt/las.github.rub-ca.las/
install -m 705 configcompilado /opt/las.github.rub-ca.las/
install -m 666 ftxt /opt/las.github.rub-ca.las/
install -m 705 scr.sh /opt/las.github.rub-ca.las/
install -m 705 desinstalador.sh /opt/las.github.rub-ca.las/

echo "Archivos copiados en ese directorio"

# Creamos un link en /usr/local/bin, si no existe el directorio nos salimos
if [ -d "/usr/local/bin" ]; then
    ln -s /opt/las.github.rub-ca.las/scr.sh /usr/local/bin/las
    chmod uo+xr /usr/local/bin/las
elif [ -d "/usr/bin" ]; then
    ln -s /opt/las.github.rub-ca.las/scr.sh /usr/bin/las
    chmod uo+xr /usr/bin/las
else
    echo "No existe la carpeta /usr/local/bin"
    echo "No existe la carpeta /usr/bin"
    echo "No se puede crear el link"
    echo "Saliendo del instalador..."
    exit
fi

# Creamos la carpeta del man page y lo copiamos
mkdir /usr/local/man/man1/ &> /dev/null
install -m 644 las.1.gz /usr/local/man/man1/
echo ""
echo "Se ha creado la manpage en /usr/local/man/man1/las.1.gz"

echo ""
echo ""
echo "[##########]  100% OK"
echo "[##########]  100% OK"
echo ""
echo "TODO INSTALADO     OK"
echo ""
echo ""