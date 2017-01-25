/*------------------------------------------------------*/
/* Prog    : TpIFT3205-2-1b.c                           */
/* Auteur  : Philippe Caron                             */
/* Date    : 11/02/2016                                 */
/* version : N/A                                        */ 
/* langage : C                                          */
/* labo    : DIRO                                       */
/*------------------------------------------------------*/

/*------------------------------------------------*/
/* FICHIERS INCLUS -------------------------------*/
/*------------------------------------------------*/
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "FonctionDemo1.h"

/*------------------------------------------------*/
/* DEFINITIONS -----------------------------------*/   
/*------------------------------------------------*/
#define NAME_VISUALISER "display "
#define NAME_IMG_IN     "photograph"
#define NAME_IMG_OUT    "image-TpIFT3205-2-1b"

#define FILTER_OUT 0
#define FILTER_IN 1

/*------------------------------------------------*/
/* PROTOTYPE DE FONCTIONS  -----------------------*/   
/*------------------------------------------------*/
void LoadImg(char*, float***, float***, float***, int*, int*);
void FreeImg(float**, float**, float**);
float sqr(float);
void CenterImg(float***, int, int);
void RoundFilter(float**, int, int, int, int, float, int);

/*------------------------------------------------*/
/* PROGRAMME PRINCIPAL   -------------------------*/                     
/*------------------------------------------------*/
int main(int argc,char **argv)
{
  int length, width;
  float** imgR;
  float** imgI;
  float** imgM;
  char path[100];
  char BufSystVisuImg[100];

  LoadImg(NAME_IMG_IN, &imgR, &imgI, &imgM, &length, &width); 

  FFTDD(imgR, imgI, length, width);

  CenterImg(&imgR, width, length);
  CenterImg(&imgI, width, length);

  RoundFilter(imgR, width, length, 7, 7, 0, FILTER_OUT);
  RoundFilter(imgI, width, length, 7, 7, 0, FILTER_OUT);

  CenterImg(&imgR, width, length);
  CenterImg(&imgI, width, length);

  IFFTDD(imgR, imgI, length, width);

  Recal(imgR, length, width);
  SaveImagePgm(NAME_IMG_OUT, imgR, length, width);

  FreeImg(imgR, imgI, imgM);

  /*Commande systeme: visualisation de Ingout.pgm*/
  strcpy(BufSystVisuImg,NAME_VISUALISER);
  strcat(BufSystVisuImg,NAME_IMG_OUT);
  strcat(BufSystVisuImg,".pgm&");
  printf(" %s",BufSystVisuImg);
  system(BufSystVisuImg);

  
  /*retour sans probleme*/ 
  printf("\nNuméro 2.1 b) terminé avec succès\n\n");
  return 0; 	 
}

/*---------------------UTILS----------------------*/
void LoadImg(char* img_name, float*** imgR, float*** imgI, float*** imgM, int* length, int* width)
{
  int i, j;
  *imgR = LoadImagePgm(img_name, length, width);
  *imgI = fmatrix_allocate_2d(*length, *width);
  *imgM = fmatrix_allocate_2d(*length, *width);
  for(i = 0; i < *length; i++) 
    for(j = 0; j < *width; j++) 
      {
        (*imgI)[i][j] = 0.0;
	(*imgM)[i][j] = 0.0;
      }
}

void FreeImg(float** imgR, float** imgI, float** imgM)
{
  free_fmatrix_2d(imgR);
  free_fmatrix_2d(imgI); 
  free_fmatrix_2d(imgM);
}

float sqr(float x)
{
  return x * x;
}

/*------------FONTCTION DE RECENTRAGE-------------*/
void CenterImg(float*** img, int width, int height)
{
  float** tmp = fmatrix_allocate_2d(height, width);
  int hcenter = width / 2;
  int vcenter = height / 2;
  int i, j;
  for (i = 0; i < width; i++)
    for (j = 0; j < height; j++)
      if (i < hcenter)
	if (j < vcenter)
	  tmp[j][i] = (*img)[j + vcenter][i + hcenter];
	else
	  tmp[j][i] = (*img)[j - vcenter][i + hcenter];
      else
  	if (j < vcenter)
	  tmp[j][i] = (*img)[j + vcenter][i - hcenter];
	else
	  tmp[j][i] = (*img)[j - vcenter][i - hcenter];
  free_fmatrix_2d(*img);
  *img = tmp;
}

/* Round Filter
 * Cette fonction défini un cercle puis en fonction du flag FILTER_IN ou FILTER_OUT
 * multiplie l'intérieur ou l'extérieur respectivement par le facteur mult
 */
void RoundFilter(float** img, int iwidth, int iheight, int fwidth, int fheight, float mult, int flags)
{
  int hcenter = iwidth / 2;
  int vcenter = iheight / 2;
  int i, j;
  if (flags & 1)
    {
       for (i = 0; i < iwidth; i++)
	for (j = 0; j < iheight; j++)
	  if (1 >= sqrt(sqr((i - hcenter) / (float)fwidth) + sqr((j - vcenter) / (float)fheight)))
	    img[j][i] *= mult;
    }
  else
    for (i = 0; i < iwidth; i++)
	for (j = 0; j < iheight; j++)
	  if (1 < sqrt(sqr((i - hcenter) / (float)fwidth) + sqr((j - vcenter) / (float)fheight)))
	    img[j][i] *= mult;
}
