/* ****************************************************************************************************
*
* @autor Iván Carrasco Alonso
* COMPILACION:
*    gcc -o luci kmeans_imagenes.c -L. -lkmeans -lm
*
* EJECUCION:
*	luci [fich_pixels]
* [fich_pixels]: fichero que contiene los colores de los pixels.
*
*
*******************************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <malloc.h>
#include <math.h>
#include <float.h> // para que k-means encuentre el maximo float (DBL_MAX)


/***********************CONSTANTES*****************/

#define ITERACCIONES_LUC 10 //numero de iteraciones en el algoritmo de la luciernaga
#define E 2.72//NUMERO E 
#define N_LUCI 4//NUMERO LUCIERANGAS
#define ITERACCIONES_KM 10 //3 //10 // 150 poner aquí el número máximo de iteraciones del algoritmo k-means
#define N_TESTS 1   // numero de pruebas
#define ERROR 0.1 // 1e-4
#define N_CLUSTERS 8 //numero de clusters que quiero
#define RGB 3 //colores del rgb numero

/*******************VARIABLES GLOBALES**********************/

int N;          // numero de pixels leidos del fichero de datos
int R=RGB;    // numero de valores leidos port pixel (3: valores R, G y B)
float **d;  // valores RGB de los pixels de la imagen (1 pixel por fila)
char nombre_fich_salida[200];    /* fichero que contiene los pixels de la imagen cuantizada */
int n_clusters_quiero= N_CLUSTERS; //32 64 128 256  //PONER AQUI EL TAMAÑO DE LA PALETA DESEADA
float lucis[N_LUCI][N_CLUSTERS][RGB];//luciernagas (Numerolucieranaga) X (clusters) X (3 colores)
float brillos[N_LUCI];//Brillo de las luciernagas

// ------------- PROTOTIPOS -----------------------

int *k_means(float **data, int n_docs, int m, int k, float t, float **centroids,  float **centroidsXXX,  int es_alea,int number_luci);
void k_medias_aleatorio_X(int n_docs, int n_clusters, int n_claves, int X,int aleatorio,int number_luci);
void aplicar_kmeans(int argc, char **argv,int number_luci,int aleatorio);
int aplicar_luciernaga(int argc, char **argv);
float distancia(int i,int j);
int mirar_mas_brillante(int j);
float aleatorio();

/* prototipos de las funciones de la librería kmeans */
#include "funciones_aux.h"


/* ############################################
 *     operaciones relacionadas con k-medias
 * ############################################ */

/* ---------------------------------------------------------------
 Copia de k_means en la que paso yo los indices de los centroides:
 paso los elementos que haya encontrado como más proximos a los centroides
 tras aplicar el algoritmo ant-tree
 PARAMETROS:
 *   data: array que contiene los items a clasificar
 *   n_docs: numero de items a clasificar
 *   m: número de claves consideradas para definir los items a clasificar
 *   k: numero de clusters que se van a generar en la partición
 *   t: error permitido
 *   centroids:
 *   centroidsXXX: centroides iniciales que paso para la ejecución de k-means
 *    es_alea: 1 => es aleatorio (tomar los centroides pasados como parámetros
                                0=> nolo es (tomas los de dentro de esta función
 ---------------------------------------------------------------- */

