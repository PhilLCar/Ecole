/*------------------------------------------------------*/
/* Prog    : TpIFT3205-1-3.c                            */
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
#define NAME_IMG_IN_1A  "D1r"
#define NAME_IMG_IN_1B  "D11r"
#define NAME_IMG_IN_1C  "D46r"
#define NAME_IMG_OUT    "image-TpIFT3205-1-4"

/*------------------------------------------------*/
/* PROTOTYPE DE FONCTIONS  -----------------------*/   
/*------------------------------------------------*/
void LoadImg(char*, float***, float***, float***, int*, int*);
void FreeImg(float**, float**, float**);
void Log(float**, int, int);
void CenterImg(float***, int, int);
void num1(char*, char*);

/*------------------------------------------------*/
/* PROGRAMME PRINCIPAL   -------------------------*/                     
/*------------------------------------------------*/
int main(int argc,char **argv)
{
  char BufSystVisuImg[100];

  // 3 Sous-numéros du devoir
  // La fonction est définie à la fin du document
  num1("a", NAME_IMG_IN_1A);
  num1("b", NAME_IMG_IN_1B);
  num1("c", NAME_IMG_IN_1C); 


  /*Commande systeme: visualisation de Ingout.pgm*/
  strcpy(BufSystVisuImg,NAME_VISUALISER);
  strcat(BufSystVisuImg,"image-TpIFT3205-1-3.pgm&");
  printf(" %s",BufSystVisuImg);
  system(BufSystVisuImg);

  /*retour sans probleme*/ 
  printf("\nNuméros 1.3 et 1.4 terminés avec succès\n\n");
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

/* Fonction log pour rehausser l'image */
void Log(float** img, int width, int height)
{
  // Calcule la différence avec la nouvelle valeur maximale
  // pour éviter d'appeler Recal une seconde fois
  float factor = 255.0 / log(256);
  int i, j;
  for (i = 0; i < width; i++)
    for (j = 0; j < height; j++)
      img[i][j] = factor * log(1 + img[i][j]);
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

/*--------------------DEVOIR----------------------*/
void num1(char* num, char* img_name)
{
  int length, width;
  float** imgR;
  float** imgI;
  float** imgM;
  char path[100];
  
  LoadImg(img_name, &imgR, &imgI, &imgM, &length, &width);
  
  FFTDD(imgR, imgI, length, width);

  //3
  strcpy(path, NAME_IMG_OUT);
  strcat(path, num);
  Mod(imgM, imgR, imgI, length, width);
  Log(imgM, length, width);
  Recal(imgM, length, width);
  CenterImg(&imgM, length, width);
  SaveImagePgm(path, imgM, length, width);

  // UTILISE L'IMAGE A COMME IMAGE DE REMISE
  if (num == "a")
    SaveImagePgm("image-TpIFT3205-1-3", imgM, length, width);

  FreeImg(imgR, imgI, imgM);
}


