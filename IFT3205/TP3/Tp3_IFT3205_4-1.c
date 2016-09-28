/*------------------------------------------------------*/
/* Prog    : Tp3_IFT3205-4-1.c                         */
/* Auteur  : Philippe Caron                             */
/* Date    : 11/03/2010                                 */
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

#include "FonctionDemo3.h"
#include "IEP.c"

/*------------------------------------------------*/
/* DEFINITIONS -----------------------------------*/   
/*------------------------------------------------*/
#define NAME_VISUALISER "display "
#define NAME_IMG_IN     "ParcMontRoyal"
#define NAME_IMG_OUT    "image-TpIFT3205-4-1"

/*------------------------------------------------*/
/* PROTOTYPE DE FONCTIONS  -----------------------*/   
/*------------------------------------------------*/


/*------------------------------------------------*/
/* PROGRAMME PRINCIPAL   -------------------------*/                     
/*------------------------------------------------*/
int main(int argc,char **argv)
 {
  int i,j;
  int length,width;
  char BufSystVisuImg[100];
  int extraval = 50;

  //Lecture Image 
  float** img = LoadImagePgm(NAME_IMG_IN,&length,&width);
  float** new = fmatrix_allocate_2d(length + extraval, width);

  //Interpolation
  extrapolate(img, new, width, length, extraval);

  SaveImagePgm(NAME_IMG_OUT, new, length + extraval, width);

  //Commande systeme: VISU
  strcpy(BufSystVisuImg,NAME_VISUALISER);
  strcat(BufSystVisuImg,NAME_IMG_IN);
  strcat(BufSystVisuImg,".pgm&");
  printf(" %s",BufSystVisuImg);
  system(BufSystVisuImg);

  strcpy(BufSystVisuImg,NAME_VISUALISER);
  strcat(BufSystVisuImg,NAME_IMG_OUT);
  strcat(BufSystVisuImg,".pgm&");
  printf(" %s",BufSystVisuImg);
  system(BufSystVisuImg);
 

  //==End=========================================================

  //Liberation memoire 
  free_fmatrix_2d(img);
 
  //retour sans probleme
  printf("\n C'est fini ... \n\n");
  return 0; 	 
}


