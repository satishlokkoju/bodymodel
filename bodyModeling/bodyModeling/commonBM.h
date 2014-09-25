#ifndef _COMMON_H_
#define _COMMON_H_

#include <opencv.hpp>
#include <vector>
#include <string>
#include <iostream>

#include <exception>
////////////////////////////////////////////////////////////////////
// Standard includes:
#ifdef __APPLE__
	#include <GLUT/glut.h>

#else
	#include <windows.h>
	#include <GL/gl.h>
	//#include <GL/gl2.h>
	#include <GL/glut.h>
	#include <GL/glext.h>
#endif

using namespace std;
using namespace cv;

#ifdef SATISH

#endif

#ifdef __cplusplus
extern "C"
{
#endif

typedef void (*Funcion)();

typedef struct triangle
{
	Point3f VertA;
	Point3f VertB;
	Point3f VertC;
} Triangle;

float AbsValue(Point3d pt);


class Options 
{
  public:
    /// Constructor
    Options();
	bool mNoNormals;
	bool mNoTexCoords;
};

class ctm_error : public std::exception
{
private:
	int mErrorCode;

public:
	explicit				ctm_error(int aError)	
	{ 
		mErrorCode = aError; 
	}

	virtual const char*		what() const throw()	
	{ 
		return "Body Modeling code ! "; 
	}

	int error_code () const throw()		
	{ 
		return mErrorCode; 
	}
};
#ifdef __cplusplus
}
#endif
#endif