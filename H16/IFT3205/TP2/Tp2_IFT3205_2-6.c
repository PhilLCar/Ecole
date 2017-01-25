/*------------------------------------------------------*/
/* Prog    : Tp2_IFT3205-2-6.c                          */
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
#define NAME_IMG_OUT    "image-TpIFT3205-2-6"

/*------------------------------------------------*/
/* PROTOTYPE DE FONCTIONS  -----------------------*/   
/*------------------------------------------------*/
float CalcAngle(float**, float**, int, int, float, float);
float divide(float, float);
void CalcVector(float**, int, int, float[2]);
void CalcVector2(float**, int, int, int[2]);

/*------------------------------------------------*/
/* PROGRAMME PRINCIPAL   -------------------------*/                     
/*------------------------------------------------*/
int main(int argc,char **argv)
{
  int length, width, i, j;
  int rwidth, rheight, protection = 5;
  float theta;
  float vec[2];
  int vec2[2];
  char BufSystVisuImg[100];
  float** imgM1, ** imgR1, ** imgI1;
  float** imgM2, ** imgR2, ** imgI2;
  float** imgR, ** imgI, ** combi;

  LoadImg(NAME_IMG_IN1, &imgR1, &imgI1, &imgM1, &length, &width);
  LoadImg(NAME_IMG_IN2, &imgR2, &imgI2, &imgM2, &length, &width);

  imgR = fmatrix_allocate_2d(length, width);
  imgI = fmatrix_allocate_2d(length, width);
 
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

  theta = CalcAngle(imgM1, imgM2, width, length, deg(10), 0.0000001);

  IFFTDD(imgR1, imgI1, length, width);
  IFFTDD(imgR2, imgI2, length, width);
 
  Rotate(&imgR2, width, length, 0, 0, theta);

  InterCorrelate(imgR2, imgR1, imgR, imgI, width, length);

  SquareMod(imgR1, imgI1, imgM1, width, length);

  PerformOp(imgR, imgM1, imgR, width, length, divide);
  PerformOp(imgI, imgM1, imgI, width, length, divide);

  IFFTDD(imgR, imgI, length, width);

  CalcVector(imgR, width, length, vec);

  // INIT
  rwidth  = 2 * width  + 2 * protection - ceil(vec[0]);
  rheight = length + 2 * protection + ceil(vec[1]);
  combi = fmatrix_allocate_2d(rheight, rwidth);
  for (i = 0; i < rwidth; i++)
    for (j = 0; j < rheight; j++)
      combi[j][i] = 0;
  
  printf("Vecteurs : (%f, %f)\n", vec[0], vec[1]);

  PasteImg(combi, imgR2, width, length, protection + width - vec[0], protection - (1 - (ceil(vec[1]) - vec[1])));
  PasteImg(combi, imgR1, width, length, protection, protection + ceil(vec[1]));

  SaveImagePgm(NAME_IMG_OUT, combi, rheight, rwidth);
  //SaveImagePgm(NAME_IMG_OUT, imgR1, length, width);

  FreeImg(imgR1, imgI1, imgM1);
  FreeImg(imgR2, imgI2, imgM2);
  free_fmatrix_2d(imgR);
  free_fmatrix_2d(imgI);
  free_fmatrix_2d(combi);

  //Commande systeme: VISU
  strcpy(BufSystVisuImg,NAME_VISUALISER);
  strcat(BufSystVisuImg,NAME_IMG_OUT);
  strcat(BufSystVisuImg,".pgm&");
  printf(" %s",BufSystVisuImg);
  system(BufSystVisuImg);


  //==End=========================================================

  //retour sans probleme
  printf("\n Question 6 terminée avec succès! ... \n\n");
  return 0; 	 
}

float CalcAngle(float** img1, float** img2, int width, int height, float start, float stop)
{
  float eval1, eval2;
  float** rot = fmatrix_allocate_2d(height, width);
  float step = start / 2;
  fprintf(stderr, "Calcul de l'angle en cours .");
  do {
    fprintf(stderr, " .");
    CopyImg(img2, rot, width, height);
    Rotate(&rot, width, height, 0, 0, start + step);
    eval1 = EvalDiff(img1, rot, width, height);
    CopyImg(img2, rot, width, height);
    Rotate(&rot, width, height, 0, 0, start - step);
    eval2 = EvalDiff(img1, rot, width, height);
    if (eval1 < eval2) start += step;
    else start -= step;
  } while ((step /= 2) > stop);
  printf("\n");
  free_fmatrix_2d(rot);
  return start;
}

void CalcVector(float** img, int width, int height, float vector[2])
{
  int i, j;
  float total, moy, max = 0;
  float x, y;

  for (i = 0; i < width; i++)
    for (j = 0; j < height; j++) {
      if (img[j][i] > max) {
	max = img[j][i];
        x = i;
        y = j;
      }
    }
  
  if (x > 0 && x < width - 1 && y > 0 && y < height - 1) {
    i = x;
    j = y;
  
    total =
      img[j - 1][i - 1] + img[j - 1][i] + img[j - 1][i + 1] +
      img[j][i - 1]     +     img[j][i] +     img[j][i + 1] +
      img[j + 1][i - 1] + img[j + 1][i] + img[j + 1][i + 1];
    
    x =
      (i - 1) * (img[j - 1][i - 1] + img[j][i - 1] + img[j + 1][i - 1]) +
      i * (img[j - 1][i]     + img[j][i]     + img[j + 1][i])     +
      (i + 1) * (img[j - 1][i + 1] + img[j][i + 1] + img[j + 1][i + 1]);
    x /= total;
    
    y =
      (j - 1) * (img[j - 1][i - 1] + img[j - 1][i] + img[j - 1][i + 1]) +
      j       * (img[j][i - 1]     +     img[j][i] +     img[j][i + 1]) +
      (j + 1) * (img[j + 1][i - 1] + img[j + 1][i] + img[j + 1][i + 1]);
    y /= total;
    }/**/
  vector[0] = x;
  vector[1] = y;
}

void CalcVector2(float** img, int width, int height, int vector[2])
{
  int i, j;
  float current, max = 0;
  
  /// SIMPLE CALC
  for (i = 0; i < width; i++)
    for (j = 0; j < height; j++) {
      if (img[j][i] > max) {
	max = img[j][i];
	vector[0] = i;
	vector[1] = j;
      }
    }
}

float divide(float a, float b)
{
  return a / b;
}
