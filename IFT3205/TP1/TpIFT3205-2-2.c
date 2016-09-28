/*------------------------------------------------------*/
/* Prog    : TpIFT3205-2-2.c                            */
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
#define NAME_IMG_OUT    "image-TpIFT3205-2-2"

/*------------------------------------------------*/
/* PROTOTYPE DE FONCTIONS  -----------------------*/   
/*------------------------------------------------*/
void LoadImg(char*, float***, float***, float***, int*, int*);
void FreeImg(float**, float**, float**);
void IsolateFrequency(float**, int, int, float, float);
void Normalize(float**, int, int, float);
void CenterImg(float***, int, int);

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
  
  IsolateFrequency(imgR, width, length, 0.005, 0.01);
  IsolateFrequency(imgI, width, length, 0.005, 0.01);
  
  CenterImg(&imgR, width, length);
  CenterImg(&imgI, width, length);
  
  IFFTDD(imgR, imgI, length, width);
  
  Recal(imgR, length, width);
  Normalize(imgR, length, width, 0.6);
  
  SaveImagePgm(NAME_IMG_OUT, imgR, length, width);

  FreeImg(imgR, imgI, imgM);   

  /*Commande systeme: visualisation de Ingout.pgm*/
  strcpy(BufSystVisuImg,NAME_VISUALISER);
  strcat(BufSystVisuImg,NAME_IMG_OUT);
  strcat(BufSystVisuImg,".pgm&");
  printf(" %s",BufSystVisuImg);
  system(BufSystVisuImg);
  
  /*retour sans probleme*/ 
  printf("\nNuméro 2.2 terminé avec succès\n\n");
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

/* Normalize
 * Éparpille les tons de gris avec un facteur f, si f <1
 * donne un effet de baisse de contraste et une augmentation
 * quand f > 1
 */
void Normalize(float** img, int width, int height, float f)
{
  int i, j;
  for (i = 0; i < width; i++)
    for (j = 0; j < height; j++) {
      img[i][j] = (img[i][j] - 127.0) * f + 127.0;
      /*if (img[j][i] > 255) img[j][i] = 255;
	else if (img[j][i] < 0) img[j][i] = 0;*/
    } 
}

/*-------------------2D FILTERS-------------------*/
/* Isolate Frequency
 * Isole une fréquence très précise et ne conserve que celle-ci
 */
void IsolateFrequency(float** img, int width, int height, float horizontal, float vertical)
{
  int hfreq = -horizontal * width / 2 + width / 2;
  int vfreq = -vertical * height / 2 + height / 2;
  int i, j;
  for (i = 0; i < width; i++)
    for (j = 0; j < height; j++)
      if (i != hfreq || j != vfreq) img[j][i] = 0;
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
