/*------------------------------------------------------*/
/* Prog    : Tp6_IFT3205_2-2.c                          */
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
  int index = 0, f0, fault;
  float Tech, max = 0;
  int length;
  char BufSystVisuSig[100];

  //===============================
  //Question 2.2
  //===============================
  float *Sign1  = LoadSignalDat2("moteur1", &length, &Tech);
  float *Sign1I = fmatrix_allocate_1d(length);
  float *Sign1M = fmatrix_allocate_1d(length); 
  FFT1D(Sign1, Sign1I, length);
  ModVct(Sign1M, Sign1, Sign1I, length);
  CenterVct(Sign1M, length);
  SaveSignalDat("FFT_Moteur1", Sign1M, length);

  max = findmax(Sign1M, length, &index);
  f0 = length / 2 - index;

  printf("\n Maximum coefficient of %f at position %d (%d to zero)\n", max, index, f0);
  printf(" Estimated RPM of motor [4-Stroke 8c] is %d\n\n", estimateRPM(f0 / (float)length, Tech, 4));
  
  max = findmaxnonharmonic(Sign1M, length / 2, f0, &fault, 2);

  printf("\n Fault candidate with value %f at position: %d (%d to zero)\n",
	 max, fault, length / 2 - fault);
  printf(" Fault frequency ratio is: %f\n\n", (float)(length / 2 - fault) / (float)f0);

  // Free signals
  free(Sign1);
  free(Sign1I);
  free(Sign1M);
   
  //Visu
  strcpy(BufSystVisuSig, NAME_VISUALISER);
  strcat(BufSystVisuSig, "FFT_Moteur1.dat&");
  printf(" %s", BufSystVisuSig);
  system(BufSystVisuSig);

  //==End=========================================================
  //==============================================================

  //retour sans probleme
  printf("\n C'est fini ... \n\n");
  return 0; 	 
}


