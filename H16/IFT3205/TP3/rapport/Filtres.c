#define FILTER_IN 0
#define FILTER_OUT 1
#define FILTER_HORIZONTAL 2
#define FILTER_VERTICAL 4
#define FILTER_POSITIVE 8


////////////////////////// FILTERS //////////////////////////////

/*-------------------2D FILTERS-------------------*/
/* Square Filter
 * Cette fonction défini un carré puis en fonction du flag FILTER_IN ou FILTER_OUT
 * multiplie l'intérieur ou l'extérieur respectivement par le facteur mult
 */
void SquareFilter(float** img, int iwidth, int iheight, int fwidth, int fheight, float mult, int flags)
{
  int hstop = (iwidth - fwidth) / 2;
  int vstop = (iheight - fheight) / 2;
  int i, j;
  if (flags & 1) {
    for (i = 0; i < iwidth; i++)
      for (j = 0; j < iheight; j++)
	if (i >= hstop && i < iwidth - hstop && j >= vstop && j < iheight - vstop)
	    img[j][i] *= mult;
  }
  else
    for (i = 0; i < iwidth; i++)
	for (j = 0; j < iheight; j++)
	  if (i < hstop || i >= iwidth - hstop || j < vstop || j >= iheight - vstop)
	      img[j][i] *= mult;
	    
}

/* Round Filter
 * Cette fonction défini un cercle puis en fonction du flag FILTER_IN ou FILTER_OUT
 * multiplie l'intérieur ou l'extérieur respectivement par le facteur mult
 */
void RoundFilter(float** img, int iwidth, int iheight, int fwidth, int fheight, float mult, int flags)
{
  int hcenter = iwidth / 2;
  int vcenter = iheight / 2;
  int i, j;
  if (flags & 1)
    {
       for (i = 0; i < iwidth; i++)
	for (j = 0; j < iheight; j++)
	  if (1 >= sqrt(sqr((i - hcenter) / (float)fwidth) + sqr((j - vcenter) / (float)fheight)))
	    img[j][i] *= mult;
    }
  else
    for (i = 0; i < iwidth; i++)
	for (j = 0; j < iheight; j++)
	  if (1 < sqrt(sqr((i - hcenter) / (float)fwidth) + sqr((j - vcenter) / (float)fheight)))
	    img[j][i] *= mult;
}

/* Bar Filter
 * Cette fonction défini un carré dont la largeur ou la hauteur correspond à la largeur
 * ou la hauteur de l'image selon les flags FILTER_HORIZONTAL et FILTER_VERTICAL
 * respectivement puis en fonction du flag FILTER_IN ou FILTER_OUT multiplie
 * l'intérieur ou l'extérieur respectivement par le facteur mult
 */
void BarFilter(float** img, int width, int height, int size, float mult, int flags)
{
  if (flags >> 2)
    SquareFilter(img, width, height, size, height, mult, flags);
  else
    SquareFilter(img, width, height, width, size, mult, flags);
}

/*-------------------3D FILTERS-------------------*/
/* Cone Filter
 * Cette fonction défini un cercle puis en fonction du flag FILTER_IN ou FILTER_OUT
 * multiplie l'intérieur ou l'extérieur respectivement par le facteur calculé par
 * la fonction func passée en paramètre, celui-ci peut être inversé par le flag
 * FILTER_POSITIVE
 */
void ConeFilter(float** img, int iwidth, int iheight, int fwidth, int fheight, float (*func)(float, float), int flags)
{
  int hcenter = iwidth / 2;
  int vcenter = iheight / 2;
  int pos = flags >> 3;
  float stop;
  if (pos) stop = sqrt(sqr(hcenter) + sqr(vcenter));
  else stop = sqrt(sqr(fwidth / 2) + sqr(fheight / 2));
  int i, j;
  if (flags & 1)
    {
       for (i = 0; i < iwidth; i++)
	for (j = 0; j < iheight; j++)
	  if (1 >= sqrt(sqr((i - hcenter) / (float)fwidth) + sqr((j - vcenter) / (float)fheight)))
	    if (pos) img[j][i] *= 1 - func(sqrt(sqr(i - hcenter) + sqr(j - vcenter)), stop);
	    else img[j][i] *= func(sqrt(sqr(i - hcenter) + sqr(j - vcenter)), stop);
    }
  else
    for (i = 0; i < iwidth; i++)
	for (j = 0; j < iheight; j++)
	  if (1 < sqrt(sqr((i - hcenter) / (float)fwidth) + sqr((j - vcenter) / (float)fheight)))
	    if (pos) img[j][i] *= 1 - func(sqrt(sqr(i - hcenter) + sqr(j - vcenter)), stop);
	    else img[j][i] *= func(sqrt(sqr(i - hcenter) + sqr(j - vcenter)), stop);
}


//////////////////////// Z FUNCTIONS ////////////////////////////

/* Straight
 * Définie une progression constante pour les filtre tri-dimensionnels
 */
float Straight(float r, float stop)
{
  return r / stop;
}

/* Square
 * Définie une progression carrée pour les filtre tri-dimensionnels
 */
float Square(float r, float stop)
{
  return sqr(r) / sqr(stop);
}

/* Straight
 * Définie une progression logaritmique pour les filtre tri-dimensionnels
 */
float Logarithm(float r, float stop)
{
  return log(r) / log(stop);
}
