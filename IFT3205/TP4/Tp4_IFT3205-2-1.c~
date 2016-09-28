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

float** mat;
float** mat_img;
float** mat_rest;
float** mat_rest_prec;
float** mat_rest_best;
float** mat_psf;

float** mat_tmp0;
float** mat_tmp1;
float** mat_tmp2;
float** mat_tmp3;
float** mat_tmp4;
float** mat_tmp5;
float** mat_tmp6;
float** mat_tmp7;
float** mat_tmp8;
float** mat_tmp9;
float** mat_tmp10;
float** mat_tmp11;
float** mat_tmp12;

//>Taille Image
int length=256;
int width=256;
int size_image=256;

//>Parametre de degradation
int size=9;
float variance_noise=0.5;
int psf=1;

//>Nb d'Iterations
int nbiter=20;

//>ImprovmentSNR
float isnr;

//------------------------------------------------
//------------------------------------------------
// FONCTIONS  ------------------------------------   
//------------------------------------------------
//------------------------------------------------


//---------------------------------------------------------
//---------------------------------------------------------
// PROGRAMME PRINCIPAL   ----------------------------------                     
//---------------------------------------------------------
//---------------------------------------------------------
int main(int argc,char **argv)
 {
  int i,j,k;
  int lgth,wdth;
  char BufSystVisuImg[100];

  //Allocation memoire matrice
  mat=fmatrix_allocate_2d(length,width);
  mat_rest=fmatrix_allocate_2d(length,width);
  mat_rest_prec=fmatrix_allocate_2d(length,width); 
  mat_rest_best=fmatrix_allocate_2d(length,width); 
  mat_psf=fmatrix_allocate_2d(length,width);

  mat_tmp0=fmatrix_allocate_2d(length,width);
  mat_tmp1=fmatrix_allocate_2d(length,width);
  mat_tmp2=fmatrix_allocate_2d(length,width);
  mat_tmp3=fmatrix_allocate_2d(length,width);
  mat_tmp4=fmatrix_allocate_2d(length,width);
  mat_tmp5=fmatrix_allocate_2d(length,width);
  mat_tmp6=fmatrix_allocate_2d(length,width);
  mat_tmp7=fmatrix_allocate_2d(length,width);
  mat_tmp8=fmatrix_allocate_2d(length,width);
  mat_tmp9=fmatrix_allocate_2d(length,width);
  mat_tmp10=fmatrix_allocate_2d(length,width);
  mat_tmp11=fmatrix_allocate_2d(length,width);
  mat_tmp12=fmatrix_allocate_2d(length,width);

 
  //=========================================================
  //== PROG =================================================
  //=========================================================

  //>Lecture Image
  mat_img=LoadImagePgm(NAME_IMG_IN,&lgth,&wdth);

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
  degradation(mat_img,mat,length,width,psf,size,variance_noise);



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
  SaveImagePgm(NAME_IMG_OUT1,mat_rest,length,width);
  SaveImagePgm(NAME_IMG_OUT2,mat,length,width);
  
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
  strcpy(BufSystVisuImg,NAME_VISUALISER);
  strcat(BufSystVisuImg,NAME_IMG_OUT1);
  strcat(BufSystVisuImg,".pgm&");
  printf("\n > %s",BufSystVisuImg);
  system(BufSystVisuImg);
  
  
  //Liberation memoire pour les matrices
  if (mat)            free_fmatrix_2d(mat); 
  if (mat_img)        free_fmatrix_2d(mat_img); 
  if (mat_rest)       free_fmatrix_2d(mat_rest);
  if (mat_rest_prec)  free_fmatrix_2d(mat_rest_prec);
  if (mat_rest_best)  free_fmatrix_2d(mat_rest_best);
  if (mat_psf)        free_fmatrix_2d(mat_psf);
  if (mat_tmp0)  free_fmatrix_2d(mat_tmp0);
  if (mat_tmp1)  free_fmatrix_2d(mat_tmp1); 
  if (mat_tmp2)  free_fmatrix_2d(mat_tmp2); 
  if (mat_tmp3)  free_fmatrix_2d(mat_tmp3); 
  if (mat_tmp4)  free_fmatrix_2d(mat_tmp4); 
  if (mat_tmp5)  free_fmatrix_2d(mat_tmp5); 
  if (mat_tmp6)  free_fmatrix_2d(mat_tmp6); 
  if (mat_tmp7)  free_fmatrix_2d(mat_tmp7); 
  if (mat_tmp8)  free_fmatrix_2d(mat_tmp8);
  if (mat_tmp9)  free_fmatrix_2d(mat_tmp9); 
  if (mat_tmp10) free_fmatrix_2d(mat_tmp10);
  if (mat_tmp11) free_fmatrix_2d(mat_tmp11);
  if (mat_tmp12) free_fmatrix_2d(mat_tmp12);

  //retour sans probleme 
  printf("\n C'est fini ... \n\n");
  return 0; 	 
}


