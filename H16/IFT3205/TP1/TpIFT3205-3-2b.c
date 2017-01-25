/*------------------------------------------------------*/
/* Prog    : TpIFT3205-3-2b.c                           */
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
#define NAME_IMG_IN     "Monrstein"
#define NAME_IMG_OUT    "image-TpIFT3205-3-2b"

#define FILTER_OUT 0
#define FILTER_IN 1
#define FILTER_POSITIVE 8

/*------------------------------------------------*/
/* PROTOTYPE DE FONCTIONS  -----------------------*/   
/*------------------------------------------------*/
void LoadImg(char*, float***, float***, float***, int*, int*);
void FreeImg(float**, float**, float**);
void Pnormalize(float**, int, int, float);
float sqr(float);
void CenterImg(float***, int, int);
void ConeFilter(float**, int, int, int, int, float(*f)(float, float), int);
float Square(float, float);

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
  
  ConeFilter(imgR, width, length, width * 0.5, length * 0.5, Square, FILTER_IN);
  ConeFilter(imgI, width, length, width * 0.5, length * 0.5, Square, FILTER_IN);
  
  CenterImg(&imgR, width, length);
  CenterImg(&imgI, width, length);
  
  IFFTDD(imgR, imgI, length, width);
  
  Recal(imgR, length, width);
  Pnormalize(imgR, width, length, 0.5);
  
  SaveImagePgm(NAME_IMG_OUT, imgR, length, width);

  FreeImg(imgM, imgR, imgI);   

  /*Commande systeme: visualisation de Ingout.pgm*/
  strcpy(BufSystVisuImg,NAME_VISUALISER);
  strcat(BufSystVisuImg,NAME_IMG_OUT);
  strcat(BufSystVisuImg,".pgm&");
  printf(" %s",BufSystVisuImg);
  system(BufSystVisuImg);

  /*retour sans probleme*/ 
  printf("\nNuméro 3.2 b) terminé avec succès\n\n");
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

/* Power Normalize
 * Centre l'image autour de zéro pour éparpille les valeurs de gris
 * selon une fonction de puissance, appel Recal automatiquement sur
 * l'image par la suite
 */
void Pnormalize(float** img, int width, int height, float f)
{
  int i, j;
  for (i = 0; i < width; i++)
    for (j = 0; j < height; j++) {
      img[i][j] = pow(img[i][j] - 127.0, f);
      if (img[i][j] != img[i][j]) img[i][j] = 0;
    }
  Recal(img, height, width);
}

float sqr(float n)
{
  return n * n;
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

/*-------------------3D FILTERS-------------------*/
/* Cone Filter
 * Cette fonction défini un cercle puis en fonction du flag FILTER_IN ou FILTER_OUT
 * multiplie l'intérieur ou l'extérieur respectivement par le facteur calculé par
 * la fonction func passée en paramètre, celui-ci peut être inversé par le flag
 * FILTER_POSITIVE
 */
void ConeFilter(float** img, int iwidth, int iheight, int fwidth, int fheight, float (*func)(float, float), int flags)
{
  int hcenter = iwidth / 2;
  int vcenter = iheight / 2;
  int pos = flags >> 3;
  float stop;
  if (pos) stop = sqrt(sqr(hcenter) + sqr(vcenter));
  else stop = sqrt(sqr(fwidth / 2) + sqr(fheight / 2));
  int i, j;
  if (flags & 1)
    {
       for (i = 0; i < iwidth; i++)
	for (j = 0; j < iheight; j++)
	  if (1 >= sqrt(sqr((i - hcenter) / (float)fwidth) + sqr((j - vcenter) / (float)fheight)))
	    if (pos) img[j][i] *= 1 - func(sqrt(sqr(i - hcenter) + sqr(j - vcenter)), stop);
	    else img[j][i] *= func(sqrt(sqr(i - hcenter) + sqr(j - vcenter)), stop);
    }
  else
    for (i = 0; i < iwidth; i++)
	for (j = 0; j < iheight; j++)
	  if (1 < sqrt(sqr((i - hcenter) / (float)fwidth) + sqr((j - vcenter) / (float)fheight)))
	    if (pos) img[j][i] *= 1 - func(sqrt(sqr(i - hcenter) + sqr(j - vcenter)), stop);
	    else img[j][i] *= func(sqrt(sqr(i - hcenter) + sqr(j - vcenter)), stop);
}

/*------------------Z FUNCTIONS-------------------*/
/* Square
 * Définie une progression carrée pour les filtres tri-dimensionnels
 */
float Square(float r, float stop)
{
  return sqr(r) / sqr(stop);
}
