/*---------------------------------------------------*/
/* module  : FonctionDemo6.c                         */
/* auteur  : Mignotte Max                            */
/* date    : --/--/2010                              */    
/* langage : C                                       */
/* labo    : DIRO                                    */
/*---------------------------------------------------*/

/*------------------------------------------------*/
/* FICHIERS INCLUS -------------------------------*/
/*------------------------------------------------*/
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "FonctionDemo6.h"

/*------------------------------------------------*/
/* FONCTIONS -------------------------------------*/
/*------------------------------------------------*/
/*---------------------------------------------------------*/
/*  Alloue de la memoire pour une matrice 1d de float      */
/*---------------------------------------------------------*/
float* fmatrix_allocate_1d(int hsize)
 {
  float* matrix;

  matrix=(float*)malloc(sizeof(float)*hsize); 
  if (matrix==NULL) printf("probleme d'allocation memoire");

  return matrix; 
 }

/*----------------------------------------------------------*/
/*  Alloue de la memoire pour une matrice 2d de float       */
/*----------------------------------------------------------*/
float** fmatrix_allocate_2d(int vsize,int hsize)
 {
  int i;
  float** matrix;
  float *imptr;

  matrix=(float**)malloc(sizeof(float*)*vsize);
  if (matrix==NULL) printf("probleme d'allocation memoire");

  imptr=(float*)malloc(sizeof(float)*hsize*vsize);
  if (imptr==NULL) printf("probleme d'allocation memoire");
 
  for(i=0;i<vsize;i++,imptr+=hsize) matrix[i]=imptr;
  return matrix;
 }

/*----------------------------------------------------------*/
/* Libere la memoire de la matrice 1d de float              */
/*----------------------------------------------------------*/
void free_fmatrix_1d(float* pmat)
 { 
  free(pmat); 
 }

//----------------------------------------------------------*/
/* Libere la memoire de la matrice 2d de float              */
/*----------------------------------------------------------*/
void free_fmatrix_2d(float** pmat)
 { 
  free(pmat[0]);
  free(pmat);
 }

/*--------------*/
/* FOURIER FFT--*/
/*--------------*/
/*------------------------------------------------*/
/*  FOURN ----------------------------------------*/
/*------------------------------------------------*/
void fourn(float data[], unsigned long nn[], int ndim, int isign)
{
	int idim;
	unsigned long i1,i2,i3,i2rev,i3rev,ip1,ip2,ip3,ifp1,ifp2;
	unsigned long ibit,k1,k2,n,nprev,nrem,ntot;
	float tempi,tempr;
	double theta,wi,wpi,wpr,wr,wtemp;

	for (ntot=1,idim=1;idim<=ndim;idim++)
		ntot *= nn[idim];
	nprev=1;
	for (idim=ndim;idim>=1;idim--) {
		n=nn[idim];
		nrem=ntot/(n*nprev);
		ip1=nprev << 1;
		ip2=ip1*n;
		ip3=ip2*nrem;
		i2rev=1;
		for (i2=1;i2<=ip2;i2+=ip1) {
			if (i2 < i2rev) {
				for (i1=i2;i1<=i2+ip1-2;i1+=2) {
					for (i3=i1;i3<=ip3;i3+=ip2) {
						i3rev=i2rev+i3-i2;
						SWAP(data[i3],data[i3rev]);
						SWAP(data[i3+1],data[i3rev+1]);
					}
				}
			}
			ibit=ip2 >> 1;
			while (ibit >= ip1 && i2rev > ibit) {
				i2rev -= ibit;
				ibit >>= 1;
			}
			i2rev += ibit;
		}
		ifp1=ip1;
		while (ifp1 < ip2) {
			ifp2=ifp1 << 1;
			theta=isign*6.28318530717959/(ifp2/ip1);
			wtemp=sin(0.5*theta);
			wpr = -2.0*wtemp*wtemp;
			wpi=sin(theta);
			wr=1.0;
			wi=0.0;
			for (i3=1;i3<=ifp1;i3+=ip1) {
				for (i1=i3;i1<=i3+ip1-2;i1+=2) {
					for (i2=i1;i2<=ip3;i2+=ifp2) {
						k1=i2;
						k2=k1+ifp1;
						tempr=(float)wr*data[k2]-(float)wi*data[k2+1];
						tempi=(float)wr*data[k2+1]+(float)wi*data[k2];
						data[k2]=data[k1]-tempr;
						data[k2+1]=data[k1+1]-tempi;
						data[k1] += tempr;
						data[k1+1] += tempi;
					}
				}
				wr=(wtemp=wr)*wpr-wi*wpi+wr;
				wi=wi*wpr+wtemp*wpi+wi;
			}
			ifp1=ifp2;
		}
		nprev *= n;
	}
}

