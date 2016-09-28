#include "Utils.c"
#include "FonctionDemo3.h"

void CopyImg(float**, float**, int, int);
void PasteImg(float**, float**, int, int, float, float);
void PasteImgInt(float**, float**, int, int, int, int);
void CropImg0(float**, int, int, float**, int, int, int, int);
void DFT(float**, float**, int, int);
void IDFT(float**, float**, int, int);

float interpol0(float** img, int width, int height, float x, float y) {
  int rx = round(x);
  int ry = round(y);

  if (rx < 0 || rx >= width || ry < 0 || ry >= height) return 0;
  
  if (img[ry][rx] > 255) return 255;
  if (img[ry][rx] < 0) return 0;
  
  return img[ry][rx];
}

float interpol2(float** img, int width, int height, float x, float y)
{
  // Rounded vals
  int xf, xc, yf, yc;
  // Propotion factors
  float tl, tr, bl, br;

  xf = floor(x);
  xc = ceil(x);
  yf = floor(y);
  yc = ceil(y);

  if (xf == xc) xc++;
  if (yf == yc) yc++;
  
  // CALCUL DES PROPORTIONS
  tl = (xc - x) * (yc - y);
  tr = (x - xf) * (yc - y);
  bl = (xc - x) * (y - yf);
  br = (x - xf) * (y - yf);

  if (xc < 0 || xf >= width || yc < 0 || yf >= height) return 0;

  if (xf < 0) {  
    if (yf < 0)
      return br * img[yc][xc];   
    else if (yc >= height)
      return tr * img[yf][xc];
    else
      return br * img[yc][xc] + tr * img[yf][xc];    
  }
  else if (xc >= width) {
    if (yf < 0)
      return bl * img[yc][xf];
    else if (yc >= height)
      return tl * img[yf][xf];
    else
      return bl * img[yc][xf] + tl * img[yf][xf];
  }
  else if (yf < 0)
    return bl * img[yc][xf] + br * img[yc][xc];
  else if (yc >= height)
    return tl * img[yf][xf] + tr * img[yf][xc];
  return br * img[yc][xc] + tr * img[yf][xc] + bl * img[yc][xf] + tl * img[yf][xf];
}


void stretch0(float** orig, int owidth, int oheight, float** new, int nwidth, int nheight) {
  int i, j;
  float hstep = (float)(owidth - 1) / (float)(nwidth - 1);
  float vstep = (float)(oheight - 1) / (float)(nheight - 1);
  for (i = 0; i < nwidth; i++)
    for (j = 0; j < nheight; j++) {
      new[j][i] = interpol0(orig, owidth, oheight, i * hstep, j * vstep);
    }
}

void stretch2(float** orig, int owidth, int oheight, float** new, int nwidth, int nheight) {
  int i, j;
  float hstep = (float)(owidth - 1) / (float)(nwidth - 1);
  float vstep = (float)(oheight - 1) / (float)(nheight - 1);
  for (i = 0; i < nwidth; i++)
    for (j = 0; j < nheight; j++) {
      new[j][i] = interpol2(orig, owidth, oheight, i * hstep, j * vstep);
    }
}

void stretch(float** orig, int owidth, int oheight, float*** new, int nwidth, int nheight) {
  float** tmpOR = fmatrix_allocate_2d(oheight, owidth);
  float** tmpOI = fmatrix_allocate_2d(oheight, owidth);
  float** tmpNI = fmatrix_allocate_2d(nheight, nwidth);
  float factor = (nwidth / owidth) * (nheight / oheight);
  int px = (nwidth - owidth) / 2;
  int py = (nheight - oheight) / 2;
  int i, j;

  CopyImg(orig, tmpOR, owidth, oheight);

  FFTDD(tmpOR, tmpOI, oheight, owidth);

  RCenterImg(&tmpOR, owidth, oheight);
  RCenterImg(&tmpOI, owidth, oheight);

  PasteImgInt(*new, tmpOR, owidth, oheight, px, py);
  PasteImgInt(tmpNI, tmpOI, owidth, oheight, px, py);

  RCenterImg(new, nwidth, nheight);
  RCenterImg(&tmpNI, nwidth, nheight);

  IFFTDD(*new, tmpNI, nheight, nwidth);

  for (i = 0; i < nwidth; i++)
    for (j = 0; j < nheight; j++) {
      (*new)[j][i] *= factor;
      if ((*new)[j][i] > 255) (*new)[j][i] = 255;
      if ((*new)[j][i] < 0) (*new)[j][i] = 0;
    }

  free_fmatrix_2d(tmpOR);
  free_fmatrix_2d(tmpOI);
  free_fmatrix_2d(tmpNI);
}

void dammagedregion(float** img, int width, int height, int stops[4]) {
  int i, j;
  int minx = width, maxx = 0;
  int miny = height, maxy = 0;
  
  for (i = 0; i < width; i++)
    for (j = 0; j < width; j++)
      if (img[j][i] == 255) {
	if (i < minx) minx = i;
	if (i > maxx) maxx = i;
	if (j < miny) miny = j;
	if (j > maxy) maxy = j;
      }
  
  stops[0] = minx;
  stops[1] = maxx + 1;
  stops[2] = miny;
  stops[3] = maxy + 1;
}

void hardtreshold(float** imgR, float** imgI, int width, int height, float value) {
  int i, j;
  float tresh = value * imgR[0][0];
  for (i = 0; i < width; i++)
    for (j = 0; j < height; j++)
      if (hyp(imgR[j][i], imgI[j][i]) < tresh) {
	imgR[j][i] = 0;
	imgI[j][i] = 0;
      }
}

