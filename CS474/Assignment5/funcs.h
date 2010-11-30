#ifndef JDG_FUNCTIONS
#define JDG_FUNCTIONS

#include "image.h"

namespace numrec
{

#define SWAP(a,b) tempr=(a);(a)=(b);(b)=tempr

void fft(float data[], unsigned long nn, int isign)
{
  unsigned long n,mmax,m,j,istep,i;
  double wtemp,wr,wpr,wpi,wi,theta;
  float tempr,tempi;

  n=nn << 1;
  j=1;
  for (i=1;i<n;i+=2) {
    if (j > i) {
      SWAP(data[j],data[i]);
      SWAP(data[j+1],data[i+1]);
    }
    m=n >> 1;
    while (m >= 2 && j > m) {
      j -= m;
      m >>= 1;
    }
    j += m;
  }
  mmax=2;
  while (n > mmax) {
    istep=mmax << 1;
    theta=isign*(6.28318530717959/mmax);
    wtemp=sin(0.5*theta);
    wpr = -2.0*wtemp*wtemp;
    wpi=sin(theta);
    wr=1.0;
    wi=0.0;
    for (m=1;m<mmax;m+=2) {
      for (i=m;i<=n;i+=istep) {
        j=i+mmax;
        tempr=wr*data[j]-wi*data[j+1];
        tempi=wr*data[j+1]+wi*data[j];
        data[j]=data[i]-tempr;
        data[j+1]=data[i+1]-tempi;
        data[i] += tempr;
        data[i+1] += tempi;
      }
      wr=(wtemp=wr)*wpr-wi*wpi+wr;
      wi=wi*wpr+wtemp*wpi+wi;
    }
    mmax=istep;
  }
}
#undef SWAP

} // namespace numrec

