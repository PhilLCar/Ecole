//------------------------------------------------------
// Prog    : Tp5_IFT3205_2-1                          
// Auteur  : Philippe Caron                                          
// Date    : 30/03/2016                   
// version : N/A                                  
// langage : C                                          
// labo    : DIRO                                       
//------------------------------------------------------


//------------------------------------------------
// FICHIERS INCLUS -------------------------------
//------------------------------------------------
#include "FonctionDemo5.h"
#include "noise.c"

//------------------------------------------------
// DEFINITIONS -----------------------------------
//------------------------------------------------
#define NAME_VISUALISER "display "
#define NAME_IMG_IN     "lena512"
#define NAME_IMG_OUT    "Tp5_IFT3205_2-1"

//------------------------------------------------
// PROTOTYPE DE FONCTIONS  -----------------------
//------------------------------------------------

//------------------------------------------------
// CONSTANTES ------------------------------------
//------------------------------------------------
#define SIGMA_NOISE  30 

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
int main(int argc,char** argv)
{ 
  int length,width;
  char BufSystVisuImg[NBCHAR];

  //>Lecture Image 
  float **Img = LoadImagePgm(NAME_IMG_IN, &length, &width);
 
  //>Allocation memory
  float **ImgDegraded = fmatrix_allocate_2d(length, width); 

  //>Degradation
  CopyImg(Img, ImgDegraded, width, length);
  
  add_gaussian_noise(ImgDegraded,length,width,SIGMA_NOISE*SIGMA_NOISE);
 
  printf("\n\n  Info Noise");
  printf("\n  ------------");
  printf("\n    > MSE = [%.2f]\n", computeMMSE(ImgDegraded,Img,length)); 
  
  //---------------------------------------------
  // SAUVEGARDE 
  // -------------------
  // L'image dégradée             > ImgDegraded
  // Le resultat du debruitage    > ImgFiltered
  //----------------------------------------------
  SaveImagePgm(NAME_IMG_OUT,ImgDegraded,length,width);  

  //>Visu Img
  strcpy(BufSystVisuImg,NAME_VISUALISER);
  strcat(BufSystVisuImg,NAME_IMG_IN);
  strcat(BufSystVisuImg,".pgm&");
  printf("\n > %s",BufSystVisuImg);
  system(BufSystVisuImg);
  
  //Visu ImgFiltered
  strcpy(BufSystVisuImg,NAME_VISUALISER);
  strcat(BufSystVisuImg,NAME_IMG_OUT);
  strcat(BufSystVisuImg,".pgm&");
  printf("\n > %s",BufSystVisuImg);
  system(BufSystVisuImg);

    
  //--------------- End -------------------------------------     
  //----------------------------------------------------------

  //Liberation memoire pour les matrices
  if (Img)          free_fmatrix_2d(Img);
  if (ImgDegraded)  free_fmatrix_2d(ImgDegraded);
  
  //Return
  printf("\n C'est fini... \n");; 
  return 0;
}
