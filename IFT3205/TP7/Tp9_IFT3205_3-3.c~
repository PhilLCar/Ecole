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
  float Tech = 1;

  //===============================
  //Question 1
  //===============================

  float*  Signal = LoadSignalDat2("SoundFile", &length, &Tech);
  float*  SCanal = fmatrix_allocate_1d(length);

  printf("\nlength %d\n", length);
  
  filter3(Signal, SCanal, length);

  printf("\nRSB=[%0.2fdB]\n", RSB(Signal, SCanal, length));

  //Sauvegarde en fichier .dat
  SaveSignalDat2("SoundFileInCanal", SCanal, length, Tech);

  //Visu Ecran
  strcpy(BufSystVisuSig,NAME_VISUALISER);
  strcat(BufSystVisuSig,"SoundFileInCanal.dat&");
  printf(" %s",BufSystVisuSig);
  system(BufSystVisuSig);

  free(SCanal);
  free(Signal);


  //---------------------------------------------
  //Sauvegarde de SignZ (30000 echantillons 
  //aléatoire entre [0::200]) dans un 
  //fichier .wav avec une periode 
  //d'echantillonnage de 10000: Nb echant/secondes
  //(pour 3 secondes d'écoute)
  //----------------------------------------------
  //----------------------------------------------
  if (0)
    {
      length=30000;
      float*  SignZ=fmatrix_allocate_1d(length);
      for(i=0;i<length;i++) SignZ[i]=(int)(((float)rand()/RAND_MAX)*200.0);
      SaveSignalDatWav("SignalZ",SignZ,length,10000);
      free(SignZ);
    }
     

  //Rappel -Pour Lecture/Sauvegarde/Visu [Fichier Son]
  //=================================================
  //float*  SignIn=LoadSignalDat("NameSignalIn",&length);
  //SaveSignalDat("NameSignalOut",SignIn,length); 
  //strcpy(BufSystVisuSig,NAME_VISUALISER);
  //strcat(BufSystVisuSig,"NameSignalOut.dat&");
  //printf(" %s",BufSystVisuSig);
  //system(BufSystVisuSig);
  
 
  //==End=========================================================

  //retour sans probleme
  printf("\n C'est fini ... \n\n");
  return 0; 	 
}