int *k_means(float **data, int n_docs, int m, int k, float t,
   float **centroids,  float **centroidsXXX,int es_alea,int number_luci)
{
    int h, i, j;                       /* contadores de bucles */
    float old_error, error = DBL_MAX; /* sum of squared euclidean distance */

    struct timeval ti_a, tf_a;   // para la aplicación del algoritmo de hormigas
    unsigned long long tiem_a, tiem_a0;  // para almacenar la diferencia entre los dos tiempos previos


    /* output cluster label for each data point.
	     Para cada item de la colección, indica el cluster al que se asocia */
    int *labels = (int*)calloc(n_docs, sizeof(int));

    int *counts = (int*)calloc(k, sizeof(int)); /* puntero a un vector de enteros que
                                                almacena el tamaño de cada cluster */


    float **c =
		       centroids ? centroids : (float**)calloc(k, sizeof(float*));

    float **c1 =
		       (float**)calloc(k, sizeof(float*)); /* centroides temporales. Reservo espacio
	                                                   para k punteros a vectores que contendrán
	                                                   las coordenadas de los k centroides */
    int pasada =1;
    double AQE, r1, r2, r3;

    FILE *fd;

    int contador =0;


 
   /****
   ** initialization.
	 Se definen k centroides (cada centroide es un vector de m pesos).
	 Para calcular un centroide se toman datos de la colección separados
	 n_docs/k posiciones.
	 (ej.: para una coleccion de 100 documentos y 5 clusters,
 	   centroide 1: con el documento 0
	   centroide 2: con el documento 20
	   centroide 3: con el documento 40
	   centroide 4: con el documento 60
	   centroide 5: con el documento 80   ) */
   for (h = i = 0; i < k; h += n_docs / k, i++)
   {
      /* para cada centroide, i, reservo espacio para tantos elementos como claves
		   incluyen los doc. de la colección */
      c1[i] = (float*)calloc(m, sizeof(float));

		/* si el puntero no es nulo, se reserva espacio para un vector de reales */
      if (!centroids)
         c[i] = (float*)calloc(m, sizeof(float));

      /* tomo k puntos como centroides iniciales */
      for (j = m; j-- > 0; c[i][j] = data[h][j]);
   }

	if(es_alea == 1)
	{
		/* yo redefino los centroides de los bucles previos, copiando los que he
		pasado como parámetro. Dejo esos bucles porque reservan memoria  */
		for(i=0; i<k; i++)
			for(j=0; j<m; j++)
				c[i][j]=centroidsXXX[i][j];
	}



   /****
   ** bucle principal
	 Repetir mientras que el error de la iteración actual frente a la previa
	 supere el umbral t */
   do {
	   pasada++;

	   if (pasada>ITERACCIONES_KM)
	   {
		   break;
	   }


      /* save error from last step
		 - old_error: error de la iteración previa
		 - error: error de la iteración actual, que se inicializa a cero */
      old_error = error, error = 0;

      /* clear old counts and temp centroids:
		 - cada cluster contiene 0 elementos (counts[] a cero)
		 - el centroide de cada cluster tiene todas sus coordenadas a cero, c1[] */
      for (i = 0; i < k; counts[i++] = 0)
      {
         for (j = 0; j < m; c1[i][j++] = 0);
      }


		/* para cada item de la colección, h, se determina el cluster más proximo,
		 almacenando su numero en labels[h] */
      for (h = 0; h < n_docs; h++)
      {
         /* ---- se identifica el cluster más proximo ----
			 Para cada cluster, i, se calcula la distancia de su prototipo al item
			 h-ésimo. Se toma el cluster que da menor valor (el número del cluster
		    se almacena en labels[h] */
         float min_distance = DBL_MAX;  // la distancia mínima inicial es el mayor float posible

         for (i = 0; i < k; i++) // para cada cluster
         {
			/* distancia entre el item j-ésimo y el centroide del cluster i-ésimo
			 (suma de los cuadrados de los errores entre las m coordenadas del
			  item h y las del prototipo del cluster i*/
            float distance = 0;
            for (j = m; j-- > 0; distance += pow(data[h][j] - c[i][j], 2));

            if (distance < min_distance)
            {
               labels[h] = i;
               min_distance = distance;
            }
         }

		 /* se suma el item h-ésimo al centroide del cluster más proximo y se
			 incrementa el contador de elementos de dicho cluster */
         /* update size and temp centroid of the destination cluster */
         for (j = m; j-- > 0; c1[labels[h]][j] += data[h][j]);
         counts[labels[h]]++;

         /* update standard error.
			 Se acumula en el error la mínima distancia encontrada entre el item h
			 y el cluster más próximo */
         error += min_distance;
      }


	 /* se actualizan todos los centroides, dividiendo la suma de coordenadas
	  de los puntos del cluster (almacenada en c1), entre el número de elementos
	  del cluster (almacenado en counts), y dejando el resultado en c */
      for (i = 0; i < k; i++)   // para cada cluster (CLUSTERS QUIERO)
      { /* update all centroids */
         for (j = 0; j < m; j++)  // para cada clave (colores)
         {
            c[i][j] = counts[i] ? c1[i][j] / counts[i] : c1[i][j];
            lucis[number_luci][i][j]=c[i][j];
         }
      }

   } while (fabs(error - old_error) > t);


	centroids = c;


	//gettimeofday(&tf_a, NULL);   // para calcularlo en usegundos

	//tiem_a0 = (tf_a.tv_sec - ti_a.tv_sec)*1000 +(tf_a.tv_usec - ti_a.tv_usec)/1000;



	// --- CALCULO DEL ERROR CUADŔATICO MEDIO Y VUELCO EL RESULTADO AL FICHERO ---
	AQE=0.0;

   // se intenta abrir para lectura el fichero. En caso de error se acaba
   fd = Abrir_fichero(nombre_fich_salida, "wt");

   /* la primera linea del fichero indica el número de filas y columnas
   de la matriz */
   fprintf(fd,"%d %d", N, R);

    
	// ------------------->
   for(i=0; i<n_docs; i++)
   {
	   // acumulo el error para este pixel
	   r1 = d[i][0] - (int)centroids[labels[i]][0] ;
	   r2 = d[i][1] - (int)centroids[labels[i]][1] ;
	   r3 = d[i][2] - (int)centroids[labels[i]][2] ;
	   AQE += ( r1*r1 + r2*r2  + r3*r3);
        
	   // escribo en disco el pixel de la imagen cuantizada
       fprintf(fd, "\n%d %d %d", (int)centroids[labels[i]][0],
         (int)centroids[labels[i]][1],(int)centroids[labels[i]][2]); //todo un pixel
	}
    


   fclose(fd);  // se cierra el fichero

   AQE = (AQE/N);
    
   brillos[number_luci]=(float)1.0/ AQE;//El brillo sera igual a la inversa del error cuadratico medio


   for (i = 0; i < k; i++) // para cada cluster...
   {
      if (!centroids)
         free(c[i]);

		free(c1[i]);
   }

   if (!centroids)
      free(c);

   free(c1);
   free(counts);

   return labels;
}