void correct(float** img, int width, int height, int lvl, float start) {
  int count = 0;
  int i, j, k, l, m;
  char imgname[50];
  float** copy = fmatrix_allocate_2d(height, width);
  float** workR = fmatrix_allocate_2d(height, width);
  float** workI = fmatrix_allocate_2d(height, width);
  int stops[4];
  dammagedregion(img, width, height, stops);
  int minx = stops[0], maxx = stops[1];
  int miny = stops[2], maxy = stops[3];
  int wwidth, wheight;
  int fx, fy;
  int p = 8;
  
  CopyImg(img, copy, width, height);
  CopyImg(img, workR, width, height);
  
  sprintf(imgname, "image-TpIFT3205-3-2-%d", count++);
  SaveImagePgm(imgname, img, height, width);
  
  for (k = 0; k < lvl; k++) {
    wwidth = width / pow(2, k);
    wheight = height / pow(2, k);
    
    workR = fmatrix_allocate_2d(wheight, wwidth);
    workI = fmatrix_allocate_2d(wheight, wwidth);

    for (l = 0; l <= k; l++)
      for (m = 0; m <= k; m++) {
	if (k > 0) {
	  fx = minx - p - (1.0 / k) * l * (wwidth - 2 * p + minx - maxx);
	  fy = miny - p - (1.0 / k) * m * (wheight - 2 * p + miny - maxy);
	} else fx = fy = 0;

	if (fx + wwidth > width) fx -= fx + wwidth - width;
	if (fy + wheight > height) fy -= fy + wheight - height;

        CropImg0(workR, wwidth, wheight, img, width, height, fx, fy);

	FFTDD(workR, workI, wheight, wwidth);

	hardtreshold(workR, workI, wwidth, wheight, start);
	start /= 2;
	
	IFFTDD(workR, workI, wheight, wwidth);

        for (i = minx < fx ? fx : minx; i < fx + wwidth && i < maxx; i++)
	  for (j = miny < fy ? fy : miny; j < fy + wheight && j < maxy; j++)
	    if (copy[j][i] == 255)
	      img[j][i] = workR[j - fy][i - fx];

	sprintf(imgname, "image-TpIFT3205-3-2-%d", count++);
	SaveImagePgm(imgname, img, height, width);
      }

    free_fmatrix_2d(workR);
    free_fmatrix_2d(workI);
  }
  
  free_fmatrix_2d(copy);
}

void preliminaryfill(float** img, float** new, int width, int height, int size)
{
  int i, j;
  PasteImgInt(new, img, width, height, 0, size);
  for (j = size - 1; j >= 0; j--) {
    new[j][0] = (new[j + 1][0] + new[j + 1][1]) / 2;
    for (i = 1; i < width - 1; i++) {
      new[j][i] = (new[j + 1][i - 1] + 2 * new[j + 1][i] + new[j + 1][i + 1]) / 4;
    }
    new[j][i] = (new[j + 1][i - 1] + new[j + 1][i]) / 2;
  }
}

void extrapolate(float** img, float** new, int width, int height, int size)
{
  float** tmpR, ** tmpI;
  int i, j, k, th;
  int iter = 7;
  float tresh = 0.02;
  char imgname[50];
  
  preliminaryfill(img, new, width, height, size);

  for (i = 1; i <= iter; i++) {
    fprintf(stderr, ".");
    th = 120;
    tmpR = fmatrix_allocate_2d(th, width);
    tmpI = fmatrix_allocate_2d(th, width);

    for (j = 0; j < width; j++)
      for (k = 0; k < th; k++)
	tmpI[k][j] = 0;
    
    CropImg0(tmpR, width, th, new, width, height + size, 0, 0);

    DFT(tmpR, tmpI, th, width);

    hardtreshold(tmpR, tmpI, width, th, tresh);
    tresh /= 2;

    IDFT(tmpR, tmpI, th, width);

    CropImg0(new, width, size, tmpR, width, th, 0, 0);

    free_fmatrix_2d(tmpR);
    free_fmatrix_2d(tmpI);
  }
}

/////////////////////// COPYING & PASTING \\\\\\\\\\\\\\\\\\\\\\\\\\\

void CopyImg(float** img, float** newImg, int width, int height)
{
  int i, j;

  for (i = 0; i < width; i++)
    for (j = 0; j < height; j++)
      newImg[j][i] = img[j][i];
}

void PasteImg(float** canvas, float** img, int width, int height, float x, float y)
{
  int i, j;
  int vecx = floor(x), vecy = floor(y);
  float val;
  x = 1.0 - x + vecx;
  y = 1.0 - y + vecy;
  for (i = -1; i < width + 1; i++)
    for (j = -1; j < height + 1; j++) {
      val = interpol2(img, width, height, i + x, j + y);
      canvas[vecy + j + 1][vecx + i + 1] = val;
    }
}

void PasteImgInt(float** canvas, float** img, int width, int height, int x, int y)
{
  int i, j;
  for (i = 0; i < width; i++)
    for (j = 0; j < height; j++)
      canvas[y + j][x + i] = img[j][i];
}

void CropImg0(float** canvas, int cw, int ch, float** img, int width, int height, int x, int y) {
  int i, j;
  for (i = 0; i < cw; i++)
    for (j = 0; j < ch; j++)
      canvas[j][i] = interpol0(img, width, height, i + x, j + y);
}
