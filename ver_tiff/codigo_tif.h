/* **********************************************************
*                         codigo_tif.h
*
*  Código común a varios programas de procesamiento de imagenes
*
* writetif :
* readtif:
* ********************************************************** */


int writetif(char *filename, int x, int y, char buffer[])
{
  TIFF *image;
	
  if((image = TIFFOpen(filename, "w")) == NULL) 
		return -1;

  TIFFSetField(image, TIFFTAG_IMAGEWIDTH, x);
  TIFFSetField(image, TIFFTAG_IMAGELENGTH, y);
  TIFFSetField(image, TIFFTAG_BITSPERSAMPLE, 8);
  TIFFSetField(image, TIFFTAG_SAMPLESPERPIXEL, 3);
  TIFFSetField(image, TIFFTAG_ROWSPERSTRIP, y);

  TIFFSetField(image, TIFFTAG_COMPRESSION, COMPRESSION_LZW);
  TIFFSetField(image, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB);
  TIFFSetField(image, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);

  TIFFSetField(image, TIFFTAG_XRESOLUTION, 150.0);
  TIFFSetField(image, TIFFTAG_YRESOLUTION, 150.0);
  TIFFSetField(image, TIFFTAG_RESOLUTIONUNIT, RESUNIT_INCH);
  
  TIFFWriteEncodedStrip(image, 0, buffer, x * y * 3 ); // 3 byte/pixel

  TIFFClose(image);
  return 0;
}


int writetif1(char *filename,int x,int y,char buffer[])
{
  TIFF *image;

  if((image = TIFFOpen(filename, "w")) == NULL) return -1;

  TIFFSetField(image, TIFFTAG_IMAGEWIDTH, x);
  TIFFSetField(image, TIFFTAG_IMAGELENGTH, y);
  TIFFSetField(image, TIFFTAG_BITSPERSAMPLE, 8);
  TIFFSetField(image, TIFFTAG_SAMPLESPERPIXEL, 1);
  TIFFSetField(image, TIFFTAG_ROWSPERSTRIP, y);

  TIFFSetField(image, TIFFTAG_COMPRESSION, COMPRESSION_LZW);
  TIFFSetField(image, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISWHITE);
  TIFFSetField(image, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);

  TIFFSetField(image, TIFFTAG_XRESOLUTION, 150.0);
  TIFFSetField(image, TIFFTAG_YRESOLUTION, 150.0);
  TIFFSetField(image, TIFFTAG_RESOLUTIONUNIT, RESUNIT_INCH);
  
  TIFFWriteEncodedStrip(image, 0, buffer, x * y ); // 1 byte/pixel

  TIFFClose(image);
  return 0;
}

int readtif(char *filename, int *x, int *y, char **buf)
{
  TIFF *image;
  uint16 photo,fillorder;
  uint32 width,height;
  tsize_t stripSize;
  unsigned long imageOffset, result;
  int stripMax, stripCount;
  char *buffer, tempbyte;
  unsigned long bufferSize, count;

  if((image = TIFFOpen(filename, "r")) == NULL) 
		return -1;
	
  TIFFGetField(image, TIFFTAG_SAMPLESPERPIXEL, &spp);
  TIFFGetField(image, TIFFTAG_BITSPERSAMPLE, &bps);
	
  printf("Bits por muestra: %d, muestras por pixel: %d\n",bps,spp);

  if(TIFFGetField(image, TIFFTAG_IMAGEWIDTH, &width) == 0) 
		return -8;
	
  if(TIFFGetField(image, TIFFTAG_IMAGELENGTH, &height) == 0) 
		return -9;

  printf("ancho=%d, alto=%d\n",width,height);
  stripSize = TIFFStripSize (image);
  stripMax = TIFFNumberOfStrips (image);
  bufferSize = TIFFNumberOfStrips (image) * stripSize;
  printf("bufferSize: %lu\n",bufferSize);


  if(TIFFGetField(image, TIFFTAG_PHOTOMETRIC, &photo) == 0) return -4;
  printf("Photometric: %u\n",photo);
      
  imageOffset = 0;
  
  if((buffer = (char *) malloc(bufferSize)) == NULL) return -5;
  
  for (stripCount = 0; stripCount < stripMax; stripCount++)
  {
    if((result = TIFFReadEncodedStrip (image, stripCount,
				      buffer + imageOffset, stripSize)) == -1) 
		  return -6;
    imageOffset += result;
  }
	
  if(photo != PHOTOMETRIC_MINISBLACK)
	{
    for(count = 0; count < bufferSize; count++)
      buffer[count] = ~buffer[count];
  }
 
  TIFFClose(image);
  *x=width;
  *y=height;
  *buf=buffer;
  return 0;
}

void  aviso_error(int err)
{
	 switch(err)
    {

	   case -1: 
        fprintf(stderr, "Could not open incoming image\n");
        break;
      case -2: 
        fprintf(stderr, "Either undefined or unsupported number of bits per sample\n");
        break;
      case -3: 
        fprintf(stderr, "Either undefined or unsupported number of samples per pixel\n");
        break;
      case -4: 
        fprintf(stderr, "Image has an undefined photometric interpretation\n");
        break;
      case -5: 
        fprintf(stderr, "Could not allocate enough memory for the uncompressed image\n");
        break;
      case -6: 
        fprintf(stderr, "Read error on input\n");
        break;
      case -7: 
        fprintf(stderr, "Image has an undefined fillorder\n");
        break;
      case -8: 
        fprintf(stderr, "Image does not define its width\n");
        break;
      case -9: 
        fprintf(stderr, "Image does not define its heigth\n");
        break;
      default:
	    fprintf(stderr, "Error indeterminado\n");
	 }
}