namespace jdg
{

// 2D Fast Fourier Transform

// val = 1 is forward, -1 is inverse
template <class pType>
void fft( Image<std::complex<pType> >& f, int val=1 );

// convolution using convolution theorem (done in freq domain)
template <class pType>
void convolve( Image<std::complex<pType> >& img,
  const Image<std::complex<pType> >& kernel, const PadWith=NEAREST );

// convolution using spacial domain (slow)
template <class pType>
void convolve_spacial( Image<pType>& img,
  const Image<pType>& kernel, const PadWith=NEAREST );

// the value for n should be positive for a highpass and positive for lowpass
// D0    : cuttoff frequency (1pixel=1Hz)
// n     : order             (higher means faster convergence)
// gammaL: lower horizontal asymptote
// gammaH: upper horizontal asymptote
template <class pType>
void butterworth( jdg::Image<pType>& filter, float D0, float n=1.0,
  float gammaL=0.0, float gammaH=1.0 );

// build a gaussian filter type > 0 for highpass and < 0 for lowpass
// D0    : variance          (in pixels)
// type  : high or low pass
// gammaL: lower horizontal asymptote
// gammaH: upper horizontal asymptote
template <class pType>
void gaussian( jdg::Image<pType>& filter, float D0, int type=1, float gammaL=0.0,
  float gammaH=1.0 );

// build an ideal filter type > 0 for highpass and < 0 for lowpass
// D0    : cuttoff frequency (1pixel=1Hz)
// type  : high or low pass
// gammaL: lower horizontal asymptote
// gammaH: upper horizontal asymptote
template <class pType>
void idealfilter( jdg::Image<pType>& filter, float D0, int type=1, float gammaL=0.0,
  float gammaH=1.0 );

template <class pType>
void haarTransform( const jdg::Image<pType>& orig, jdg::Image<pType>& coef );
template <class pType>
void ihaarTransform( const jdg::Image<pType>& orig, jdg::Image<pType>& coef );

template <class pType>
void fft( Image<std::complex<pType> >& f, int val )
{
  // resize to a power of 2
  int height = std::pow(2, std::ceil(log(f.getHeight())/log(2)));
  int width = std::pow(2, std::ceil(log(f.getWidth())/log(2)));

  // pad the image with zeros
  if ( height != f.getHeight() || width != f.getWidth() )
    f.pad( width, height, NEAREST );

  // large enough to hold rows or columns
  float* ary_vals = new float[std::max(width,height)*2];

  // perform 1D fft on all rows
  for ( int row = height-1; row >= 0; --row )
  {
    // build a row array
    for ( int i = width-1; i >= 0; --i )
    {
      // build the array for a row
      ary_vals[2*i] = static_cast<float>(f(i,row).real());
      ary_vals[2*i+1] = static_cast<float>(f(i,row).imag());
      
      // multiply by -1^(x+y)
      if ( (i+row)%2 != 0 && val >= 0 ) // odd
      {
        ary_vals[2*i] *= -1;
        ary_vals[2*i+1] *= -1;
      }
    }

    // find the fft of the row
    numrec::fft( ary_vals - 1, width, val );

    // put value back into image and multiply by 1/height
    for ( int i = width-1; i >= 0; --i )
    {
      f(i,row) = std::complex<pType>(
        static_cast<pType>(ary_vals[2*i]),     // real part
        static_cast<pType>(ary_vals[2*i+1]));  // imaginary part
      
      if ( val > 0 )
        f(i,row) *= 1.0/(height*width);
    }
  }

  // perform 1D fft on all columns
  for ( int col = width-1; col >= 0; --col )
  {
    for ( int i = height-1; i >= 0; --i )
    {
      ary_vals[2*i] = static_cast<float>(f(col,i).real());
      ary_vals[2*i+1] = static_cast<float>(f(col,i).imag());
    }

    numrec::fft( ary_vals - 1, height, val );

    for ( int i = height-1; i >= 0; --i )
      f(col,i) = std::complex<pType>(
        static_cast<pType>(ary_vals[2*i]),
        static_cast<pType>(ary_vals[2*i+1]));
  }

  delete [] ary_vals;
}

template <class pType>
void convolve( Image<std::complex<pType> >& img,
  const Image<std::complex<pType> >& kernel, const PadWith pad )
{
  Image<std::complex<pType> > kern = kernel;
  
  int origW = img.getWidth(), origH = img.getHeight();
  int dims = 
    max( img.getWidth(), img.getHeight() ) +
    max( kern.getWidth(), kern.getHeight() );

  int shiftX = min(img.getWidth(), kernel.getWidth())/2;
  int shiftY = min(img.getHeight(), kernel.getHeight())/2;

  // pad images
  img.pad( dims, dims, pad, shiftX, shiftY );
  kern.pad( dims, dims );

  // fourier transform
  fft(img);
  fft(kern);

  // multiplication
  img *= kern;

  // invert fourier
  fft(img,-1);

  // normalize back to value (this was divided out twice in the fft)
  img *= img.getWidth() * img.getHeight();

  // unpad the image back to original size ZEROS because it's efficient
  img.pad( origW, origH, jdg::ZEROS, -2*shiftX, -2*shiftY );
}

template <class pType>
void convolve_spacial( Image<pType>& img,
  const Image<pType>& kernel, const PadWith=NEAREST )
{
  int width = img.getWidth(),
      height = img.getHeight(),
      kernW = kernel.getWidth(),
      kernH = kernel.getHeight();

  int kernW_h = kernW/2,
      kernH_h = kernH/2,
      realX, realY;

  Image<pType> retImg(width, height);

  for ( int x = 0; x < width; x++ )
  for ( int y = 0; y < height; y++ )
  {
    retImg(x,y) = 0;
    for ( int kernX = 0; kernX < kernW; kernX++ )
    for ( int kernY = 0; kernY < kernH; kernY++ )
    {
      realX = x-kernW_h+kernX;
      realY = y-kernH_h+kernY;
      if ( realX >= 0 && realY >= 0 && realX < width && realY < height )
        retImg(x,y) = retImg(x,y) +
          img( x-kernW_h+kernX, y-kernH_h+kernY ) *
          kernel( kernW-kernX-1, kernH-kernY-1 );
    }
  }

  img = retImg;
}

template <class pType>
void butterworth( jdg::Image<pType>& filter, float D0, float n,
  float gammaL, float gammaH )
{
  int width = filter.getWidth();
  int height = filter.getHeight();
  
  float startX = -(width-1) / 2.0,
        startY = -(height-1) / 2.0,
        stopX = -startX,
        stopY = -startY,
        D0_sqr = D0*D0,
        diff = gammaH-gammaL,
        yy;

  for ( float y = startY; y <= stopY; y+=1.0 )
  {
    yy = y*y;
    for ( float x = startX; x <= stopX; x+=1.0 )
      if ( x != 0 && y != 0 )
        filter(x-startX,y-startY) = gammaL + diff/(1+pow(D0_sqr/(x*x+yy),n));
      else if (n>0) // lowpass
        filter(x-startX,y-startY) = gammaH;
      else          // highpass
        filter(x-startX,y-startY) = gammaL;
  }
}

template <class pType>
void gaussian( jdg::Image<pType>& filter, float D0, int type, float gammaL,
  float gammaH )
{
  int width = filter.getWidth();
  int height = filter.getHeight();
  
  float startX = -(width-1) / 2.0,
        startY = -(height-1) / 2.0,
        stopX = -startX,
        stopY = -startY,
        D0_sqr2 = 2*D0*D0,
        diff = gammaH-gammaL,
        yy;

  for ( float y = startY; y <= stopY; y+=1.0 )
  {
    yy = y*y;
    for ( float x = startX; x <= stopX; x+=1.0 )
      if ( type >= 0 ) // highpass
        filter(x-startX,y-startY) = diff*(1-exp(-(x*x+yy)/D0_sqr2))+gammaL;
      else            // lowpass
        filter(x-startX,y-startY) = diff*exp(-(x*x+yy)/D0_sqr2)+gammaL;
  }
}

template <class pType>
void idealfilter( jdg::Image<pType>& filter, float D0, int type, float gammaL,
  float gammaH )
{
  int width = filter.getWidth();
  int height = filter.getHeight();
  
  float startX = -(width-1) / 2.0,
        startY = -(height-1) / 2.0,
        stopX = -startX,
        stopY = -startY,
        first = (type < 0 ? gammaH : gammaL),
        second = (type < 0 ? gammaL : gammaH),
        yy;


  for ( float y = startY; y <= stopY; y+=1.0 )
  {
    yy = y*y;
    for ( float x = startX; x <= stopX; x+=1.0 )
      if ( sqrt(x*x+y*y) <= D0 )
        filter(x-startX,y-startY) = first;
      else
        filter(x-startX,y-startY) = second;
      
  }
}

// Wavelet
template <class pType>
void haarTransform( const jdg::Image<pType>& orig, jdg::Image<pType>& coef )
{
  const double SIN30 = static_cast<pType>(0.707106781);

  // move up to powers of two
  int height = std::pow(2, std::ceil(log(orig.getHeight())/log(2)));
  int width = std::pow(2, std::ceil(log(orig.getWidth())/log(2)));

  int dim = std::max(height,width);

  coef = orig;
  
  if ( orig.getHeight() != dim || orig.getWidth() != dim )
    coef.pad(dim, dim);  // pad image with zeros to make power of 2
  
  jdg::Image<pType> temp = coef;

  while ( dim > 1 )
  {
    // all rows
    pType left, right;
    temp = coef;
    int halfs = dim / 2;
    for ( int row = 0; row < dim; row++ )
    {
      for ( int col = 0; col < dim; col+=2 )
      {
        left = temp(col,row);
        right = temp(col+1,row);
        coef(col/2,row) = SIN30*(left+right); // lowpass
        coef(col/2+halfs,row) = SIN30*(left-right); // highpass
      }
    }

    // all cols
    pType top, bottom;
    temp = coef;
    for ( int col = 0; col < dim; col++ )
      for ( int row = 0; row < dim; row+=2 )
      {
        top = temp(col,row);
        bottom = temp(col,row+1);
        coef(col,row/2) = SIN30*(top+bottom); // lowpass
        coef(col,row/2+halfs) = SIN30*(top-bottom); // highpass
      }

    dim /= 2;
  }
}

template <class pType>
void ihaarTransform( const jdg::Image<pType>& orig, jdg::Image<pType>& coef )
{
  const double SIN30 = static_cast<pType>(0.707106781);

  // move up to powers of two
  int height = std::pow(2, std::ceil(log(orig.getHeight())/log(2)));
  int width = std::pow(2, std::ceil(log(orig.getWidth())/log(2)));

  int dim=1, size = std::max(height,width);

  coef = orig;
  
  if ( orig.getHeight() != size || orig.getWidth() != size )
    coef.pad(dim, dim);  // pad image with zeros to make power of 2
  
  jdg::Image<pType> temp = coef;

  while ( dim < size )
  {
    int doubles = dim*2;
    
    // all cols
    pType top, bottom;
    temp = coef;
    for ( int col = 0; col < doubles; col++ )
      for ( int row = 0; row < doubles; row+=2 )
      {
        top = temp(col,row/2);
        bottom = temp(col,row/2+dim);
        coef(col,row) = SIN30*(top+bottom); // lowpass
        coef(col,row+1) = SIN30*(top-bottom); // highpass
      }
    
    // all rows
    pType left, right;
    temp = coef;
    for ( int row = 0; row < doubles; row++ )
    {
      for ( int col = 0; col < doubles; col+=2 )
      {
        left = temp(col/2,row);
        right = temp(col/2+dim,row);
        coef(col,row) = SIN30*(left+right); // lowpass
        coef(col+1,row) = SIN30*(left-right); // highpass
      }
    }

    dim *= 2;
  }

}

}
#endif