/*----------------------------------------------------------*/
/* FFT1D                                                    */
/*----------------------------------------------------------*/
void FFT1D(float* vctR,float* vctI,int lgth)
{
 int i;
 float* data;
 unsigned long* nn;

 /*allocation memoire*/
 data=(float*)malloc(sizeof(float)*((2*lgth)+1));
 nn=(unsigned long*)malloc(sizeof(unsigned long)*2); 
 nn[1]=lgth; 

 /*Remplissage de data*/
 for(i=0;i<lgth;i++)  
   { data[(2*i)+1]=vctR[i];
     data[(2*i)+2]=vctI[i]; 
    }

 /*FFTDD*/
 fourn(data,nn,1,FFT);

 /*Resultat*/
 for(i=0;i<lgth;i++)
  { vctR[i]=data[(2*i)+1];
    vctI[i]=data[(2*i)+2];  }

 /*Liberation memoire*/
 free(data);
 free(nn);
}


/*----------------------------------------------------------*/
/* IFFT1D                                                   */
/*----------------------------------------------------------*/
void IFFT1D(float* vctR,float* vctI,int lgth)
{
 int i;
 float* data;
 unsigned long* nn;

 /*allocation memoire*/
 data=(float*)malloc(sizeof(float)*((2*lgth)+1));
 nn=(unsigned long*)malloc(sizeof(unsigned long)*2); 
 nn[1]=lgth; 

 /*Remplissage de data*/
 for(i=0;i<lgth;i++)  
   { data[(2*i)+1]=vctR[i];
     data[(2*i)+2]=vctI[i]; }

 /*FFTDD*/
 fourn(data,nn,1,IFFT);

 /*Resultat*/
 for(i=0;i<lgth;i++)
  { vctR[i]=data[(2*i)+1];
    vctI[i]=data[(2*i)+2];  }

 /*Liberation memoire*/
 free(data);
 free(nn);
}

//--------------------------//
//--- OUTILS SPECTRAL 1D ---//
//--------------------------//
/*----------------------------------------------------------*/
/* ModVct                                                     */
/*----------------------------------------------------------*/
void ModVct(float* vctM,float* vctR,float* vctI,int lgth)
{
 int i,j;

 /*Calcul du module*/
 for(i=0;i<lgth;i++)
 vctM[i]=sqrt((vctR[i]*vctR[i])+(vctI[i]*vctI[i]));
}

/*----------------------------------------------------------*/
/*  CenterVct                                               */
/*----------------------------------------------------------*/
void CenterVct(float* vct,int lgth)
{
 int i;
 int ci;
 float* vcttmp;

 /*Initialisation*/
 ci=(int)(lgth/2);

 /*Allocation memoire*/
 vcttmp=fmatrix_allocate_1d(lgth);

 /*Recadrage*/
 for(i=0;i<ci;i++) vcttmp[ci+i]=vct[i];
 for(i=ci;i<lgth;i++) vcttmp[i-ci]=vct[i];

 /*Transfert*/
 for(i=0;i<lgth;i++) vct[i]=vcttmp[i];

 /*desallocation memoire*/
 free_fmatrix_1d(vcttmp);
}

