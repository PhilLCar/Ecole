/*------------------------------------------------------*/
/* Prog    : Tp2_IFT3205-2-4.c                          */
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
#define NAME_IMG_OUT    "image-TpIFT3205-2-4"

/*------------------------------------------------*/
/* PROTOTYPE DE FONCTIONS  -----------------------*/   
/*------------------------------------------------*/
float CalcAngle(float** img1, float** img2, int width, int height, float start, float stop);

/*------------------------------------------------*/
/* PROGRAMME PRINCIPAL   -------------------------*/                     
/*------------------------------------------------*/
int main(int argc,char **argv)
 {
  int length, width;
  float theta;
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

  theta = CalcAngle(imgM1, imgM2, width, length, deg(10), 0.00001);

  IFFTDD(imgR1, imgI1, length, width);
  IFFTDD(imgR2, imgI2, length, width);
 
  Rotate(&imgR2, width, length, 0, 0, theta);

  SaveImagePgm(NAME_IMG_OUT, imgR2, length, width);

  FreeImg(imgR1, imgI1, imgM1);
  FreeImg(imgR2, imgI2, imgM2);

  //Commande systeme: VISU
  strcpy(BufSystVisuImg,NAME_VISUALISER);
  strcat(BufSystVisuImg,NAME_IMG_OUT);
  strcat(BufSystVisuImg,".pgm&");
  printf(" %s",BufSystVisuImg);
  system(BufSystVisuImg);


  //==End=========================================================

  //retour sans probleme
  printf("\n Question 4 terminée avec succès! ... \n\n");
  return 0; 	 
}

float CalcAngle(float** img1, float** img2, int width, int height, float start, float stop)
{
  float eval1, eval2;
  float** rot = fmatrix_allocate_2d(height, width);
  float step = start / 2;
  do {
    CopyImg(img2, rot, width, height);
    Rotate(&rot, width, height, 0, 0, start + step);
    eval1 = EvalDiff(img1, rot, width, height);
    CopyImg(img2, rot, width, height);
    Rotate(&rot, width, height, 0, 0, start - step);
    eval2 = EvalDiff(img1, rot, width, height);
    if (eval1 < eval2) start += step;
    else start -= step;
  } while ((step /= 2) > stop);
  free_fmatrix_2d(rot);
  return start;
}
