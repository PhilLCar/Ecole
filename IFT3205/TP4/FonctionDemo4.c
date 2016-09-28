//------------------------------------------------------
// module  : FonctionDemo4.c
// auteur  : Mignotte Max
// date    :
// version : 1.0
// langage : C++
// labo    : DIRO
// note    :
//------------------------------------------------------
// 

//------------------------------------------------
// FICHIERS INCLUS -------------------------------
//------------------------------------------------
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "FonctionDemo4.h"

/*------------------------------------------------*/
/* DEFINITIONS -----------------------------------*/
/*------------------------------------------------*/
#define FFT   1
#define IFFT -1
#define FFT2D 2


//--------------------------//
//-- Matrice de Flottant --//
//--------------------------//
//---------------------------------------------------------
//  Alloue de la memoire pour une matrice 1d de float      
//---------------------------------------------------------
float* fmatrix_allocate_1d(int hsize)
 {
  float* matrix;

  matrix=(float*)malloc(sizeof(float)*hsize);
  if (matrix==NULL) printf("probleme d'allocation memoire");

  return matrix;
 }

//----------------------------------------------------------
//  Alloue de la memoire pour une matrice 2d de float       
//----------------------------------------------------------
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

//----------------------------------------------------------
// Libere la memoire de la matrice 1d de float              
//----------------------------------------------------------
void free_fmatrix_1d(float* pmat)
 {
  free(pmat);
 }

//----------------------------------------------------------
// Libere la memoire de la matrice 2d de float              
//----------------------------------------------------------
void free_fmatrix_2d(float** pmat)
 {
  free(pmat[0]);
  free(pmat);
 }

//--------------------//
//-- LOAD/SAVE/FILE --//
//--------------------//
//----------------------------------------------------------
// Chargement de l'image de nom <name> (en pgm)             
//----------------------------------------------------------
float** LoadImagePgm(char* name,int *length,int *width)
 {
  int i,j;
  unsigned char var;
  char buff[NBCHAR];
  int tmp;
  char* ptmp;
  float** mat;

  char stringTmp1[NBCHAR],stringTmp2[NBCHAR];
 
  int ta1,ta2,ta3;
  FILE *fic;

  //-----nom du fichier pgm-----
  strcpy(buff,name);
  strcat(buff,".pgm");
  printf("---> Ouverture de %s",buff);

  //----ouverture du fichier----
  fic=fopen(buff,"r");
  if (fic==NULL)
    { printf("\n- Grave erreur a l'ouverture de %s  -\n",buff);
      exit(-1); }

  //--recuperation de l'entete--
  ptmp=fgets(stringTmp1,100,fic);
  ptmp=fgets(stringTmp2,100,fic);
  tmp=fscanf(fic,"%d %d",&ta1,&ta2);
  tmp=fscanf(fic,"%d\n",&ta3);

  //--affichage de l'entete--
  printf("\n\n--Entete--");
  printf("\n----------");
  printf("\n%s%s%d %d \n%d\n",stringTmp1,stringTmp2,ta1,ta2,ta3);

  *length=ta1;
  *width=ta2;
  mat=fmatrix_allocate_2d(*length,*width);
   
  //--chargement dans la matrice--
     for(i=0;i<*length;i++)
      for(j=0;j<*width;j++)  
        { tmp=fread(&var,1,1,fic);
          mat[i][j]=var; }

   //---fermeture du fichier---
  fclose(fic);

  return(mat);
 }


//----------------------------------------------------------
// Sauvegarde de l'image de nom <name> au format pgm        
//----------------------------------------------------------
void SaveImagePgm(char* name,float** mat,int length,int width)
 {
  int i,j;
  char buff[NBCHAR];
  FILE* fic;
  time_t tm;

  //--extension--
  strcpy(buff,name);
  strcat(buff,".pgm");

  //--ouverture fichier--
  fic=fopen(buff,"w");
    if (fic==NULL) 
        { printf(" Probleme dans la sauvegarde de %s",buff); 
          exit(-1); }
  printf("\n Sauvegarde de %s au format pgm\n",name);

  //--sauvegarde de l'entete--
  fprintf(fic,"P5");
  if ((ctime(&tm))==NULL) fprintf(fic,"\n#\n");
  else fprintf(fic,"\n# IMG Module, %s",ctime(&tm));
  fprintf(fic,"%d %d",width,length);
  fprintf(fic,"\n255\n");

  //--enregistrement--
     for(i=0;i<length;i++)
      for(j=0;j<width;j++) 
        fprintf(fic,"%c",(char)mat[i][j]);
   
  //--fermeture fichier--
   fclose(fic); 
 } 

