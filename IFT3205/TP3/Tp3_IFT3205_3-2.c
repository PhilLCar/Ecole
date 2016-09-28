/*------------------------------------------------------*/
/* Prog    : Tp3_IFT3205-3-2.c                         */
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
#define NAME_IMG_IN     "barbara512_Degraded"
#define NAME_IMG_OUT_0  "image-TpIFT3205-3-2-0"
#define NAME_IMG_OUT_1  "image-TpIFT3205-3-2-1"
#define NAME_IMG_OUT_2  "image-TpIFT3205-3-2-2"
#define NAME_IMG_OUT_3  "image-TpIFT3205-3-2-3"
#define NAME_IMG_OUT_4  "image-TpIFT3205-3-2-4"
#define NAME_IMG_OUT_5  "image-TpIFT3205-3-2-5"

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

  //Interpolation
  correct(img, width, length, 3, 0.02);

  //Commande systeme: VISU
  strcpy(BufSystVisuImg,NAME_VISUALISER);
  strcat(BufSystVisuImg,NAME_IMG_OUT_0);
  strcat(BufSystVisuImg,".pgm&");
  printf(" %s",BufSystVisuImg);
  system(BufSystVisuImg);

  strcpy(BufSystVisuImg,NAME_VISUALISER);
  strcat(BufSystVisuImg,NAME_IMG_OUT_1);
  strcat(BufSystVisuImg,".pgm&");
  printf(" %s",BufSystVisuImg);
  system(BufSystVisuImg);
  
  strcpy(BufSystVisuImg,NAME_VISUALISER);
  strcat(BufSystVisuImg,NAME_IMG_OUT_2);
  strcat(BufSystVisuImg,".pgm&");
  printf(" %s",BufSystVisuImg);
  system(BufSystVisuImg);

  strcpy(BufSystVisuImg,NAME_VISUALISER);
  strcat(BufSystVisuImg,NAME_IMG_OUT_3);
  strcat(BufSystVisuImg,".pgm&");
  printf(" %s",BufSystVisuImg);
  system(BufSystVisuImg);

  strcpy(BufSystVisuImg,NAME_VISUALISER);
  strcat(BufSystVisuImg,NAME_IMG_OUT_4);
  strcat(BufSystVisuImg,".pgm&");
  printf(" %s",BufSystVisuImg);
  system(BufSystVisuImg);

  strcpy(BufSystVisuImg,NAME_VISUALISER);
  strcat(BufSystVisuImg,NAME_IMG_OUT_5);
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


