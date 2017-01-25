/*------------------------------------------------------*/
/* Prog    : Tp6_IFT3205_3-3.c                          */
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
#include <time.h>

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
  //Question 3.3
  //===============================
  float *Sign1  = LoadSignalDat("moteur2", &length);
  float *mean   = fmatrix_allocate_1d(block_size);

  /*
   * Le document dit "Hamming" mais le barème "Hann", par précaution
   * les deux fonctions seront calculées
   */
  Hamming(Sign1, mean, length, block_size);
  SaveSignalDat("FFT_Moteur2_PermMoyHamming", mean, block_size);
  Hann(Sign1, mean, length, block_size);
  SaveSignalDat("FFT_Moteur2_PermMoyHann", mean, block_size);

  // Freeimg
  free(Sign1);
  free(mean);
  
  //Visu
  strcpy(BufSystVisuSig, NAME_VISUALISER);
  strcat(BufSystVisuSig, "FFT_Moteur2_PermMoyHamming.dat&");
  printf(" %s", BufSystVisuSig);
  system(BufSystVisuSig);

  // Sleep 100ms pour empêcher le script de collisioner
  struct timespec ts;
  ts.tv_sec = 0;
  ts.tv_nsec = 100000000;
  nanosleep(&ts, NULL);
    
  strcpy(BufSystVisuSig, NAME_VISUALISER);
  strcat(BufSystVisuSig, "FFT_Moteur2_PermMoyHann.dat&");
  printf(" %s", BufSystVisuSig);
  system(BufSystVisuSig);

  //==End=========================================================
  //==============================================================

  //retour sans probleme
  printf("\n C'est fini ... \n\n");
  return 0; 	 
}


