#ifndef JDG_IMAGE_CLASS
#define JDG_IMAGE_CLASS

#include <complex>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <cv.h>
#include <highgui.h>

namespace std
{
  // overloaded min/max functions for complex
  template <class pType>
  complex<pType>& max( complex<pType>& lhs, complex<pType>& rhs )
  {
    return ( abs(lhs) < abs(rhs) ? rhs : lhs );
  }

  template <class pType>
  complex<pType>& min( complex<pType>& lhs, complex<pType>& rhs )
  {
    return ( abs(lhs) < abs(rhs) ? lhs : rhs );
  }
} // namespace std

namespace jdg
{

enum PadWith { ZEROS=0, NEAREST=1, WRAP=2, MIRROR=3 };

// L1 makes all sum to param1 in normalize function
enum NormType { MINMAX=0, MINMAX_LOG=1, L1=2 };

template <class pType>
class Image
{
  public:
    
    ////////////////////////////////////////////////////////////////////////////
    // constructors/destructor                                                //
    ////////////////////////////////////////////////////////////////////////////

    ~Image();
    Image();
    Image(const Image<pType>& rhs);
    Image(const std::string& filename);
    Image(int width, int height);

    template <class pType2>
    Image(const Image<pType2>& rhs);
    
    template <class pType2>
    Image(const Image<std::complex<pType2> >& rhs);
    
    ////////////////////////////////////////////////////////////////////////////
    // other methods                                                          //
    ////////////////////////////////////////////////////////////////////////////

    // execute a function on all pixels
    template <class A>
    void callFunc( void (*func)(A) );

    template <class A, class B>
    void callFunc( A (*func)(B) );
    
    // execute a function on all pixels with x/y location passed as well
    template <class A>
    void callFunc( void (*func)(A, int, int) );
    
    template <class A, class B>
    void callFunc( A (*func)(B, int, int) );

    // normalize the image
    void normalize( const NormType type, pType param1, pType param2=255.0 );

    // pType must be primitive for load/save to work
    void load(const std::string& filename);
    void save(const std::string& filename) const;

    // display the image in a highgui window and pause for input
    void show(const std::string& window = "image") const;

    // re-allocate memory for the image and change _width and _height
    inline void resizeCanvas(int width, int height);

    // pad the image
    void pad( int width, int height, const PadWith padding = ZEROS,
      int xoffset=0, int yoffset=0 );

    pType min() const;
    pType max() const;

    ////////////////////////////////////////////////////////////////////////////
    // accessor functions                                                     //
    ////////////////////////////////////////////////////////////////////////////
    
    int getWidth() const { return _width; }
    int getHeight() const { return _height; }
    const pType* getData() const { return _data; }

    ////////////////////////////////////////////////////////////////////////////
    // operator overloads                                                     //
    ////////////////////////////////////////////////////////////////////////////

    // accessors to pixels
    inline pType& operator()(int x, int y);
    inline pType operator()(int x, int y, const PadWith padding = ZEROS) const;
   
    // assignment opperators
    Image<pType>& operator=(const Image<pType>& rhs);

    template <class pType2>
    Image<pType>& operator=(const Image<pType2>& rhs);
    
    template <class pType2>
    Image<pType>& operator=(const Image<std::complex<pType2> >& rhs);

    // equality operators
    bool operator==(const Image<pType>& rhs) const;
    bool operator!=(const Image<pType>& rhs) const { return !((*this)==rhs); }
    
    template <class pType2> 
    bool operator==(const Image<pType2>& rhs) const;
    template <class pType2>
    bool operator!=(const Image<pType2>& rhs) const { return !((*this)==rhs); }
    
    template <class pType2>
    bool operator==(const Image<std::complex<pType2> >& rhs) const;
    
    // arithmetic operators
    Image<pType> operator+ (const pType& rhs) const;
    Image<pType> operator- (const pType& rhs) const;
    Image<pType> operator* (const pType& rhs) const;
    Image<pType> operator/ (const pType& rhs) const;
    Image<pType> operator+ (const Image<pType>& rhs) const;
    Image<pType> operator- (const Image<pType>& rhs) const;
    Image<pType> operator* (const Image<pType>& rhs) const;
    Image<pType> operator/ (const Image<pType>& rhs) const;
    
    template<class pType2>
    Image<pType> operator+ (const pType2& rhs) const;
    template<class pType2>
    Image<pType> operator- (const pType2& rhs) const;
    template<class pType2>
    Image<pType> operator* (const pType2& rhs) const;
    template<class pType2>
    Image<pType> operator/ (const pType2& rhs) const;
    template<class pType2>
    Image<pType> operator+ (const Image<pType2>& rhs) const;
    template<class pType2>
    Image<pType> operator- (const Image<pType2>& rhs) const;
    template<class pType2>
    Image<pType> operator* (const Image<pType2>& rhs) const;
    template<class pType2>
    Image<pType> operator/ (const Image<pType2>& rhs) const;
    
