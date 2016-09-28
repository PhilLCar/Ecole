/*------------------------------------------------------*/
/* Prog    : Tp6_IFT3205_3-2.c                          */
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
  int block_size = 512;
  int length;
  char BufSystVisuSig[100];

  //===============================
  //Question 3.2
  //===============================
  float *Sign1  = LoadSignalDat("moteur2", &length);
  float *mean   = fmatrix_allocate_1d(block_size);
  
  meanFFT(Sign1, mean, length, block_size);
  SaveSignalDat("FFT_Moteur2_PermMoy", mean, block_size);

  // Freeimg
  free(Sign1);
  free(mean);
  
  //Visu
  strcpy(BufSystVisuSig, NAME_VISUALISER);
  strcat(BufSystVisuSig, "FFT_Moteur2_PermMoy.dat&");
  printf(" %s\n", BufSystVisuSig);
  system(BufSystVisuSig);

  //==End=========================================================
  //==============================================================

  //retour sans probleme
  printf("\n C'est fini ... \n\n");
  return 0; 	 
}


