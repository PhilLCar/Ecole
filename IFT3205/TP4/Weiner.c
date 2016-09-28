#include "IEP.c"
#include "FonctionDemo4.h"

void Mod(float **imgR, float **imgI, float **imgM, int width, int height)
{
  int i, j;
  for (i = 0; i < width; i++)
    for(j = 0; j < height; j++) {
      imgM[j][i] = hyp(imgR[j][i], imgI[j][i]);
    }
}

void Recal(float **img, float width, float height)
{
  int i, j;
  for (i = 0; i < width; i++)
    for(j = 0; j < height; j++) {
      if (img[j][i] > 255) img[j][i] = 255;
      else if (img[j][i] < 0) img[j][i] = 0;
    }
}

void WienerFilter(float **g, float **h, float **f, int width, int height, float sigma)
{
  int i, j;
  float den;
  float **gi = fmatrix_allocate_2d(height, width);
  float **hi = fmatrix_allocate_2d(height, width);
  float **fi = fmatrix_allocate_2d(height, width);
  float **hm = fmatrix_allocate_2d(height, width);
  float **fm = fmatrix_allocate_2d(height, width);

  sigma /= width * height;

  FFTDD(g, gi, height, width);
  FFTDD(h, hi, height, width);
  FFTDD(f, fi, height, width);

  Mod(f, fi, fm, width, height);
  Mod(h, hi, hm, width, height);

  for (i = 0; i < width; i++)
    for (j = 0; j < height; j++) {
      den = sqr(hm[j][i]) + sigma / sqr(fm[j][i]);
      // R R
      f[j][i]   =  g[j][i] *   h[j][i] / den;
      // I I
      f[j][i]  += gi[j][i] * -hi[j][i] / den;
      // R I
      fi[j][i]  =  g[j][i] * -hi[j][i] / den;
      // I R
      fi[j][i] += gi[j][i] *   h[j][i] / den;
    }

  IFFTDD(f, fi, height, width);

  Recal(f, width, height);

  FreeImg(gi, hi);
  FreeImg(fm, hm);
  free_fmatrix_2d(fi);
}

float ISNR(float **fo, float **g, float **fr, int width, int height)
{
  float deg = 0;
  float res = 0;
  int i, j;
  for (i = 0; i < width; i++)
    for (j = 0; j < height; j++) {
      deg += sqr(fo[j][i] -  g[j][i]);
      res += sqr(fo[j][i] - fr[j][i]);
    }
  return 10 * log(deg / res) / log(10);
}

float Iterate(float **o, float **g, float **h, float **f, int width, int height, float sigma, int iter)
{
  int i, j;
  float isnr, max = 0;
  float **imgMax = fmatrix_allocate_2d(height, width);
  float **go = fmatrix_allocate_2d(height, width);
  float **ho = fmatrix_allocate_2d(height, width);

  CopyImg(g, go, width, height);
  CopyImg(h, ho, width, height);

  for (i = 0; i < iter; i++) {
    WienerFilter(g, h, f, width, height, sigma);
    
    CopyImg(go, g, width, height);
    CopyImg(ho, h, width, height);
    
    isnr = ISNR(o, go, f, width, height);
    if (isnr > max) {
      max = isnr;
      CopyImg(f, imgMax, width, height);
    }
    
    fprintf(stderr, "%2d >> ISNR: %.3f db\n", i + 1, isnr);
  }

  CopyImg(imgMax, f, width, height);

  FreeImg3(go, imgMax, ho);

  return max;
}
