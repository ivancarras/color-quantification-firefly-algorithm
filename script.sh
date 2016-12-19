#!/bin/bash

echo "Escribe el nombre del programa:"
read programa
./$programa avion.txt
./$programa lena.txt
./$programa lago.txt
./$programa pimientos.txt
./$programa datos_mandril.txt
./$programa chica_rubia.txt

./txt2tifmod salida_luci_avion.txt
./txt2tifmod salida_luci_lena.txt
./txt2tifmod salida_luci_lago.txt
./txt2tifmod salida_luci_pimientos.txt
./txt2tifmod salida_luci_datos_mandril.txt
./txt2tifmod salida_luci_chica_rubia.txt

