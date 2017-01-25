//------------------------------------------------------
// Prog    : Tp4_IFT3205-3-1.c                          
// Auteur  : Philippe Caron                                           
// Date    : 17/03/2016                                
// version : N/A                                            
// langage : C                                          
// labo    : DIRO                                       
//------------------------------------------------------

//------------------------------------------------
// FICHIERS INCLUS -------------------------------
//------------------------------------------------
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "FonctionDemo4.h"
#include "Weiner.c"

//------------------------------------------------
// DEFINITIONS -----------------------------------   
//------------------------------------------------
#define NAME_VISUALISER "display "
#define NAME_IMG_IN     "cameraman"
#define NAME_IMG_OUT    "image-TpIFT3205-3-1"

//------------------------------------------------
// PROTOTYPE DE FONCTIONS  -----------------------   
//------------------------------------------------

void CreateRectFunc(float**, int, int, int);

//------------------------------------------------
// VARIABLES GLOBALES  ---------------------------   
//------------------------------------------------

//>Parametre de degradation
int size=9;
float variance_noise=0.5;
int psf=1;

//---------------------------------------------------------
//---------------------------------------------------------
// PROGRAMME PRINCIPAL   ----------------------------------                     
//---------------------------------------------------------
//---------------------------------------------------------
int main(int argc,char **argv)
 {
  int width, length;
  char BufSystVisuImg[100];
  float **img1, **img2, **img3, **img4;
  
  //=========================================================
  //== PROG =================================================
  //=========================================================

  //>Lecture Image
  img1 = LoadImagePgm(NAME_IMG_IN, &length, &width);
  img2 = fmatrix_allocate_2d(length, width);
  img3 = fmatrix_allocate_2d(length, width);
  img4 = fmatrix_allocate_2d(length, width);

  //--------------------------------------------------------
  //>Degradation
  //--------------------------------------------------------
  // Cette fonction ajoute un flou créé par une psf uniforme 
  // (fonction porte) de taille sizexsize puis ajoute sur
  // cette image rendue floue, un bruit Gaussien de variance
  // variance_noise
  //
  //--------------------------------------------------------
  degradation(img1, img2, length, width, psf, size, variance_noise);
  


  //============
  //   WIENER
  //============
  // .....
  CreateRectFunc(img3, width, length, size);
  RCenterImg(&img3, width, length);

  CopyImg(img2, img4, width, length);

  Iterate(img1, img2, img3, img4, width, length, variance_noise, 20);
	  
  //----------------------------------------------
  // SAUVEGARDE et VISU
  // -------------------
  // Le resultat de la restoration > mat_rest
  // L'image dégradée              > mat
  // L'image non dégradée          > mat_img
  //----------------------------------------------
  //SaveImagePgm(NAME_IMG_OUT1,mat_rest,length,width);
  SaveImagePgm(NAME_IMG_OUT, img4, length, width);
  
  strcpy(BufSystVisuImg,NAME_VISUALISER);
  strcat(BufSystVisuImg,NAME_IMG_OUT);
  strcat(BufSystVisuImg,".pgm&");
  printf("\n > %s",BufSystVisuImg);
  system(BufSystVisuImg);
  
  
  //Liberation memoire pour les matrices
  FreeImg(img1, img2);
  FreeImg(img3, img4);

  //retour sans probleme 
  printf("\n C'est fini ... \n\n");
  return 0; 	 
}

void CreateRectFunc(float **img, int width, int height, int size)
{
  int i, j;
  int val = width * height / (size * size);

  for (i = 0; i < width; i++)
    for (j = 0; j < height; j++)
      if (i >= (float)(width / 2) - (float)(size / 2) && i <= (float)(width / 2) + (float)(size / 2) &&
	  j >= (float)(height / 2) - (float)(size / 2) && j <= (float)(height / 2) + (float)(size / 2))
	img[j][i] = val;
      else
	img[j][i] = 0;
}
