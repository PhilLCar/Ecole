/*------------------------------------------------------*/
/* Prog    : Tp3_IFT3205-2-1a.c                         */
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
#define NAME_IMG_IN  "lena128"
#define NAME_IMG_OUT "image-TpIFT3205-2-1a"

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

  //Lecture Image 
  float** img = LoadImagePgm(NAME_IMG_IN,&length,&width);
  float** target = fmatrix_allocate_2d(length * 4, width * 4);

  //Interpolation
  stretch0(img, width, length, target, width * 4, length * 4);
 
  //Sauvegarde
  SaveImagePgm(NAME_IMG_OUT, target, 4 * length, 4 * width);

  //Commande systeme: VISU
  strcpy(BufSystVisuImg,NAME_VISUALISER);
  strcat(BufSystVisuImg,NAME_IMG_OUT);
  strcat(BufSystVisuImg,".pgm&");
  printf(" %s",BufSystVisuImg);
  system(BufSystVisuImg);
 


  //==End=========================================================

  //Liberation memoire 
  free_fmatrix_2d(img);
  free_fmatrix_2d(target);  
 
  //retour sans probleme
  printf("\n C'est fini ... \n\n");
  return 0; 	 
}

