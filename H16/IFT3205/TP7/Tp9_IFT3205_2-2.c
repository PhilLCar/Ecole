/*------------------------------------------------------*/
/* Prog    : Tp9_IFT3205.c                              */
/* Auteur  :                                            */
/* Date    : --/--/2010                                 */
/* version :                                            */ 
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

#include "FonctionDemo9.h"
#include "sound.c"

/*------------------------------------------------*/
/* DEFINITIONS -----------------------------------*/   
/*------------------------------------------------*/
#define NAME_VISUALISER_IMG "./display "
#define NAME_VISUALISER     "./ViewSig.sh "

/*------------------------------------------------*/
/* PROTOTYPE DE FONCTIONS  -----------------------*/   
/*------------------------------------------------*/

/*------------------------------------------------*/
/* PROGRAMME PRINCIPAL   -------------------------*/                     
/*------------------------------------------------*/
int main(int argc,char **argv)
{
  int i,j;
  char BufSystVisuSig[100];
  int length;
  float Tech;
  float freq = 500.0;

  float *Sign = LoadSignalDat2("SoundFile", &length, &Tech);
  
  float theta = 2 * PI / (Tech * freq);

  for (i = 0; i < length; i++) {
    Sign[i] += cos(theta * i);
  }

  SaveSignalDat2("SoundFileDeg", Sign, length, Tech);
  
  //Visu Ecran
  strcpy(BufSystVisuSig,NAME_VISUALISER);
  strcat(BufSystVisuSig,"SoundFileDeg.dat&");
  printf(" %s",BufSystVisuSig);
  system(BufSystVisuSig);

  printf("\n C'est fini ... \n\n");
  return 0; 	 
}

