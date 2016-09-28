//------------------------------------------------------
// Prog    : Tp4_IFT3205                          
// Auteur  :                                            
// Date    :                                  
// version :                                             
// langage : C                                          
// labo    : DIRO                                       
//------------------------------------------------------

//------------------------------------------------
// FICHIERS INCLUS -------------------------------
//------------------------------------------------
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "FonctionDemo4.h"
#include "Utils.c"

//------------------------------------------------
// DEFINITIONS -----------------------------------   
//------------------------------------------------
#define NAME_VISUALISER "display "
#define NAME_IMG_IN  "cameraman"
#define NAME_IMG_OUT1 "cameraman_restored"
#define NAME_IMG_OUT2 "cameraman_degraded"

//------------------------------------------------
// PROTOTYPE DE FONCTIONS  -----------------------   
//------------------------------------------------

//------------------------------------------------
// VARIABLES GLOBALES  ---------------------------   
//------------------------------------------------

//>Taille Image
int size_image=256;

//>Parametre de degradation
int size=9;
float variance_noise=0.5;
int psf=1;

//>Nb d'Iterations
int nbiter=20;

//>ImprovmentSNR
float isnr;

//---------------------------------------------------------
//---------------------------------------------------------
// PROGRAMME PRINCIPAL   ----------------------------------                     
//---------------------------------------------------------
//---------------------------------------------------------
int main(int argc,char **argv)
 {
  int i,j,k;
  int width, length;
  char BufSystVisuImg[100];
  float **img1, **img2;
  
  //=========================================================
  //== PROG =================================================
  //=========================================================

  //>Lecture Image
  img1 = LoadImagePgm(NAME_IMG_IN, &length, &width);
  img2 = fmatrix_allocate_2d(length, width);

  //--------------------------------------------------------
  //>Degradation
  //--------------------------------------------------------
  // Cette fonction ajoute un flou créé par une psf uniforme 
  // (fonction porte) de taille sizexsize puis ajoute sur
  // cette image rendue floue, un bruit Gaussien de variance
  // variance_noise
  //
  // Entrée : mat_img :: image non dégradée
  // Sortie : mat     :: image dégradée
  //--------------------------------------------------------
  degradation(img1, img2, length, width, psf, size, variance_noise);
  


  //============
  //WIENER
  //============
  // .....
  

  //---------------------------------------------
  // SAUVEGARDE et VISU
  // -------------------
  // Le resultat de la restoration > mat_rest
  // L'image dégradée              > mat
  // L'image non dégradée          > mat_img
  //----------------------------------------------
  //SaveImagePgm(NAME_IMG_OUT1,mat_rest,length,width);
  SaveImagePgm(NAME_IMG_OUT2, img2, length, width);
  
  strcpy(BufSystVisuImg,NAME_VISUALISER);
  strcat(BufSystVisuImg,NAME_IMG_OUT2);
  strcat(BufSystVisuImg,".pgm&");
  printf("\n > %s",BufSystVisuImg);
  system(BufSystVisuImg);
  strcpy(BufSystVisuImg,NAME_VISUALISER);
  strcat(BufSystVisuImg,NAME_IMG_IN);
  strcat(BufSystVisuImg,".pgm&");
  printf("\n > %s",BufSystVisuImg);
  system(BufSystVisuImg);
  /*strcpy(BufSystVisuImg,NAME_VISUALISER);
  strcat(BufSystVisuImg,NAME_IMG_OUT1);
  strcat(BufSystVisuImg,".pgm&");
  printf("\n > %s",BufSystVisuImg);
  system(BufSystVisuImg);*/
  
  
  //Liberation memoire pour les matrices
  FreeImg(img1, img2);

  //retour sans probleme 
  printf("\n C'est fini ... \n\n");
  return 0; 	 
}


