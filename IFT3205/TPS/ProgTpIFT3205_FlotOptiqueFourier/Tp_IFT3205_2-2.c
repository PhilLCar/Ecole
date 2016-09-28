/*------------------------------------------------------*/
/* Prog    :                                            */
/* Auteur  :                                            */
/* Date    :                                            */
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

#include "FonctionDemo2.h"

/*------------------------------------------------*/
/* DEFINITIONS -----------------------------------*/   
/*------------------------------------------------*/
#define NAME_VISUALISER "display "
#define NAME_IMG_OUT "OpticalFlow"

/*------------------------------------------------*/
/* PROTOTYPE DE FONCTIONS  -----------------------*/   
/*------------------------------------------------*/




/*------------------------------------------------*/
/* PROGRAMME PRINCIPAL   -------------------------*/                     
/*------------------------------------------------*/
int main(int argc,char **argv)
 {
  int length,width;
  char BufSystVisuImg[100];
 
  //>Constants
  const int SizeWind=32;

  //>Read_Arguments
  if(argc<3) 
   { printf("\n\t Usage: %s image1 image2",argv[0]); 
     printf("\n\t Exemple: %s rubikseq0 rubikseq1",argv[0]);
     printf("\n\t Exemple: %s salesman0 salesman1",argv[0]);
     printf("\n\t Exemple: %s carfront0 carfront1",argv[0]);
     printf("\n\n\n");
     return 0; } 

  //>Read_Image 
  float** Img1=LoadImagePgm(argv[1],&length,&width);
  float** Img2=LoadImagePgm(argv[2],&length,&width);

  //>Memory_Allocation
  float** ImgOptFlot=fmatrix_allocate_2d(length,width);
  float** OptFl_Vx=fmatrix_allocate_2d(length,width);
  float** OptFl_Vy=fmatrix_allocate_2d(length,width); 
  float** SquWin1=fmatrix_allocate_2d(SizeWind,SizeWind);
  float** SquWin2=fmatrix_allocate_2d(SizeWind,SizeWind);

  //>Optical_Flow_Estimation[Img1,Img2]
  

  // A vouss de jouer .......


  ConvertVelocityFieldInArrowField(ImgOptFlot,OptFl_Vx,OptFl_Vy,length,width,7);
  SaveImagePpm(NAME_IMG_OUT,Img2,ImgOptFlot,length,width);

  //>Command_System:VISU
  strcpy(BufSystVisuImg,NAME_VISUALISER);
  strcat(BufSystVisuImg,NAME_IMG_OUT);
  strcat(BufSystVisuImg,".ppm&");
  printf("\n %s",BufSystVisuImg);
  system(BufSystVisuImg);

  //>Free_Memory
  if (Img1)        free_fmatrix_2d(Img1);
  if (Img2)        free_fmatrix_2d(Img2);
  if (ImgOptFlot)  free_fmatrix_2d(ImgOptFlot); 
  if (OptFl_Vx)    free_fmatrix_2d(OptFl_Vx);
  if (OptFl_Vy)    free_fmatrix_2d(OptFl_Vy);
  if (SquWin1)     free_fmatrix_2d(SquWin1);
  if (SquWin2)     free_fmatrix_2d(SquWin2); 

  //>Return_Without_Problem
  printf("\n C'est fini ... \n\n");
  return 0; 	 
}