//-------------//
//-- FOURIER --//
//-------------//
//--------------------------------------------------------------
//  FOURN ------------------------------------------------------
//--------------------------------------------------------------
void fourn(float data[], unsigned long nn[], int ndim, int isign)
{
	int idim;
	unsigned long i1,i2,i3,i2rev,i3rev,ip1,ip2,ip3,ifp1,ifp2;
	unsigned long ibit,k1,k2,n,nprev,nrem,ntot;
	float tempi,tempr;
	double theta,wi,wpi,wpr,wr,wtemp;
        float temp;

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
                                                { temp=data[i3]; data[i3]=data[i3rev]; data[i3rev]=temp; }
						{ temp=data[i3+1]; data[i3+1]=data[i3rev+1]; data[i3rev+1]=temp; }
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


//----------------------------------------------------------
// FFTDD                                                    
//----------------------------------------------------------
void FFTDD(float** mtxR,float** mtxI,int lgth, int wdth)
{
 int i,j;
 int posx,posy;

 float* data;
 float* ImgFreqR;
 float* ImgFreqI;
 unsigned long* nn;

 //allocation memoire
 data=(float*)malloc(sizeof(float)*(2*wdth*lgth)+1);
 ImgFreqR=(float*)malloc(sizeof(float)*(wdth*lgth));
 ImgFreqI=(float*)malloc(sizeof(float)*(wdth*lgth));
 nn=(unsigned long*)malloc(sizeof(unsigned long)*(FFT2D+1)); 

 //Remplissage de nn
 nn[1]=lgth; nn[2]=wdth;

 //Remplissage de data
 for(i=0;i<lgth;i++) for(j=0;j<wdth;j++) 
   { data[2*(i*lgth+j)+1]=mtxR[i][j];
     data[2*(i*lgth+j)+2]=mtxI[i][j]; }

 //FFTDD
 fourn(data,nn,FFT2D,FFT);

 //Remplissage
 for(i=0;i<(wdth*lgth);i++)
  { ImgFreqR[i]=data[(2*i)+1];
    ImgFreqI[i]=data[(2*i)+2];  }

 //Conversion en Matrice
 for(i=0;i<(wdth*lgth);i++)
  { posy=(int)(i/wdth);
    posx=(int)(i%wdth);

    mtxR[posy][posx]=ImgFreqR[i]/(wdth*lgth);  
    mtxI[posy][posx]=ImgFreqI[i]/(wdth*lgth); }

 //Liberation memoire
 free(data);
 free(ImgFreqR);
 free(ImgFreqI);
 free(nn);
}

//----------------------------------------------------------
// IFFTDD                                                   
//----------------------------------------------------------
void IFFTDD(float** mtxR,float**  mtxI,int lgth,int wdth)
{
 int i,j;
 int posx,posy;

 float* data;
 float* ImgFreqR;
 float* ImgFreqI;
 unsigned long* nn;

 //allocation memoire
 data=(float*)malloc(sizeof(float)*(2*wdth*lgth)+1);
 ImgFreqR=(float*)malloc(sizeof(float)*(wdth*lgth));
 ImgFreqI=(float*)malloc(sizeof(float)*(wdth*lgth));
 nn=(unsigned long*)malloc(sizeof(unsigned long)*(FFT2D+1));

 //Remplissage de nn
 nn[1]=lgth; nn[2]=wdth;

 //Remplissage de data
 for(i=0;i<lgth;i++) for(j=0;j<wdth;j++) 
   { data[2*(i*lgth+j)+1]=mtxR[i][j];
     data[2*(i*lgth+j)+2]=mtxI[i][j]; }

 //FFTDD
 fourn(data,nn,FFT2D,IFFT);

 //Remplissage
 for(i=0;i<(wdth*lgth);i++)
  { ImgFreqR[i]=data[(2*i)+1];
    ImgFreqI[i]=data[(2*i)+2]; }

 //Conversion en Matrice
 for(i=0;i<(wdth*lgth);i++)
  { posy=(int)(i/wdth);
    posx=(int)(i%wdth);

   mtxR[posy][posx]=ImgFreqR[i];  
   mtxI[posy][posx]=ImgFreqI[i]; }

 //Liberation memoire
 free(data);
 free(ImgFreqR);
 free(ImgFreqI);
 free(nn);
}

//-------------------//
//--- DEGRADATION ---//
//-------------------//
//----------------------------------------------------------
// retourne un nombre aleatoire entre zero et un
//----------------------------------------------------------
float randomize(void)
{ return ((float)rand()/RAND_MAX); }

//----------------------------------------------------------
//  bruit gaussien  
//----------------------------------------------------------
float gaussian_noise(float var,float mean)
{
 float noise,theta;

 //generation du bruit
 noise=sqrt(-2*var*log(1.0-((float)rand()/RAND_MAX)));
 theta=(float)rand()*1.9175345E-4-PI;
 noise=noise*cos(theta);
 noise+=mean;
 if (noise>GREY_LEVEL) noise=GREY_LEVEL;
 if (noise<0) noise=0;
 return noise;
}

//----------------------------------------------------------
// Degradation Possible                       
//----------------------------------------------------------                
void degradation(float** mat,float** mat_degrad,int lgth,int wdth,int mtd,int sz,float vrnoise)
{
 int i,j;
 float mean,variance;
 
 //Information
 printf("\n -----");
 printf("\n Methode [%d]",mtd);
 printf("\n    size [%d]",sz);
 printf("\n    variance noise [%.4f]",vrnoise); 
 printf("\n -----");
 printf("\n\n");

 //allocation memoire
 float** mat_noise=fmatrix_allocate_2d(lgth,wdth);
 float** mat_blur=fmatrix_allocate_2d(lgth,wdth);

 //Matrice Bruit
 mean=0.0;
 variance=0.0;
 for(i=0;i<lgth;i++) for(j=0;j<wdth;j++) 
   { mat_noise[i][j]=gaussian_noise(vrnoise,128.0);
     mat_noise[i][j]-=128.0;
     mean+=mat_noise[i][j]; }
 mean/=(wdth*lgth);
 printf("\n Bruit > Moyenne=[%.3f]",mean);

 for(i=0;i<lgth;i++) for(j=0;j<wdth;j++) 
   variance+=CARRE(mat_noise[i][j]-mean);
 variance/=(wdth*lgth);
 printf(" Variance=[%.3f]",variance);

 //Floue
 if (mtd==1)
    { printf("\n Psf -1- Uniform Blur ([size:%dx%d] [noise:%.2f])",sz,sz,vrnoise);
      convolveUB(mat,mat_degrad,lgth,wdth,sz); }

 if (mtd==2)
    { printf("\n Psf -2- (1/1+i2+j2) Blur ([size:%d][noise:%.3f])",sz,vrnoise);
      convolvePAR(mat,mat_degrad,lgth,wdth,sz); }

 if (mtd==3)
    { printf("\n Psf -3- (1-4-6-4-1)/16 Blur (horizontal & vertical)");
      convolveGAUSS(mat,mat_degrad,lgth,wdth); } 

 //Copie Matrice Floue
  for(i=0;i<lgth;i++) for(j=0;j<wdth;j++) mat_blur[i][j]=mat_degrad[i][j]; 

 //Bruit
 for(i=0;i<lgth;i++) for(j=0;j<wdth;j++) 
   {
    mat_degrad[i][j]+=mat_noise[i][j];
    if (mat_degrad[i][j]>255) mat_degrad[i][j]=255;
    if (mat_degrad[i][j]<0)   mat_degrad[i][j]=0;
   }

 //desallocation memoire
 free_fmatrix_2d(mat_noise);
 free_fmatrix_2d(mat_blur);
}

//-Differents Types de flous
//
//----------------------------------------------------------
// Uniform Blur                   
//----------------------------------------------------------                
void convolveUB(float** mt_img,float** mt_imgdeg,int lgth,int wdth,int sz)
{
 int i,j,k,l,m,n;
 int halfsize;
 float sum;

 //Init
 halfsize=(int)(sz/2);
 printf(" -[halfsize=%d]-",halfsize);

 //convolution
  for(i=0;i<lgth;i++) for(j=0;j<wdth;j++) 
   {
    sum=0.0;
    for(k=-halfsize;k<=halfsize;k++) for(l=-halfsize;l<=halfsize;l++)
       {
         m=0;
	 if ((i+k)<0)          m=lgth;
         if ((i+k)>(lgth-1))   m=-lgth;

         n=0;
         if ((j+l)<0)          n=wdth;
         if ((j+l)>(wdth-1))   n=-wdth;

         sum+=mt_img[i+k+m][j+l+n];
       }
    sum/=(CARRE(sz));
    mt_imgdeg[i][j]=(int)sum;
   }
}

//----------------------------------------------------------
// floue (1/1+i2+j2)              
//----------------------------------------------------------                
void convolvePAR(float** mt_img,float** mt_imgdeg,int lgth,int wdth,int sz)
{
 int i,j,k,l,m,n;
 int halfsize;
 float sum;
 float coef;
 float sum_coef;

 //Init
 halfsize=(int)(sz/2);
 printf(" -[halfsize=%d]-",halfsize);

 sum_coef=0.0;
 for(k=-halfsize;k<=halfsize;k++) for(l=-halfsize;l<=halfsize;l++)
     sum_coef+=1.0/(1+CARRE(k)+CARRE(l));

 printf("\n Somme des coeffs: %.4f",sum_coef);
 
 //convolution
  for(i=0;i<lgth;i++) for(j=0;j<wdth;j++) 
   {
    sum=0.0;
    for(k=-halfsize;k<=halfsize;k++) for(l=-halfsize;l<=halfsize;l++)
       {
         m=0;
	 if ((i+k)<0)          m=lgth;
         if ((i+k)>(lgth-1))   m=-lgth;

         n=0;
         if ((j+l)<0)          n=wdth;
         if ((j+l)>(wdth-1))   n=-wdth;

         coef=1.0/(1+CARRE(k)+CARRE(l));
         sum+=coef*mt_img[i+k+m][j+l+n];
       }

    sum/=(sum_coef);
    mt_imgdeg[i][j]=(int)sum;
   }
}

//----------------------------------------------------------
// floue Gaussien (1-4-6-4-1)/16 (horizontal & vertical)              
//----------------------------------------------------------                
void convolveGAUSS(float** mt_img,float** mt_imgdeg,int lgth,int wdth)
{
 int i,j,k,l,m,n;
 float sum;
 float coef[5];

 //Init
 l=0;  n=0;

 coef[0]=1.0;
 coef[1]=4.0;
 coef[2]=6.0;
 coef[3]=4.0;
 coef[4]=1.0;

 //allocation memoire
 float** mat_tmp=fmatrix_allocate_2d(lgth,wdth);

 //convolution horizontal
  for(i=0;i<lgth;i++) for(j=0;j<wdth;j++) 
   {
    sum=0.0;
    for(l=-2;l<=2;l++)
       {
         n=0;
         if ((j+l)<0)           n=wdth;
         if ((j+l)>(wdth-1))    n=-wdth;

         sum+=coef[l+2]*mt_img[i][j+l+n];
       }

    sum/=16.0;
    mt_imgdeg[i][j]=(int)sum;
   }

  //copiage dans mat_tampon
  for(i=0;i<lgth;i++) for(j=0;j<wdth;j++)   mat_tmp[i][j]=mt_imgdeg[i][j];

 //convolution vertical
  for(i=0;i<lgth;i++) for(j=0;j<wdth;j++) 
   {
    sum=0.0;
    for(k=-2;k<=2;k++) 
       {
         m=0;
	 if ((i+k)<0)          m=lgth;
         if ((i+k)>(lgth-1))   m=-lgth;

         sum+=coef[k+2]*mat_tmp[i+k+m][j+l+n];
       }

    sum/=16.0;
    mt_imgdeg[i][j]=(int)sum;
   }

 //desallocation memoire
 free_fmatrix_2d(mat_tmp);

}