    template<class pType2>
    Image<pType> operator+ (const std::complex<pType2> & rhs) const;
    template<class pType2>
    Image<pType> operator- (const std::complex<pType2> & rhs) const;
    template<class pType2>
    Image<pType> operator* (const std::complex<pType2> & rhs) const;
    template<class pType2>
    Image<pType> operator/ (const std::complex<pType2> & rhs) const;
    template<class pType2>
    Image<pType> operator+ (const Image<std::complex<pType2> >& rhs) const;
    template<class pType2>
    Image<pType> operator- (const Image<std::complex<pType2> >& rhs) const;
    template<class pType2>
    Image<pType> operator* (const Image<std::complex<pType2> >& rhs) const;
    template<class pType2>
    Image<pType> operator/ (const Image<std::complex<pType2> >& rhs) const;

    // compound operators
    Image<pType>& operator+=(const pType& rhs);
    Image<pType>& operator-=(const pType& rhs);
    Image<pType>& operator*=(const pType& rhs);
    Image<pType>& operator/=(const pType& rhs);
    Image<pType>& operator+=(const Image<pType>& rhs);
    Image<pType>& operator-=(const Image<pType>& rhs);
    Image<pType>& operator*=(const Image<pType>& rhs);
    Image<pType>& operator/=(const Image<pType>& rhs);

    template<class pType2>
    Image<pType>& operator+=(const pType2& rhs);
    template<class pType2>
    Image<pType>& operator-=(const pType2& rhs);
    template<class pType2>
    Image<pType>& operator*=(const pType2& rhs);
    template<class pType2>
    Image<pType>& operator/=(const pType2& rhs);
    template<class pType2>
    Image<pType>& operator+=(const Image<pType2>& rhs);
    template<class pType2>
    Image<pType>& operator-=(const Image<pType2>& rhs);
    template<class pType2>
    Image<pType>& operator*=(const Image<pType2>& rhs);
    template<class pType2>
    Image<pType>& operator/=(const Image<pType2>& rhs);

    template<class pType2>
    Image<pType>& operator+=(const std::complex<pType2> & rhs);
    template<class pType2>
    Image<pType>& operator-=(const std::complex<pType2> & rhs);
    template<class pType2>
    Image<pType>& operator*=(const std::complex<pType2> & rhs);
    template<class pType2>
    Image<pType>& operator/=(const std::complex<pType2> & rhs);
    template<class pType2>
    Image<pType>& operator+=(const Image<std::complex<pType2> >& rhs);
    template<class pType2>
    Image<pType>& operator-=(const Image<std::complex<pType2> >& rhs);
    template<class pType2>
    Image<pType>& operator*=(const Image<std::complex<pType2> >& rhs);
    template<class pType2>
    Image<pType>& operator/=(const Image<std::complex<pType2> >& rhs);

    ////////////////////////////////////////////////////////////////////////////
    // private data members                                                   //
    ////////////////////////////////////////////////////////////////////////////

