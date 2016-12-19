/* **********************************************************
*                         txt2tif.c  
* Genera una imagen tiff a partir de un fichero de texto que contiene los
* valores RGB del los pixles de una imagen.
*  RGB texto ----> imagen tiff
*

* **********************************************************
*
* COMPILACION:
*	  gcc txt2tif.c -ltiff -o txt2tif
*
* EJECUCION:  
*	./txt2tif [fich_texto_RGB] 
*
* [fich_texto_RGB]: fichero que contiene enteros correspondientes a los
*    valores RGB de un conjunto de pixels. En la primera línea contendrá
*    el tamaño de la imagen (anchura y altura)
*    x y
*     R1   G1   B1
*     ...
*     Rn   Gn    Bn
*  siendo n = x*y 
*
*  Se pueden indicar varios ficheros de entrada.
*
*
* Genera un fichero de salida por cada fichero de entrada:
*     [fich_texto_RGB]_txt2tif.tif
*
* **********************************************************											
********************************************************** */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "tiffio.h"


uint16 bps,spp;
#include "codigo_tif.h"


unsigned short p[4096];

int main(int argc, char *argv[])
{
  int x,y,  
	   i,j;  
  unsigned char *buf,
	             outfile[1024];  
  FILE *f;       
  char buf2[256], 
	    *pbuf;

	
  for(i=1;i<argc;++i)
  {
    printf("Procesando fichero %s\n",argv[i]);

	  
    f=fopen(argv[i],"rt");
    if(f==0)
	  { 
		  printf("\nERROR al abrir el fichero %s", argv[i]);
		  continue; 
	  }
	  
    *buf2=0;

    fgets(buf2,200,f);  
    x = strtoul(buf2, &pbuf,10);
    y = strtoul(pbuf, NULL,10);
	  
    if( (x>0)  &&  (y>0) )
    {
      printf("Ancho: %d, Alto: %d\n",x,y);

       buf = (unsigned char *)calloc(x*y,3*sizeof(unsigned char));
		 if (buf == NULL)
		 {
			 printf("\nERROR al reservar memoria trabajando con el fichero %s", argv[i]);
			 continue;
		 }
		 
      for(j=0; j<3*x*y; )
      {
        *buf2=0;
			
        fgets(buf2,200,f);

        buf[j++]=255 -(unsigned char)strtoul(buf2,&pbuf,10);
        buf[j++]=255- (unsigned char)strtoul(pbuf,&pbuf,10);
        buf[j++]=255-(unsigned char)strtoul(pbuf,&pbuf,10);
      }
		 
      printf("generado buffer\n");fflush(stdout);


      strcpy(outfile, argv[i]);
      strcat(outfile,"_txt2tif.tif");		 
      printf("creando %s\n",outfile); fflush(stdout);

      writetif(outfile,x,y,buf);

      free(buf);
    }
	  
    fclose(f); 
  }
}
