/*------------------------------------------------------*/
/* Prog    : Tp2_IFT3205-2-2.c                          */
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
#define NAME_IMG_IN1    "UdM_1"
#define NAME_IMG_IN2    "UdM_2"
#define NAME_IMG_OUT1   "image-TpIFT3205-2-1a"
#define NAME_IMG_OUT2   "image-TpIFT3205-2-1b"

/*------------------------------------------------*/
/* PROTOTYPE DE FONCTIONS  -----------------------*/   
/*------------------------------------------------*/

/*------------------------------------------------*/
/* PROGRAMME PRINCIPAL   -------------------------*/                     
/*------------------------------------------------*/
int main(int argc,char **argv)
 {
  int length, width;
  char BufSystVisuImg[100];
  float** imgM1, ** imgR1, ** imgI1;
  float** imgM2, ** imgR2, ** imgI2;

  LoadImg(NAME_IMG_IN1, &imgR1, &imgI1, &imgM1, &length, &width);
  LoadImg(NAME_IMG_IN2, &imgR2, &imgI2, &imgM2, &length, &width);

  FFTDD(imgR1, imgI1, length, width);
  FFTDD(imgR2, imgI2, length, width);

  Mod(imgM1, imgR1, imgI1, length, width);
  Mod(imgM2, imgR2, imgI2, length, width);
  RCenterImg(&imgM1, width, length);
  RCenterImg(&imgM2, width, length);
  Recal(imgM1, length, width);
  Recal(imgM2, length, width);
  Mult(imgM1, 100, length, width);
  Mult(imgM2, 100, length, width);

  SaveImagePgm(NAME_IMG_OUT1, imgM1, length, width);
  SaveImagePgm(NAME_IMG_OUT2, imgM2, length, width);

  FreeImg(imgR1, imgI1, imgM1);
  FreeImg(imgR2, imgI2, imgM2);

  //Commande systeme: VISU
  strcpy(BufSystVisuImg,NAME_VISUALISER);
  strcat(BufSystVisuImg,NAME_IMG_OUT1);
  strcat(BufSystVisuImg,".pgm&");
  printf(" %s",BufSystVisuImg);
  system(BufSystVisuImg);
  strcpy(BufSystVisuImg,NAME_VISUALISER);
  strcat(BufSystVisuImg,NAME_IMG_OUT2);
  strcat(BufSystVisuImg,".pgm&");
  printf(" %s",BufSystVisuImg);
  system(BufSystVisuImg);


  //==End=========================================================

  //retour sans probleme
  printf("\n Question 1 terminée avec succès! ... \n\n");
  return 0; 	 
}