/* *******************************************************************************
 * llama al algoritmo k-means pasándole como centroides iniciales varios
 * puntos elegidos al azar
************************************************************************************/
void k_medias_aleatorio_X(int n_docs, int n_clusters, int n_claves, int X,int aleatorio,int number_luci) 
{
    
    float **centroids; /* centroides para pasar a k-medias */
    int *candidato =NULL;    /* indices de los items que se tomarán como centroides */
    int i, k,j;
    int alea;
    struct timeval ti_a, tf_a;   // para la aplicación del algoritmo de hormigas
    unsigned long long tiem_a;  // para almacenar la diferencia entre los dos tiempos previos
   

	// --1-- DEFINIR CENTROIDES INICIALES PARA K-MEANS
	// reservo memoria para los centroides que voy a pasar a k-medias
    
    centroids = RESERVAR_MAT_REAL(n_clusters, n_claves);
    
    //valores aleatorios
	if(aleatorio==1){// mejora para elegir centroides aleatorios directamente aquí
  
       for(i=0; i<n_clusters; i++)
       {
	       	alea = rand() % n_docs;
	        for (k = n_claves; k-- > 0; centroids[i][k] = d[alea][k]);
       }
   }
   //valores no aleatorios
   else{
    
        for(j=0;j<n_clusters;j++){
                centroids[j][0]=lucis[number_luci][j][0];
                centroids[j][1]=lucis[number_luci][j][1];
                centroids[j][2]=lucis[number_luci][j][2];
                
            }
    
   }
   // --2-- LLAMAR A K-MEANS
   int *c = k_means(d, n_docs, n_claves, n_clusters, ERROR, NULL, centroids, aleatorio,number_luci);

	// --3-- LIBERAR MEMORIA DINÁMICA
   free(c);


   LIBERAR_MAT_REAL(&centroids, n_clusters); // para k_means_MARISA

   if(candidato != NULL)
		free(candidato);

	
}
/************************************************************************************************
* funcion que aplica el kmeans al fichero de pixeles que le pasamos como argumento
*************************************************************************************************/
void aplicar_kmeans(int argc, char **argv,int number_luci,int aleatorio){
    
   
    char nombre_fich[200];  /* fichero que contiene los pixels de la imagen original */
    char cad[42];			// para el nombre del fichero de salida

    time_t t;         // para iniciar el generador de números aleatorios
    int pruebas;     // contador de ejecuciones independientes realizadas de k-means

    // para calcular el tiempo empleado en los cálculos
    // --- para calcular tiempo en milisegundos
    struct timeval ti_a, tf_a;   // instantes inicial y final
    unsigned long long tiem_a;   // para almacenar la diferencia entre los dos tiempos previos

    struct tm *instante_actual;
    time_t momento;

   // se inicia el generador de números aleatorios en función del tiempo
   srand((unsigned) time(&t));

   // se define el nombre de los ficheros en los que se escribirán estadísticos
   // instante actual
   time(&momento);

    // se convierte el valor para poder extraer información
    instante_actual = localtime(&momento);

    /* cad será una cadena que incluye el dia (2 cifras), el mes abreviado, el
    año, un guion bajo, la hora (2 cifras), los minutos y los segundos actuales,
     un punto seguido de 'txt' */
    strftime(cad, 42,"%d%b%y_%H%M%S.txt", instante_actual);

    // nombre del fichero para almacenar el resultado del algoritmo
    strcpy(nombre_fich_salida,"salida_luci_");
    strcat(nombre_fich_salida, argv[1]);
  
   // -------------------------------------------------------
   // -- SE DETERMINAN LOS VALORES A UTILIZAR PARA LOS CÁLCULOS --
   // -------------------------------------------------------
   // -- LECTURA DE PARAMETROS PARA LA EJECUCION --
    /* si se paso al menos un argumento, se toma como nombre del fichero de
    datos sobre los articulos. Es obligatorio */
    if(argc >=2)
	    strcpy(nombre_fich, argv[1]);
    else
    {
	    printf("\nDebe indicar el nombre del fichero que contiene los pixels");
	    exit(1);
    }

    // se leen los pixels del fichero de entrada
    LEER_M_REAL(nombre_fich, &d, &N, &R);

    // se realizan varias ejecuciones de k-means para poder obtener medias
    for(pruebas=0; pruebas <N_TESTS; pruebas++)
    {
	    

       // -------------------------------------------------------
       // -- APLICO K-MEDIAS --
       // -------------------------------------------------------
	    k_medias_aleatorio_X( N, n_clusters_quiero, R, 5,aleatorio,number_luci);
    }
    
       // -------------------------------------------------------
       // -- LIBERACION DE LA MEMORIA DINAMICA UTILIZADA --
       // -------------------------------------------------------
       LIBERAR_MAT_REAL(&d, N);     // la que ocupa la matriz de documentos

       
}

