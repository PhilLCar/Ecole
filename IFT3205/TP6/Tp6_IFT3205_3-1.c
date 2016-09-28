/*------------------------------------------------------*/
/* Prog    : Tp6_IFT3205_3-1.c                          */
/* Auteur  : Philippe Caron                             */
/* Date    : 08/04/2010                                 */
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

#include "FonctionDemo6.h"
#include "sound.c"

/*------------------------------------------------*/
/* DEFINITIONS -----------------------------------*/   
/*------------------------------------------------*/
#define NAME_VISUALISER "./ViewSig.sh "

/*------------------------------------------------*/
/* PROTOTYPE DE FONCTIONS  -----------------------*/   
/*------------------------------------------------*/

/*------------------------------------------------*/
/* PROGRAMME PRINCIPAL   -------------------------*/                     
/*------------------------------------------------*/
int main(int argc,char **argv)
{
  int length;
  char BufSystVisuSig[100];

  //===============================
  //Question 3.1
  //===============================
  float *Sign1  = LoadSignalDat("moteur2", &length);
  float *Sign1I = fmatrix_allocate_1d(length);
  float *Sign1M = fmatrix_allocate_1d(length); 
  FFT1D(Sign1, Sign1I, length);
  ModVct(Sign1M, Sign1, Sign1I, length);
  CenterVct(Sign1M, length);
  SaveSignalDat("FFT_Moteur2", Sign1M, length);

  // Free signals
  free(Sign1);
  free(Sign1I);
  free(Sign1M);
   
  //Visu
  strcpy(BufSystVisuSig, NAME_VISUALISER);
  strcat(BufSystVisuSig, "FFT_Moteur2.dat&");
  printf(" %s\n", BufSystVisuSig);
  system(BufSystVisuSig);

  //==End=========================================================
  //==============================================================

  //retour sans probleme
  printf("\n C'est fini ... \n\n");
  return 0; 	 
}


