//------------------------------------------------------
// Prog    : Tp5_IFT3205_2-3                          
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
#define NAME_IMG_OUT    "image-TpIFT3205-2-2"
#define NAME_IMG_DEG    "lena512_Degraded"

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
  int i, T;
  float mse, min;
  
  int length,width;
  char BufSystVisuImg[NBCHAR];

  //>Lecture Image 
  float **Img = LoadImagePgm(NAME_IMG_IN, &length, &width);
 
  //>Allocation memory
  float **ImgDegraded = fmatrix_allocate_2d(length, width);
  float **ImgDenoised = fmatrix_allocate_2d(length, width);  

  //>Degradation
  CopyImg(Img, ImgDegraded, width, length);
  
  add_gaussian_noise(ImgDegraded,length,width,SIGMA_NOISE*SIGMA_NOISE);
 
  printf("\n\n  Info Noise");
  printf("\n  ------------");
  printf("\n    > MSE = [%.2f]\n", min = computeMMSE(ImgDegraded,Img,length)); 
 

  //=========================================================
  //== PROG =================================================
  //=========================================================

  for (i = 0; i < 255; i++) {
    if (!(i % 4)) fprintf(stderr, ".");
    
    CopyImg(ImgDegraded, ImgDenoised, width, length);

    RemoveNoise2(ImgDenoised, width, length, i);

    mse = computeMMSE(ImgDenoised, Img, length);
    if (mse < min) {
      min = mse;
      T = i;
    }
  }
  
  CopyImg(ImgDegraded, ImgDenoised, width, length);

  RemoveNoise2(ImgDenoised, width, length, T);

  printf("\n\n  Info Denoise");
  printf("\n  ------------");
  printf("\n    > T   =  %d", T); 
  printf("\n    > MSE = [%.2f]", min); 
  
  //---------------------------------------------
  // SAUVEGARDE 
  // -------------------
  // L'image dégradée             > ImgDegraded
  // Le resultat du debruitage    > ImgFiltered
  //----------------------------------------------
  SaveImagePgm(NAME_IMG_OUT,ImgDenoised,length,width);
  
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
  if (ImgDenoised)  free_fmatrix_2d(ImgDenoised);
  
  //Return
  printf("\n C'est fini... \n");; 
  return 0;
}
