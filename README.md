# Color quantification using firefly algorithm
![alt tag](https://github.com/ivancarras/color-quantification-firefly-algorithm/blob/master/lenna_reduction.PNG)

Color quantification using firefly algorithm  
The objetive of this project is the color image reduction through the firefly algorithm with the k-means. 
You can change the fireflys number and the iteractions of k-means.

# Requeriments
You need a c compiler and a text editor for choose your configuration of fireflies and k-means iteractions
The pixels of the image in a txt document, that is to say, you must have the RGB values.

# To prove it
I have two programs:
-txt2tifmod: this program transform the pixel values to image of format tiff
-luci8: this program use the firefly algorithm and k-means for reduce the color of the image to 8 bits per pixel.

You can modify them and compile them with:

  TXT2TIFMOD
  
  	gcc txt2tif.c -ltiff -o txt2tif
	execute: ./txt2tif [fich_img_RGB] 
  LUCI8
  
    	gcc -o luci kmeans_imagenes.c -L. -lkmeans -lm
	execute: luci [fich_pixels]
## REDUCTION OF ORIGINAL TO 16 BITS
![alt tag](https://github.com/ivancarras/color-quantification-firefly-algorithm/blob/master/plane_reduction.PNG)
![alt tag](https://github.com/ivancarras/color-quantification-firefly-algorithm/blob/master/monkey_reduction.PNG)
![alt tag](https://github.com/ivancarras/color-quantification-firefly-algorithm/blob/master/lenna_reduction.PNG)
![alt tag](https://github.com/ivancarras/color-quantification-firefly-algorithm/blob/master/lake_reduction.PNG)
![alt tag](https://github.com/ivancarras/color-quantification-firefly-algorithm/blob/master/Peppers_reduction.PNG)
![alt tag](https://github.com/ivancarras/color-quantification-firefly-algorithm/blob/master/blonde_girl_reduction.PNG)

Explanation of the Algorithm
[A Hybrid Approach for Color Image Quantization Using K-means and Firefly Algorithms ](http://waset.org/publications/6965/a-hybrid-approach-for-color-image-quantization-using-k-means-and-firefly-algorithms)
