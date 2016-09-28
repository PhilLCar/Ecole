/*---------------------------------------------------*/
/* module  : FonctionDemo6.h                         */
/* auteur  : Max Mignotte                            */
/* revision:                                         */
/* date    : --/--/2010                              */   
/* langage : C                                       */
/* labo    : DIRO                                    */
/*---------------------------------------------------*/

#ifndef FONCTIONDEMO_H
#define FONCTIONDEMO_H

/*------------------------------------------------*/
/* FICHIERS INCLUS -------------------------------*/
/*------------------------------------------------*/
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/*------------------------------------------------*/
/* DEFINITIONS -----------------------------------*/
/*------------------------------------------------*/
#define SWAP(a,b) tempr=(a);(a)=(b);(b)=tempr
#define SQUARE(X) ((X)*(X))
#define MAX(i,j)  ((i)>(j)?(i):(j))
#define MIN(i,j)  ((i)>(j)?(i):(j))

#define NBCHAR 200

#define FFT   1
#define IFFT -1
#define FFT2D 2

#define GREY_LEVEL 255
#define PI 3.141592654


/*------------------------------------------------*/
/* PROTOTYPES DES FONCTIONS  ---------------------*/
/*------------------------------------------------*/

//>Allocation
float*  fmatrix_allocate_1d(int);
float** fmatrix_allocate_2d(int,int);
void    free_fmatrix_1d(float*);
void    free_fmatrix_2d(float**);

//>FFT 2D-1D
void    fourn(float*,unsigned long*,int,int);
void    FFT1D(float*,float*,int);
void    IFFT1D(float*,float*,int);

//>Outils spectral 1D
void  ModVct(float*,float*,float*,int);
void  CenterVct(float*,int);

//>Degradation
float gaussian_noise(float);

//>Lecture/Sauve Signal
float* LoadSignalDat(char*,int*); 
void SaveSignalDat(char*,float*,int);
void SaveSignalDat2(char*,float*,int,float);

#endif
