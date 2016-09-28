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

  //===============================
  //Question 1
  //===============================

  float Dirac[20];
  float Echelon[20];
  float Signal[20];

  memset(Dirac,   0, 20 * sizeof(float));
  for (i = 1; i < 20; i++)
    Echelon[i] = 1;

  Dirac[2] = 1;
  Echelon[0] = 0;
  Echelon[1] = 0;
  
  filter3(Dirac, Signal, 20);

  printf("\nReponse-Dirac:\n");

  for (i = 2; i < 10; i++)
    printf("\t[ %f ]\n", Signal[i]);
  printf("\n");

  filter3(Echelon, Signal, 20);

  printf("\nReponse-Echelon:\n");

  for (i = 2; i < 10; i++)
    printf("\t[ %f ]\n", Signal[i]);
  printf("\n");
  
 
  //==End=========================================================

  //retour sans probleme
  printf("\n C'est fini ... \n\n");
  return 0; 	 
}


