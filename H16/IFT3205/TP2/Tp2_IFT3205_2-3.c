/*------------------------------------------------------*/
/* Prog    : Tp2_IFT3205-2-3.c                          */
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
#define NAME_IMG_OUT1   "image-TpIFT3205-2-3a"
#define NAME_IMG_OUT2   "image-TpIFT3205-2-3b"

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

  // Trouve un angle pour une certaine valeur d'incertitude
  // en convergeant sur la valeur par bon équivalents à la
  // moitiée de la distance, jusqu'à ce que la distance soit
  // inférieure à cette valeur

  // Incertitude sur l'angle
  float E = 0.00000005;

  // Fonction de calcul, retourne les résultats intermédiares
  // sous la forme [ANGLE::BON] en radians
  theta = CalcAngle(imgM1, imgM2, width, length, deg(10), E);

  printf("\nAngle >>> [%f±%f]\n", theta, E);
 
  Rotate(&imgM2, width, length, 0, 0, theta);

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
  printf("\n Question 3 terminée avec succès! ... \n\n");
  return 0; 	 
}

float CalcAngle(float** img1, float** img2, int width, int height, float start, float stop)
{
  float eval1, eval2, eval = 0;
  float** rot = fmatrix_allocate_2d(height, width);
  float step = start / 2;
  do {
    printf("[%f::%d]\n", start, (int)eval);
    CopyImg(img2, rot, width, height);
    Rotate(&rot, width, height, 0, 0, start + step);
    eval1 = EvalDiff(img1, rot, width, height);
    CopyImg(img2, rot, width, height);
    Rotate(&rot, width, height, 0, 0, start - step);
    eval2 = EvalDiff(img1, rot, width, height);
    if (eval1 < eval2) {
      start += step;
      eval = eval1;
    } else {
      start -= step;
      eval = eval2;
    }
  } while ((step /= 2) > stop);
  free_fmatrix_2d(rot);
  return start;
}