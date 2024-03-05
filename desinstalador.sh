#!/bin/bash

# Si no es root nos salimos
if [ "$EUID" -ne 0 ]; then
    echo "Ejecuta el desinstalador como root"
    exit
fi

echo ""
echo "Iniciando desinstalador..."
echo ""
echo ""
echo "Se van a borrar los siguientes elementos:"
echo ""
echo "- /opt/las.github.rub-ca.las/"
echo "- /usr/local/bin/las/"
echo "- /usr/bin/las/"
echo "- /usr/local/man/man1/las.1.gz"

echo "Quiere desintalar las? Si/n"
read -s -n 1 key

if [[ $key == "n" ]]; then
    echo "No se ha desinstalado el programa"
    exit
fi

rm -r /opt/las.github.rub-ca.las/ &>/dev/null
rm /usr/local/bin/las &>/dev/null
rm /usr/bin/las &>/dev/null
rm /usr/local/man/man1/las.1.gz &>/dev/null

clear
echo ""
echo "LAS DESINSTALADO"
echo ""
echo ""