//-------------------//
//--- DEGRADATION ---//
//-------------------//
//----------------------------------------------------------
//  Gaussian noise  
//----------------------------------------------------------
float gaussian_noise(float var)
{
 float noise,theta;

 //Noise generation 
 noise=sqrt(-2*var*log(1.0-((float)rand()/RAND_MAX)));
 theta=(float)rand()*1.9175345E-4-PI;
 noise=noise*cos(theta);
 return noise;
}

//-----------------------------//
// -LECTURE/SAUVEGARDE SIGNAL -//
//-----------------------------//
/*----------------------------------------------------------*/
/* Chargement du signal de nom <name>.dat                   */
/*----------------------------------------------------------*/
float* LoadSignalDat(char* name,int* length)
 {
  int i;
  float ech1,ech2;
  char buff[NBCHAR];
  float Tech;
  FILE *fic;

  //Allocation
  float** Vct2D=fmatrix_allocate_2d(2,10000000);

  //nom du fichier dat
  strcpy(buff,name);
  strcat(buff,".dat");
  printf("\n  >> Ouverture de %s",buff);

  //ouverture du fichier
  fic=fopen(buff,"r");
  if (fic==NULL)
    { printf("\n- Grave erreur a l'ouverture de %s  -\n",buff);
      exit(-1); }
 
  //Lecture Donnée & Longueur & Periode Ech
  for(i=0;;i++)
       { fscanf(fic,"%f %f",&ech1,&ech2);
         if (feof(fic)) break;
         //printf("\n[%f::%f]",ech1,ech2);
         Vct2D[0][i]=ech1;
         Vct2D[1][i]=ech2; }

  (*length)=i;
  Tech=Vct2D[0][1];
  Tech=1.0/Tech;
  Tech=(int)Tech;
  printf(" (%d echantillons)",(*length));
  printf("\n  >> Techantillonnage:: %.0f echantillons/seconde",Tech);

  //Chargement
  float* VctFinal=fmatrix_allocate_1d((*length));
  for(i=0;i<(*length);i++) VctFinal[i]=Vct2D[1][i];
 
   //End 
  fclose(fic);
  (*length)=i;
  free_fmatrix_2d(Vct2D);
  return VctFinal;
 }

/*----------------------------------------------------------*/
/* Sauvegarde du signal de nom <name>.dat                   */
/*----------------------------------------------------------*/
void SaveSignalDat(char* name,float* vct,int length)
 {
  int i;
  char buff[NBCHAR];
  FILE* fic;

  /*--extension--*/
  strcpy(buff,name);
  strcat(buff,".dat");

  /*--ouverture fichier--*/
  fic=fopen(buff,"w");
    if (fic==NULL) 
        { printf(" Probleme dans la sauvegarde de %s",buff); 
          exit(-1); }
  printf("\n Sauvegarde de %s au format dat\n",name);

  /*--enregistrement--*/
  for(i=0;i<length;i++) fprintf(fic,"%f %f\n",(float)i,vct[i]);

   /*--fermeture fichier--*/
   fclose(fic);  
 }

/*----------------------------------------------------------*/
/* Sauvegarde du signal de nom <name>.dat                   */
/*----------------------------------------------------------*/
void SaveSignalDat2(char* name,float* vct,int length,float Tech)
 {
  int i;
  char buff[NBCHAR];
  FILE* fic;

  /*--extension--*/
  strcpy(buff,name);
  strcat(buff,".dat");

  /*--ouverture fichier--*/
  fic=fopen(buff,"w");
    if (fic==NULL) 
        { printf(" Probleme dans la sauvegarde de %s",buff); 
          exit(-1); }
  printf("\n Sauvegarde de %s au format dat\n",name);

  /*--enregistrement--*/
  for(i=0;i<length;i++) fprintf(fic,"%f %f\n",(float)i*Tech,vct[i]);

   /*--fermeture fichier--*/
   fclose(fic);  
 }
