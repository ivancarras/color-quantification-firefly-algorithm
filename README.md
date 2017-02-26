# Color quantification using firefly algorithm
Color quantification using firefly algorithm  
The objetive of this project is the color image reduction through the firefly algorithm with the k-means. 
You can change the fireflys number and the iteractions of k-means.

#Requeriments
You need a c compiler and a text editor for choose your configuration of fireflies and k-means iteractions
The pixels of the image in a txt document, that is to say, you must have the RGB values.

#To prove it
I have two programas:
-txt2tifmod: this program transform the pixel values to image of format tiff
-luci8: this program use the firefly algorithm and k-means for reduce the color of the image to 8 bits per pixel.

You can modify them and compile them with:

  TXT2TIFMOD
  
  	gcc txt2tif.c -ltiff -o txt2tif
	execute: ./txt2tif [fich_img_RGB] 
  LUCI8
  
    	gcc -o luci kmeans_imagenes.c -L. -lkmeans -lm
	execute: luci [fich_pixels]
