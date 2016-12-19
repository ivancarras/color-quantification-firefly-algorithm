/* *********************************************************
* Prototipos de las funciones incluidas en funciones_aux.c
********************************************************** */

/* -------------------------------------------------------------
 * Muestra un aviso al usuario y fuerza el final del programa
 *
 * PARAMETROS:
 *   cad_ini: parte inicial del mensaje
 *   cad_fin: parte final del mensaje
 *
 * El mensaje se muestra con el formato:
 *   cad_ini -- cad_fin
 * uso la primera cadena para identificar la operación que origina el problema
 * ( o la estructura de datos correspondiente), y la segunda para mostrar un
 * mensaje predefinido en globales.h (aunque también puedo pasar una cadena sin mas)
 ------------------------------------------------------------- */
void aviso_y_fin(char *cad_ini, char *cad_fin);

/* -----------------------------------------------------------
* Crea una matriz dinamica de reales con n_filas filas y n_col columnas.
* Si se produce un error, el programa acaba.
*
* PARAMETROS:
*   n_filas: numero de filas
*   n_col: numero de columnas
*
* RETORNO:
*   puntero a la matriz
*   (si se produce un error, el programa acaba)
------------------------------------------------------------- */
float **RESERVAR_MAT_REAL(int n_filas, int n_col);

/* -----------------------------------------------------------
* Crea una matriz dinamica de enteros con n_filas filas y diferente numero de
* columnas en cada fila. Cada fila necesita espacio para los documentos del
* cluster asociado. Si se produce un error, el programa acaba.
*
* PARAMETROS:
*   n_filas: numero de filas (numero de clusters encontrados
*   n_col: numero de columnas de cada fila
*
* RETORNO:
*   puntero a la matriz
*   (si se produce un error, el programa acaba)
------------------------------------------------------------- */
int **RESERVAR_MAT_CLUSTERS(int n_filas, int *n_col);

/* -----------------------------------------------------------
* Crea un vector dinamico de enteros. Si se produce un error, el programa acaba.
*
* PARAMETROS:
*   n_elem: numero de elementos del vector
*
* RETORNO:
*   puntero al vector
*   (si se produce un error, el programa acaba)
------------------------------------------------------------- */
int *RESERVAR_VECTOR_ENTERO(int n_elem);

/* -----------------------------------------------------------
* Libera la memoria ocupada por una matriz dinamica de reales
*
* PARAMETROS:
*   p: dirección del puntero al comienzo de la matriz
*   n_filas: numero de filas de la matriz
----------------------------------------------------------- */
void LIBERAR_MAT_REAL(float ***p, int n_filas);

/* -----------------------------------------------------------
* Libera la memoria ocupada por una matriz dinamica de enteros
*
* PARAMETROS:
*   p: direccion del puntero al comienzo de la matriz
*   n_filas: numero de filas de la matriz
----------------------------------------------------------- */
void LIBERAR_MAT_ENTERA(int ***p, int n_filas);

/* -------------------------------------------------------------
* Abre un fichero en el modo indicado. En caso de error el programa termina.
*
* PARAMETROS:
*   nombre_fich: nombre o ruta del fichero a abrir
*   modo: modo de apertura
* RETORNO:
*   en caso de exito, descritor del fichero
*   (en caso de error el programa acaba)
-------------------------------------------------------------- */
FILE * Abrir_fichero(char *nombre_fich, char *modo);

/* --------------------------------------------------------------
* Lee de un fichero una matriz de reales, considerando n_f filas y n_c columnas.
* La primera linea del fichero contiene dos enteros que indican las filas y las
* columnas, respectivamente. El resto de las lineas contienen los datos de las
* sucesivas filas de la matriz
* La utilizo para leer las frecuencias de aparición de las palabras en los
* articulos.
*
* PARAMETROS:
*    nombre_fich: nombre del fichero que contiene los datos
*    mat: matriz que almacenara los datos del fichero
*    n_f: numero de filas de la matriz
*    n_c: numero de columnas de la matriz
*
* RETORNO:
*   numero de valores leidos, si la operacion concluyo con exito
* (si no se leyeron todos los valores requeridos, el programa acaba)
* -------------------------------------------------------------- */
int LEER_M_REAL(char *nombre_fich, float ***mat, int *n_f, int *n_c);

/* ----------------------------------------------------------------------
 * Dado un vector que indica, para cada documento, el cluster asociado, copia en
 * una matriz dicha información.
 *
 * PARAMETROS:
 *   clusters: array que contiene tantas filas como clusters y tantas columnas
 *            como elementos tenga el cluster correspondiente a la fila.
 *            En cada columna almacena el índice de un elemento del cluster
 *            correspondiente a la fila.
 *   n_clus: número de clusters
 *   que_cluster: vector que indica, para cada documento, el cluster asociado
 *   n_docs: numero de documentos de la colección
 ---------------------------------------------------------------------- */
void COPIAR_DATOS_CLUSTERS(int **clusters, int n_clus, int *que_cluster, int n_docs);

/*  ----------------------------------------------------------------------
 * Cuenta los elementos que hay en cada cluster, para poder solicitar espacio
 * dinamicamente para almacenar tales elementos
 *
 * PARAMETROS:
 *   n_clus: numero de clusters
 *   nelem_cluster: número de elementos de cada cluster
 *   que_cluster: cluster al que pertenece cada documento
 *   n_docs: número de documentos de la colección
 ---------------------------------------------------------------------- */
void CONTAR_ELEM_CLUSTERS(int n_clus, int *nelem_cluster, int *que_cluster, 
   int n_docs);