/*------------------------------------------------------*/
/* Prog    : Tp2_IFT3205-2-1.c                          */
/* Auteur  : Philippe Caron                             */
/* Date    : 14/02/2016                                 */
/* version : 1                                          */ 
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

#include "FonctionDemo2.h"

#include "Traitement.c"

/*------------------------------------------------*/
/* DEFINITIONS -----------------------------------*/   
/*------------------------------------------------*/
#define NAME_VISUALISER "display "
#define NAME_IMG_IN     "lena"
#define NAME_IMG_OUT    "image-TpIFT3205-2-2.pgm"

/*------------------------------------------------*/
/* PROGRAMME PRINCIPAL   -------------------------*/                     
/*------------------------------------------------*/
int main(int argc,char **argv)
 {
  int length,width;
  char BufSystVisuImg[100];
  float** imgM, ** imgR, ** imgI;

  LoadImg(NAME_IMG_IN, &imgR, &imgI, &imgM, &length, &width);

  // Effectue la rotation par rapport au centre de l'image
  Rotate(&imgR, width, length, 0, 0, deg(22.5));

  SaveImagePgm(NAME_IMG_OUT, imgR, length, width);

  FreeImg(imgR, imgI, imgM);

  //Commande systeme: VISU
  strcpy(BufSystVisuImg,NAME_VISUALISER);
  strcat(BufSystVisuImg,NAME_IMG_OUT);
  strcat(BufSystVisuImg,".pgm&");
  printf(" %s",BufSystVisuImg);
  system(BufSystVisuImg);
  //==End=========================================================

  //retour sans probleme
  printf("\n Question 2 terminée avec succès! ... \n\n");
  return 0; 	 
}


