/*------------------------------------------------------*/
/* Prog    : Tp7_IFT3205_2.c                            */
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

#include "FonctionDemo7.h"

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

  //==============================
  //Question 2
  //------------
  //==============================
  float*  SignIn=LoadSignalDat("Signal1",&length);
  float*  SignOut=fmatrix_allocate_1d(length);

  //SaveSignalDat("Autocor_Signal1",SignOut,10);
 
  //Visu
  strcpy(BufSystVisuSig,NAME_VISUALISER);
  strcat(BufSystVisuSig,"Signal1.dat&");
  //strcat(BufSystVisuSig,"Autocor_Signal1&");
  printf(" %s",BufSystVisuSig);
  system(BufSystVisuSig);

 
  //==End=========================================================
 
  //retour sans probleme
  printf("\n C'est fini ... \n\n");
  return 0; 	 
}