/**
*Esta función calcula la distancia euclidea entre el centroide de una imagen y el pixel
**/
float distancia(int i,int j){
    int k;
    float suma=0;
    for(k=0;k<N_CLUSTERS;k++){
        suma+=sqrt(pow((lucis[i][k][0]-lucis[j][k][0]),2)+pow((lucis[i][k][1]-lucis[j][k][1]),2)+pow((lucis[i][k][2]-lucis[j][k][2]),2));
    }
    suma*=(float)(1/(255*N_CLUSTERS));
    return suma;
}
/**Funcion que mira si hay una luciernaga mas brilante que lj ,
si no hay una luciernaga mas brillante que lj entonces lj se mueve aleatoriamente
**/
int mirar_mas_brillante(int j){
    int i;
    
    for(i=0;i<N_LUCI;i++){
        
        if(lucis[i]>lucis[j]) return 0;
    }
    return 1;
    
 
}

/**Funcion que calcula el valor aleatorio para calcular el movimiento de la luciernaga
**/
float aleatorio(){
       srand (time(NULL)); 
    return rand()%10-5; //valores aleatorios de -5 a 5
}

/**
* Aplicación del algoritmo de la luciernaga a la cuantificación de los colores de la imagen con k-means
**/
int aplicar_luciernaga(int argc, char **argv){
    int i,j,k;
 
    int beta=1,gamma=1; //valores iniciales de beta y gamma
    int iteraccion=0;
   
      /**OBTENEMOS LOS VALORES INICIALES DE LOS CENTROIDES ALEATORIOS DE LA LUCIERNAGA Y DEL BRILLO**/
         for(i=0;i<N_LUCI;i++){ 
                 printf("\nLucieraga<%d>\n",i);
                printf(" ->Establecemos los centroides iniciales a través de k-means\n");
                   aplicar_kmeans(argc,argv,i,1); //aplicar k-means con los valores iniciales aleatorios
                for(j=0;j<N_CLUSTERS;j++){
                printf("%d %d %d\n",(int)lucis[i][j][0],(int)lucis[i][j][1],(int)lucis[i][j][2]);
                
                }
                printf("\t Brillo %f\n",brillos[i]);
         }
      while(iteraccion<ITERACCIONES_LUC){

        /**MOVIMIENTOS DE LA LUCIERNAGA**/
        for(i=0;i<N_LUCI;i++){
            for(j=0;j<N_LUCI;j++){
   
                if(brillos[j]>brillos[i]){ /**MOVEMOS LA LUCIERNAGA I HACIA J**/
                    for(k=0;k<N_CLUSTERS;k++){ //calcular movimiento
                         lucis[i][k][0]=lucis[i][k][0]+beta*pow(E,-gamma*pow(distancia(i,j),2))+aleatorio();
                         lucis[i][k][1]=lucis[i][k][1]+beta*pow(E,-gamma*pow(distancia(i,j),2))+aleatorio();
                         lucis[i][k][2]=lucis[i][k][2]+beta*pow(E,-gamma*pow(distancia(i,j),2))+aleatorio();
                         
                    }
                
                    
                }
                /**SUMA DEL NÚMERO ALEATORIO AL MAS BRILLANTE (I) SI LO HAY**/
                if(mirar_mas_brillante(i)==1){ /**Si no existe una luciernaga mas brillante que la i entonces esta se mueve aleatoriamente sumandole un valor aleatorio**/
                         lucis[i][k][0]=lucis[i][k][0]+beta*aleatorio();
                         lucis[i][k][1]=lucis[i][k][1]+beta*aleatorio();
                         lucis[i][k][2]=lucis[i][k][2]+beta*aleatorio();
                }
                
            }
        }
          /**VUELVO A CALCULAR K-MEANS UTILIZANDO LOS CENTROIDES DE CADA LUCIERNAGA NUEVOS**/
        for(i=0;i<N_LUCI;i++){ 
                     printf("Lucieraga<%d> nº iteraccion → %d \n",i,iteraccion);
                   printf("Aplicamos de nuevo k-means\n");
                  aplicar_kmeans(argc,argv,i,0);//aplicamos de nuevo k-medias con los centroides obtenidos anteriormente
                for(j=0;j<N_CLUSTERS;j++){
                    
                    printf("%d %d %d\n",(int)lucis[i][j][0],(int)lucis[i][j][1],(int)lucis[i][j][2]);
                
                }
                printf("\t Brillo %f\n",brillos[i]);
               
         }
         iteraccion++;
      }
    return 0;
}

/**FUNCION PRINCIPAL**/
int main(int argc, char **argv){
    struct timeval t1,t2; 
    unsigned long long dif_tiempo;  
    gettimeofday(&t1, NULL);   
    aplicar_luciernaga(argc,argv);
    gettimeofday(&t2, NULL);   
    dif_tiempo = (t2.tv_sec - t1.tv_sec);
    printf("\tTiempo de procesamiento: %llu segundos\n",dif_tiempo);
    return 0;
}
