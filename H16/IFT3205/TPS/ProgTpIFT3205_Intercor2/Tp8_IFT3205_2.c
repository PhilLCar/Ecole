/*------------------------------------------------------*/
/* Prog    : Tp8_IFT3205.c                              */
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

#include "FonctionDemo8.h"

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
  int i,j;
  int length;
  char BufSystVisuSig[100];

  //===============================
  //Question 2.1
  //===============================
  
      float*  Sign0=LoadSignalDat("SinusRecu0",&length);
      float*  Sign1=LoadSignalDat("SinusRecu1",&length);
      float*  Sign2=LoadSignalDat("SinusEmis1",&length);
   
  //Visu
  strcpy(BufSystVisuSig,NAME_VISUALISER);
  strcat(BufSystVisuSig,"SinusRecu0.dat&");
  printf(" %s",BufSystVisuSig);
  system(BufSystVisuSig);
  sleep(0.05);

  strcpy(BufSystVisuSig,NAME_VISUALISER);
  strcat(BufSystVisuSig,"SinusRecu1.dat&");
  printf(" %s",BufSystVisuSig);
  system(BufSystVisuSig);
  sleep(0.05);

  strcpy(BufSystVisuSig,NAME_VISUALISER);
  strcat(BufSystVisuSig,"SinusEmis1.dat&");
  printf(" %s",BufSystVisuSig);
  system(BufSystVisuSig); 
  sleep(0.05);
   

  
  //==End=========================================================

 
  //retour sans probleme
  printf("\n C'est fini ... \n\n");
  return 0; 	 
}