  protected:
    int _width;
    int _height;
    pType* _data;
};

template <class pType>
template <class A>
void Image<pType>::callFunc( void (*func)(A) )
{
  for ( int i = 0; i < _height; i++ )
    for ( int j = 0; j < _width; j++ )
      func((*this)(j,i));
}

template <class pType>
template <class A, class B>
void Image<pType>::callFunc( A (*func)(B) )
{
  for ( int i = 0; i < _height; i++ )
    for ( int j = 0; j < _width; j++ )
      (*this)(j,i) = func((*this)(j,i));
}

template <class pType>
template <class A>
void Image<pType>::callFunc( void (*func)(A, int, int) )
{
  for ( int i = 0; i < _height; i++ )
    for ( int j = 0; j < _width; j++ )
      func((*this)(j,i), j, i);
}

template <class pType>
template <class A, class B>
void Image<pType>::callFunc( A (*func)(B, int, int) )
{
  for ( int i = 0; i < _height; i++ )
    for ( int j = 0; j < _width; j++ )
      (*this)(j,i) = func((*this)(j,i), j, i);
}

template <class pType>
pType Image<pType>::min() const
{
  int len = _width * _height;

  pType* data = _data;
  pType val = *_data;

  for ( int i = 1; i < len; i++ )
  {
    if (std::min(*data, val)==*data)
      val = *data;
    data++;
  }
  return val;
}

template <class pType>
pType Image<pType>::max() const
{
  int len = _width * _height;

  pType* data = _data;
  pType val = *_data;

  for ( int i = 1; i < len; i++ )
  {
    if (std::max(*data,val)==*data)
      val = *data;
    data++;
  }
  return val;
}

template <class pType>
void Image<pType>::normalize( const NormType type, pType param1, pType param2 )
{
  if ( !_data ) return;

  if ( type == MINMAX )
  {
    pType min, max;
    min = max = *_data;

    for ( int i = _width * _height - 1; i >= 1; --i )
    {
      min = std::min(_data[i],min);
      max = std::max(_data[i],max);
    }

    pType factor = max-min;
    pType diff = param2-param1;

    for ( int i = _width * _height - 1; i >= 0; --i )
      _data[i] = ((_data[i]-min)*diff)/factor+param1;
  }
  else if ( type == MINMAX_LOG )
  {
    pType min, max;
    *_data = std::log(std::abs(*_data) + 1);
    min = max = *_data;

    for ( int i = _width * _height - 1; i >= 1; --i )
    {
      _data[i] = std::log(std::abs(_data[i]) + 1);
      min = std::min(_data[i],min);
      max = std::max(_data[i],max);
    }
    
    pType factor = max-min;
    pType diff = param2-param1;

    for ( int i = _width * _height - 1; i >= 0; --i )
      _data[i] = ((_data[i] - min)*diff)/factor+param1;
  }
  else if ( type == L1 )
  {
    // get sum of everything
    pType sum=_data[0];
    for ( int i = _width * _height - 1; i >= 1; --i )
      sum += _data[i];
    if ( abs(sum) != 0 )
    {
      sum = pow(sum,-1) * param1;
      for ( int i = _width * _height - 1; i >= 0; --i )
        _data[i] *= sum;
    }
  }
}

template <class pType>
void Image<pType>::pad( int width, int height, const PadWith padding,
  int xoffset, int yoffset)
{
  Image<pType> ret(width, height);

  for ( int i = 0; i < height; i++ )
    for ( int j = 0; j < width; j++ )
      ret(j, i) = (*this)(j-xoffset, i-yoffset, padding);

  *this = ret;
}

// display image in grayscale
template <class pType>
void Image<pType>::show(const std::string& window) const
{
  IplImage* img = cvCreateImage( cvSize( _width, _height ), IPL_DEPTH_8U, 1 );

  // create new IplImage
  for ( int i = 0; i < _height;i++ )
    for ( int j = 0; j < _width; j++ )
      cvSetReal2D(img,i,j,static_cast<double>(_data[j+i*_width]));

  cvNamedWindow(window.c_str());
  cvShowImage(window.c_str(),img);
  cvWaitKey();
  cvDestroyWindow(window.c_str());
  cvReleaseImage(&img);
}

template <class pType>
inline void Image<pType>::resizeCanvas(int width, int height)
{
  if ( _data )
    delete [] _data;
  _width = width;
  _height = height;
  _data = new pType[_width*_height];
}

template <class pType>
template <class pType2>
Image<pType> Image<pType>::operator+ (const std::complex<pType2> & rhs) const
{
  Image<pType> ret(_width, _height);

  for ( int i = 0; i < _height; i++ )
    for ( int j = 0; j < _width; j++ )
      ret(j, i) = (*this)(j,i) + static_cast<pType>(abs(rhs));

  return ret;
}

template <class pType>
template <class pType2>
Image<pType> Image<pType>::operator- (const std::complex<pType2> & rhs) const
{
  Image<pType> ret(_width, _height);

  for ( int i = 0; i < _height; i++ )
    for ( int j = 0; j < _width; j++ )
      ret(j, i) = (*this)(j,i) - static_cast<pType>(abs(rhs));

  return ret;
}

template <class pType>
template <class pType2>
Image<pType> Image<pType>::operator* (const std::complex<pType2> & rhs) const
{
  Image<pType> ret(_width, _height);

  for ( int i = 0; i < _height; i++ )
    for ( int j = 0; j < _width; j++ )
      ret(j, i) = (*this)(j,i) * static_cast<pType>(abs(rhs));

  return ret;
}

template <class pType>
template <class pType2>
Image<pType> Image<pType>::operator/ (const std::complex<pType2> & rhs) const
{
  Image<pType> ret(_width, _height);

  for ( int i = 0; i < _height; i++ )
    for ( int j = 0; j < _width; j++ )
      ret(j, i) = (*this)(j,i) / static_cast<pType>(abs(rhs));

  return ret;
}

template <class pType>
template <class pType2>
Image<pType> Image<pType>::operator+ (const Image<std::complex<pType2> >& rhs)
  const
{
  int rhs_height = rhs.getHeight(),
      rhs_width = rhs.getWidth();

  // max
  int height = std::max(_height,rhs_height),
      width = std::max(_width, rhs_width);

  Image<pType> ret(width, height);

  for ( int i = 0; i < height; i++ )
    for ( int j = 0; j < width; j++ )
      ret(j, i) = (*this)(j,i) + static_cast<pType>(abs(rhs(j,i)));

  return ret;
}

template <class pType>
template <class pType2>
Image<pType> Image<pType>::operator- (const Image<std::complex<pType2> >& rhs)
  const
{
  int rhs_height = rhs.getHeight(),
      rhs_width = rhs.getWidth();

  // max
  int height = std::max(_height,rhs_height),
      width = std::max(_width, rhs_width);

  Image<pType> ret(width, height);

  for ( int i = 0; i < height; i++ )
    for ( int j = 0; j < width; j++ )
      ret(j, i) = (*this)(j,i) - static_cast<pType>(abs(rhs(j,i)));

  return ret;
}

template <class pType>
template <class pType2>
Image<pType> Image<pType>::operator* (const Image<std::complex<pType2> >& rhs)
  const
{
  int rhs_height = rhs.getHeight(),
      rhs_width = rhs.getWidth();

  // max
  int height = std::max(_height,rhs_height),
      width = std::max(_width, rhs_width);

  Image<pType> ret(width, height);

  for ( int i = 0; i < height; i++ )
    for ( int j = 0; j < width; j++ )
      ret(j, i) = (*this)(j,i) * static_cast<pType>(abs(rhs(j,i)));

  return ret;
}

template <class pType>
template <class pType2>
Image<pType> Image<pType>::operator/ (const Image<std::complex<pType2> >& rhs)
  const
{
  int rhs_height = rhs.getHeight(),
      rhs_width = rhs.getWidth();

  // max
  int height = std::max(_height,rhs_height),
      width = std::max(_width, rhs_width);

  Image<pType> ret(width, height);

  for ( int i = 0; i < height; i++ )
    for ( int j = 0; j < width; j++ )
      ret(j, i) = (*this)(j,i) / static_cast<pType>(abs(rhs(j,i)));

  return ret;
}

template <class pType>
template <class pType2>
Image<pType>& Image<pType>::operator+=(const std::complex<pType2> & rhs)
{
  for( int i = 0; i < _height; i++ )
    for ( int j = 0; j < _width; j++ )
      (*this)(j, i) += static_cast<pType>(abs(rhs));

  return *this;
}

template <class pType>
template <class pType2>
Image<pType>& Image<pType>::operator-=(const std::complex<pType2> & rhs)
{
  for( int i = 0; i < _height; i++ )
    for ( int j = 0; j < _width; j++ )
      (*this)(j, i) -= static_cast<pType>(abs(rhs));

  return *this;
}

template <class pType>
template <class pType2>
Image<pType>& Image<pType>::operator*=(const std::complex<pType2> & rhs)
{
  for( int i = 0; i < _height; i++ )
    for ( int j = 0; j < _width; j++ )
      (*this)(j, i) *= static_cast<pType>(abs(rhs));

  return *this;
}

template <class pType>
template <class pType2>
Image<pType>& Image<pType>::operator/=(const std::complex<pType2> & rhs)
{
  for( int i = 0; i < _height; i++ )
    for ( int j = 0; j < _width; j++ )
      (*this)(j, i) /= static_cast<pType>(abs(rhs));

  return *this;
}

template <class pType>
template <class pType2>
Image<pType>& Image<pType>::operator+=(const Image<std::complex<pType2> >& rhs)
{
  int rhs_height = rhs.getHeight(),
      rhs_width = rhs.getWidth();

  // max
  int height = std::max(_height, rhs_height),
      width = std::max(_width, rhs_width);

  for( int i = 0; i < height; i++ )
    for ( int j = 0; j < width; j++ )
      (*this)(j, i) += static_cast<pType>(abs(rhs(j,i)));

  return *this;
}

template <class pType>
template <class pType2>
Image<pType>& Image<pType>::operator-=(const Image<std::complex<pType2> >& rhs)
{
  int rhs_height = rhs.getHeight(),
      rhs_width = rhs.getWidth();

  // max
  int height = std::max(_height, rhs_height),
      width = std::max(_width, rhs_width);

  for( int i = 0; i < height; i++ )
    for ( int j = 0; j < width; j++ )
      (*this)(j, i) -= static_cast<pType>(abs(rhs(j,i)));

  return *this;
}

template <class pType>
template <class pType2>
Image<pType>& Image<pType>::operator*=(const Image<std::complex<pType2> >& rhs)
{
  int rhs_height = rhs.getHeight(),
      rhs_width = rhs.getWidth();

  // max
  int height = std::max(_height, rhs_height),
      width = std::max(_width, rhs_width);

  for( int i = 0; i < height; i++ )
    for ( int j = 0; j < width; j++ )
      (*this)(j, i) *= static_cast<pType>(abs(rhs(j,i)));

  return *this;
}

template <class pType>
template <class pType2>
Image<pType>& Image<pType>::operator/=(const Image<std::complex<pType2> >& rhs)
{
  int rhs_height = rhs.getHeight(),
      rhs_width = rhs.getWidth();

  // max
  int height = std::max(_height, rhs_height),
      width = std::max(_width, rhs_width);

  for( int i = 0; i < height; i++ )
    for ( int j = 0; j < width; j++ )
      (*this)(j, i) /= static_cast<pType>(abs(rhs(j,i)));

  return *this;
}

template <class pType>
template <class pType2>
Image<pType> Image<pType>::operator+ (const pType2& rhs) const
{
  Image<pType> ret(_width, _height);

  for ( int i = 0; i < _height; i++ )
    for ( int j = 0; j < _width; j++ )
      ret(j, i) = (*this)(j,i) + static_cast<pType>(rhs);

  return ret;
}

template <class pType>
template <class pType2>
Image<pType> Image<pType>::operator- (const pType2& rhs) const
{
  Image<pType> ret(_width, _height);

  for ( int i = 0; i < _height; i++ )
    for ( int j = 0; j < _width; j++ )
      ret(j, i) = (*this)(j,i) - static_cast<pType>(rhs);

  return ret;
}

template <class pType>
template <class pType2>
Image<pType> Image<pType>::operator* (const pType2& rhs) const
{
  Image<pType> ret(_width, _height);

  for ( int i = 0; i < _height; i++ )
    for ( int j = 0; j < _width; j++ )
      ret(j, i) = (*this)(j,i) * static_cast<pType>(rhs);

  return ret;
}

template <class pType>
template <class pType2>
Image<pType> Image<pType>::operator/ (const pType2& rhs) const
{
  Image<pType> ret(_width, _height);

  for ( int i = 0; i < _height; i++ )
    for ( int j = 0; j < _width; j++ )
      ret(j, i) = (*this)(j,i) / static_cast<pType>(rhs);

  return ret;
}

template <class pType>
template <class pType2>
Image<pType> Image<pType>::operator+ (const Image<pType2>& rhs) const
{
  int rhs_height = rhs.getHeight(),
      rhs_width = rhs.getWidth();

  // max
  int height = std::max(_height,rhs_height),
      width = std::max(_width, rhs_width);

  Image<pType> ret(width, height);

  for ( int i = 0; i < height; i++ )
    for ( int j = 0; j < width; j++ )
      ret(j, i) = (*this)(j,i) + static_cast<pType>(rhs(j,i));

  return ret;
}

template <class pType>
template <class pType2>
Image<pType> Image<pType>::operator- (const Image<pType2>& rhs) const
{
  int rhs_height = rhs.getHeight(),
      rhs_width = rhs.getWidth();

  // max
  int height = std::max(_height,rhs_height),
      width = std::max(_width, rhs_width);

  Image<pType> ret(width, height);

  for ( int i = 0; i < height; i++ )
    for ( int j = 0; j < width; j++ )
      ret(j, i) = (*this)(j,i) - static_cast<pType>(rhs(j,i));

  return ret;
}

template <class pType>
template <class pType2>
Image<pType> Image<pType>::operator* (const Image<pType2>& rhs) const
{
  int rhs_height = rhs.getHeight(),
      rhs_width = rhs.getWidth();

  // max
  int height = std::max(_height,rhs_height),
      width = std::max(_width, rhs_width);

  Image<pType> ret(width, height);

  for ( int i = 0; i < height; i++ )
    for ( int j = 0; j < width; j++ )
      ret(j, i) = (*this)(j,i) * static_cast<pType>(rhs(j,i));

  return ret;
}

template <class pType>
template <class pType2>
Image<pType> Image<pType>::operator/ (const Image<pType2>& rhs) const
{
  int rhs_height = rhs.getHeight(),
      rhs_width = rhs.getWidth();

  // max
  int height = std::max(_height,rhs_height),
      width = std::max(_width, rhs_width);

  Image<pType> ret(width, height);

  for ( int i = 0; i < height; i++ )
    for ( int j = 0; j < width; j++ )
      ret(j, i) = (*this)(j,i) / static_cast<pType>(rhs(j,i));

  return ret;
}

template <class pType>
template <class pType2>
Image<pType>& Image<pType>::operator+=(const pType2& rhs)
{
  for( int i = 0; i < _height; i++ )
    for ( int j = 0; j < _width; j++ )
      (*this)(j, i) += static_cast<pType>(rhs);

  return *this;
}

template <class pType>
template <class pType2>
Image<pType>& Image<pType>::operator-=(const pType2& rhs)
{
  for( int i = 0; i < _height; i++ )
    for ( int j = 0; j < _width; j++ )
      (*this)(j, i) -= static_cast<pType>(rhs);

  return *this;
}

template <class pType>
template <class pType2>
Image<pType>& Image<pType>::operator*=(const pType2& rhs)
{
  for( int i = 0; i < _height; i++ )
    for ( int j = 0; j < _width; j++ )
      (*this)(j, i) *= static_cast<pType>(rhs);

  return *this;
}

template <class pType>
template <class pType2>
Image<pType>& Image<pType>::operator/=(const pType2& rhs)
{
  for( int i = 0; i < _height; i++ )
    for ( int j = 0; j < _width; j++ )
      (*this)(j, i) /= static_cast<pType>(rhs);

  return *this;
}

template <class pType>
template <class pType2>
Image<pType>& Image<pType>::operator+=(const Image<pType2>& rhs)
{
  int rhs_height = rhs.getHeight(),
      rhs_width = rhs.getWidth();

  // max
  int height = std::max(_height, rhs_height),
      width = std::max(_width, rhs_width);

  for( int i = 0; i < height; i++ )
    for ( int j = 0; j < width; j++ )
      (*this)(j, i) += static_cast<pType>(rhs(j,i));

  return *this;
}

template <class pType>
template <class pType2>
Image<pType>& Image<pType>::operator-=(const Image<pType2>& rhs)
{
  int rhs_height = rhs.getHeight(),
      rhs_width = rhs.getWidth();

  // max
  int height = std::max(_height, rhs_height),
      width = std::max(_width, rhs_width);

  for( int i = 0; i < height; i++ )
    for ( int j = 0; j < width; j++ )
      (*this)(j, i) -= static_cast<pType>(rhs(j,i));

  return *this;
}

template <class pType>
template <class pType2>
Image<pType>& Image<pType>::operator*=(const Image<pType2>& rhs)
{
  int rhs_height = rhs.getHeight(),
      rhs_width = rhs.getWidth();

  // max
  int height = std::max(_height, rhs_height),
      width = std::max(_width, rhs_width);

  for( int i = 0; i < height; i++ )
    for ( int j = 0; j < width; j++ )
      (*this)(j, i) *= static_cast<pType>(rhs(j,i));

  return *this;
}

template <class pType>
template <class pType2>
Image<pType>& Image<pType>::operator/=(const Image<pType2>& rhs)
{
  int rhs_height = rhs.getHeight(),
      rhs_width = rhs.getWidth();

  // max
  int height = std::max(_height, rhs_height),
      width = std::max(_width, rhs_width);

  for( int i = 0; i < height; i++ )
    for ( int j = 0; j < width; j++ )
      (*this)(j, i) /= static_cast<pType>(rhs(j,i));

  return *this;
}

template <class pType>
Image<pType> Image<pType>::operator+ (const pType& rhs) const
{
  Image<pType> ret(_width, _height);

  for ( int i = 0; i < _height; i++ )
    for ( int j = 0; j < _width; j++ )
      ret(j, i) = (*this)(j,i) + rhs;

  return ret;
}

template <class pType>
Image<pType> Image<pType>::operator- (const pType& rhs) const
{
  Image<pType> ret(_width, _height);

  for ( int i = 0; i < _height; i++ )
    for ( int j = 0; j < _width; j++ )
      ret(j, i) = (*this)(j,i) - rhs;

  return ret;
}

template <class pType>
Image<pType> Image<pType>::operator* (const pType& rhs) const
{
  Image<pType> ret(_width, _height);

  for ( int i = 0; i < _height; i++ )
    for ( int j = 0; j < _width; j++ )
      ret(j, i) = (*this)(j,i) * rhs;

  return ret;
}

template <class pType>
Image<pType> Image<pType>::operator/ (const pType& rhs) const
{
  Image<pType> ret(_width, _height);

  for ( int i = 0; i < _height; i++ )
    for ( int j = 0; j < _width; j++ )
      ret(j, i) = (*this)(j,i) / rhs;

  return ret;
}

template <class pType>
Image<pType> Image<pType>::operator+ (const Image<pType>& rhs) const
{
  int rhs_height = rhs.getHeight(),
      rhs_width = rhs.getWidth();

  // max
  int height = std::max(_height,rhs_height),
      width = std::max(_width, rhs_width);

  Image<pType> ret(width, height);

  for ( int i = 0; i < height; i++ )
    for ( int j = 0; j < width; j++ )
      ret(j, i) = (*this)(j,i) + rhs(j,i);

  return ret;
}

template <class pType>
Image<pType> Image<pType>::operator- (const Image<pType>& rhs) const
{
  int rhs_height = rhs.getHeight(),
      rhs_width = rhs.getWidth();

  // max
  int height = std::max(_height,rhs_height),
      width = std::max(_width, rhs_width);

  Image<pType> ret(width, height);

  for ( int i = 0; i < height; i++ )
    for ( int j = 0; j < width; j++ )
      ret(j, i) = (*this)(j,i) - rhs(j,i);

  return ret;
}

template <class pType>
Image<pType> Image<pType>::operator* (const Image<pType>& rhs) const
{
  int rhs_height = rhs.getHeight(),
      rhs_width = rhs.getWidth();

  // max
  int height = std::max(_height,rhs_height),
      width = std::max(_width, rhs_width);

  Image<pType> ret(width, height);

  for ( int i = 0; i < height; i++ )
    for ( int j = 0; j < width; j++ )
      ret(j, i) = (*this)(j,i) * rhs(j,i);

  return ret;
}

template <class pType>
Image<pType> Image<pType>::operator/ (const Image<pType>& rhs) const
{
  int rhs_height = rhs.getHeight(),
      rhs_width = rhs.getWidth();

  // max
  int height = std::max(_height,rhs_height),
      width = std::max(_width, rhs_width);

  Image<pType> ret(width, height);

  for ( int i = 0; i < height; i++ )
    for ( int j = 0; j < width; j++ )
      ret(j, i) = (*this)(j,i) / rhs(j,i);

  return ret;
}

template <class pType>
Image<pType>& Image<pType>::operator+=(const pType& rhs)
{
  for( int i = 0; i < _height; i++ )
    for ( int j = 0; j < _width; j++ )
      (*this)(j, i) += rhs;

  return *this;
}

template <class pType>
Image<pType>& Image<pType>::operator-=(const pType& rhs)
{
  for( int i = 0; i < _height; i++ )
    for ( int j = 0; j < _width; j++ )
      (*this)(j, i) -= rhs;

  return *this;
}

template <class pType>
Image<pType>& Image<pType>::operator*=(const pType& rhs)
{
  for( int i = 0; i < _height; i++ )
    for ( int j = 0; j < _width; j++ )
      (*this)(j, i) *= rhs;

  return *this;
}

template <class pType>
Image<pType>& Image<pType>::operator/=(const pType& rhs)
{
  for( int i = 0; i < _height; i++ )
    for ( int j = 0; j < _width; j++ )
      (*this)(j, i) /= rhs;

  return *this;
}

template <class pType>
Image<pType>& Image<pType>::operator+=(const Image<pType>& rhs)
{
  int rhs_height = rhs.getHeight(),
      rhs_width = rhs.getWidth();

  // max
  int height = std::max(_height, rhs_height),
      width = std::max(_width, rhs_width);

  for( int i = 0; i < height; i++ )
    for ( int j = 0; j < width; j++ )
      (*this)(j, i) += rhs(j,i);

  return *this;
}

template <class pType>
Image<pType>& Image<pType>::operator-=(const Image<pType>& rhs)
{
  int rhs_height = rhs.getHeight(),
      rhs_width = rhs.getWidth();

  // max
  int height = std::max(_height, rhs_height),
      width = std::max(_width, rhs_width);

  for( int i = 0; i < height; i++ )
    for ( int j = 0; j < width; j++ )
      (*this)(j, i) -= rhs(j,i);

  return *this;
}

template <class pType>
Image<pType>& Image<pType>::operator*=(const Image<pType>& rhs)
{
  int rhs_height = rhs.getHeight(),
      rhs_width = rhs.getWidth();

  // max
  int height = std::max(_height, rhs_height),
      width = std::max(_width, rhs_width);

  for( int i = 0; i < height; i++ )
    for ( int j = 0; j < width; j++ )
      (*this)(j, i) *= rhs(j,i);

  return *this;
}

template <class pType>
Image<pType>& Image<pType>::operator/=(const Image<pType>& rhs)
{
  int rhs_height = rhs.getHeight(),
      rhs_width = rhs.getWidth();

  // max
  int height = std::max(_height, rhs_height),
      width = std::max(_width, rhs_width);

  for( int i = 0; i < height; i++ )
    for ( int j = 0; j < width; j++ )
      (*this)(j, i) /= rhs(j,i);

  return *this;
}

template <class pType>
bool Image<pType>::operator==(const Image<pType>& rhs) const
{
  if ( rhs.getHeight() == _height && rhs.getWidth() == _width )
  {
    const pType* data = rhs.getData();
    for ( int i = _height*_width-1; i >= 0; --i )
      if (_data[i] != data[i])
        return false;
    return true;
  }
  return false;
}

template <class pType>
template <class pType2>
bool Image<pType>::operator==(const Image<pType2>& rhs) const
{
  if ( rhs.getHeight() == _height && rhs.getWidth() == _width )
  {
    const pType2* data = rhs.getData();
    for ( int i = _height*_width-1; i >= 0; --i )
      if (_data[i] != static_cast<pType>(data[i]))
        return false;
    return true;
  }
  return false;
}

template <class pType>
template <class pType2>
bool Image<pType>::operator==(const Image<std::complex<pType2> >& rhs) const
{
  if ( rhs.getHeight() == _height && rhs.getWidth() == _width )
  {
    const std::complex<pType2>* data = rhs.getData();
    for ( int i = _height*_width-1; i >= 0; --i )
      if (_data[i] != static_cast<pType>(abs(data[i])))
        return false;
    return true;
  }
  return false;
}

template <class pType>
template <class pType2>
Image<pType>& Image<pType>::operator=(const Image<std::complex<pType2> >& rhs)
{
  if ( rhs.getHeight() != _height && rhs.getWidth() != _width )
    resizeCanvas(rhs.getWidth(), rhs.getHeight());

  const std::complex<pType2>* data = rhs.getData();

  for ( int i = _height*_width-1; i >= 0; --i )
    _data[i] = static_cast<pType>(std::abs(data[i]));

  return *this;
}

template <class pType>
template <class pType2>
Image<pType>& Image<pType>::operator=(const Image<pType2>& rhs)
{
  if ( rhs.getHeight() != _height && rhs.getWidth() != _width )
    resizeCanvas(rhs.getWidth(), rhs.getHeight());

  const pType2* data = rhs.getData();

  for ( int i = _height*_width-1; i >= 0; --i )
    _data[i] = static_cast<pType>(data[i]);

  return *this;
}

template <class pType>
Image<pType>& Image<pType>::operator=(const Image<pType>& rhs)
{
  if ( rhs.getHeight() != _height && rhs.getWidth() != _width )
    resizeCanvas(rhs.getWidth(), rhs.getHeight());

  const pType* data = rhs.getData();
  for ( int i = _height*_width-1; i >= 0; --i )
    _data[i] = data[i];

  return *this;
}

template <class pType>
inline pType& Image<pType>::operator()(int x, int y)
{
  return _data[x+y*_width];
}

template <class pType>
inline pType Image<pType>::operator()(int x, int y, const PadWith padding) const
{
  if ( x >= _width || y >= _height || x < 0 || y < 0 )
  {
    if ( padding == ZEROS )
      return static_cast<pType>(0);
    else if ( padding == NEAREST )
    {
      x = std::max(0,x);
      y = std::max(0,y);
      x = std::min(_width-1,x);
      y = std::min(_height-1,y);
    }
    else if ( padding == WRAP )
    {
      // negative mods don't evaluate how i want, so i'll make them :)
      if ( x < 0 )
        x = (_width - (-x % _width)) + (-x / _width + 1) * _width;
      if ( y < 0 )
        y = (_height - (-y % _height)) + (-y / _height + 1) * _height;

      x %= _width;
      y %= _height;
    }
    else if ( padding == MIRROR )
    {
      // negative mods don't eval... blah blah ditto
      if ( x < 0 )
        x = (_width - (-x % _width)) + (-x / _width + 1) * _width;
      if ( y < 0 )
        y = (_height - (-y % _height)) + (-y / _height + 1) * _height;

      if ( x >= _width || x < 0 )
      {
        if ( (x / _width) % 2 != 0 )
          x = (_width-1) - (x % _width);
        else
          x %= _width;
      }
      if ( y >= _height || y < 0 )
      {
        if ( (y / _height) % 2 != 0 )
          y = (_height-1) - (y % _height);
        else
          y %= _height;
      }
    }
  }

  return _data[x+y*_width];
}

// constructors/destructor /////////////////////////////////////////////////////

template <class pType>
Image<pType>::Image(int width, int height) : _width(width), _height(height)
{
  _data = new pType[width*height];
}

template <class pType>
Image<pType>::Image() : _width(0), _height(0), _data(0)
{}

template <class pType>
Image<pType>::Image(const std::string& filename) : _width(0), _height(0),
  _data(0)
{
  load(filename);
}

template <class pType>
Image<pType>::Image(const Image<pType>& rhs)
{
  _width = rhs.getWidth();
  _height = rhs.getHeight();
  _data = new pType[_width*_height];

  const pType* data = rhs.getData();

  for ( int i = _width*_height-1; i>=0; --i )
    _data[i] = data[i];
}

template <class pType>
template <class pType2>
Image<pType>::Image(const Image<pType2>& rhs)
{
  _width = rhs.getWidth();
  _height = rhs.getHeight();
  _data = new pType[_width*_height];

  const pType2* data = rhs.getData();

  for ( int i = _width*_height-1; i>=0; --i )
    _data[i] = static_cast<pType>(data[i]);
}

template <class pType>
template <class pType2>
Image<pType>::Image(const Image<std::complex<pType2> >& rhs)
{
  _width = rhs.getWidth();
  _height = rhs.getHeight();
  _data = new pType[_width*_height];

  const std::complex<pType2>* data = rhs.getData();

  for ( int i = _width*_height-1; i>=0; --i )
    _data[i] = static_cast<pType>(std::abs(data[i]));
}

template <class pType>
Image<pType>::~Image()
{
  if ( _data )
    delete [] _data;
}

// file I/O functions //////////////////////////////////////////////////////////

template <class pType>
void jdg::Image<pType>::load(const std::string& filename)
{
  int i;
  int N, M, Q;
  unsigned char *charImage;
  char header [100], *ptr;
  std::ifstream ifp;

  ifp.open(filename.c_str(), std::ios::in | std::ios::binary);

  if (!ifp) {
    std::cout << "Can't read image: " << filename << std::endl;
    exit(1);
  }

  // read header

  ifp.getline(header,100,'\n');
  if ( (header[0]!=80) ||    // 'P'
      (header[1]!=53) ) {    // '5'
    std::cout << "Image " << filename << " is not PGM" << std::endl;
    exit(1);
  }

  ifp.getline(header,100,'\n');
  while(header[0]=='#')
    ifp.getline(header,100,'\n');

  M=strtol(header,&ptr,0);
  N=atoi(ptr);

  ifp.getline(header,100,'\n');
  Q=strtol(header,&ptr,0);

  charImage = (unsigned char *) new unsigned char [M*N];

  ifp.read( reinterpret_cast<char *>(charImage), (M*N)*sizeof(unsigned char));

  if (ifp.fail()) {
    std::cout << "Image " << filename << " has wrong size" << std::endl;
    exit(1);
  }

  ifp.close();

  //
  // Convert the unsigned characters to integers
  //

  _width = M;
  _height = N;
  
  resizeCanvas(M,N);

  for(i=M*N-1; i>=0; --i)
    _data[i] = charImage[i];

  delete [] charImage;
}

template <class pType>
void jdg::Image<pType>::save(const std::string& filename) const
{
  int i;
  unsigned char *charImage;
  std::ofstream ofp;

  charImage = new unsigned char[_width*_height];

  // convert the integer values to unsigned char

  for(i=_height*_width-1; i>=0; --i)
    charImage[i]=static_cast<unsigned char>(_data[i]);

  ofp.open(filename.c_str(), std::ios::out | std::ios::binary);

  if (!ofp) {
    std::cout << "Can't open file: " << filename << std::endl;
    exit(1);
  }

  ofp << "P5" << std::endl;
  ofp << _width << " " << _height << std::endl;
  ofp << 255 << std::endl;

  ofp.write(
    reinterpret_cast<char *>(charImage),
    (_width*_height)*sizeof(unsigned char));

  if (ofp.fail()) {
    std::cout << "Can't write image " << filename << std::endl;
    exit(0);
  }

  ofp.close();

  delete [] charImage;

}
} // namespace jdg

#endif // JDG_IMAGE_CLASS

