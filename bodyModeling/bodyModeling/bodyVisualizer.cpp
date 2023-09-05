/*---------------------------------------------------------*/
/*                    <copyright notice>                   */
/*                                                         */
/*	The Code is proprietary to Viewtek Solutions Pvt. Ltd. */
/*	   Mail to satish@viewtek.in for any clarifications.   */
/*                                                         */
/*			<Copyright (C) 2013 Satish Lakkoju.>           */
/*				   <All Rights Reserved>                   */
/*---------------------------------------------------------*/
#include <stdio.h>
#include "glviewer.h"
#include "common.h"
/* */

void draw(void* params)
{
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT); // Clear entire screen:
	glFlush();
}

/* */

//-----------------------------------------------------------------------------
// Bridge GLUT callback functions to class methods
//-----------------------------------------------------------------------------
// NOTE: This is just a hack to be able to reference the application class
// object from the GLUT callback functions, since there is no way (afaik) to
// pass user data (i.e. the object reference) through GLUT...
static GLViewer*	gGLViewer = NULL;

/// Redraw function.
void GLUTWindowRedraw(void)
{
	if (gGLViewer)
	{
		gGLViewer->WindowRedraw();
	}
}

/// Resize function.
void GLUTWindowResize(int w, int h)
{
	if (gGLViewer)
	{
		gGLViewer->WindowResize(w, h);
	}
}

/// Mouse click function
void GLUTMouseClick(int button, int state, int x, int y)
{
	if (gGLViewer)
	{
		gGLViewer->MouseClick(button, state, x, y);
	}
}

/// Mouse move function
void GLUTMouseMove(int x, int y)
{
	if (gGLViewer)
	{
		gGLViewer->MouseMove(x, y);
	}
}

/// Keyboard function
void GLUTKeyDown(unsigned char key, int x, int y)
{
	if (gGLViewer)
	{
		gGLViewer->KeyDown(key, x, y);
	}
}

/// Keyboard function (special keys)
void GLUTSpecialKeyDown(int key, int x, int y)
{
	if (gGLViewer)
	{
		gGLViewer->SpecialKeyDown(key, x, y);
	}
}

/* */
static void onTrackbarHeight(int pos, void* ptr)
{
	cout << "Height" << pos << "%   "<<std::endl;

	double			var = pos / 100.0;
	if(var <=0)
		return;
}
/* */
static void onTrackbarWeight(int pos, void* ptr)
{
	cout << "Weight" << pos << "%   "<<std::endl;

	double			var = pos / 100.0;
	if(var <=0)
		return;
}

/* */
static void onTrackbarChest(int pos, void* ptr)
{
	cout << "Chest" << pos << "%   "<<std::endl;

	double			var = pos / 100.0;
	if(var <=0)
		return;
}


/* */
static void onTrackbarInseam(int pos, void* ptr)
{
	cout << "Height" << pos << "%   "<<std::endl;

	double			var = pos / 100.0;
	if(var <=0)
		return;
}


/* */
static void onTrackbarHips(int pos, void* ptr)
{
	cout << "Hips" << pos << "%   "<<std::endl;

	double			var = pos / 100.0;
	if(var <=0)
		return;
}

/* */
static void onTrackbarWaist(int pos, void* ptr)
{
	cout << "Waist" << pos << "%   "<<std::endl;

	double			var = pos / 100.0;
	if(var <=0)
		return;
}

/* */
static void onTrackbarExercise(int pos, void* ptr)
{
	cout << "Exercise" << pos << "%   "<<std::endl;

	double			var = pos / 100.0;
	if(var <=0)
		return;
}

/* */
 
static void  onTrackbarRed(int state, void* ptr)
{

}
/* */
 
static void  onTrackbarBlue(int state, void* ptr)
{

}
/* */
 
static void  onTrackbarGreen(int state, void* ptr)
{

}
/* */
int main(int argc, char ** argv)
{
	// Testing
	const string	windowName = "BodyVisualizer";
	namedWindow(windowName, cv::WINDOW_AUTOSIZE);

	// setOpenGlContext(windowName);	
	resizeWindow(windowName, 480, 640);
	// setOpenGlDrawCallback(windowName,draw,NULL);
	//Mat img = imread(argv[1]);
	//imshow(windowName,img);
	//waitKey(0);
	//endof Testing


	// create the tracbar for Height
	int pos = 95;
	int p = 100;
	createTrackbar("Height--", windowName, &pos, 100, onTrackbarHeight, (void*) &p);

	// create the tracbar for Weight
	createTrackbar("Weight--", windowName, &pos, 100, onTrackbarWeight, (void*) &p);

	// create the tracbar for Chest
	createTrackbar("Chest--", windowName, &pos, 100, onTrackbarChest, (void*) &p);

	// create the tracbar for Waist
	createTrackbar("Waist--", windowName, &pos, 100, onTrackbarWaist, (void*) &p);

	// create the tracbar for Hips
	createTrackbar("Hips--", windowName, &pos, 100, onTrackbarHips, (void*) &p);

	// create the tracbar for Inseam
	createTrackbar("Inseam--", windowName, &pos, 100, onTrackbarInseam, (void*) &p);

	// create the tracbar for Excercise
	createTrackbar("Exercise--", windowName, &pos, 100, onTrackbarExercise, (void*) &p);

	// for colors
	// create the tracbar for Red Color
	createTrackbar("Red--", windowName, &pos, 100, onTrackbarRed, (void*) &p);
	// create the tracbar for Blue Color
	createTrackbar("Blue--", windowName, &pos, 100, onTrackbarBlue, (void*) &p);
	// create the tracbar for Green Color
	createTrackbar("Green--", windowName, &pos, 100, onTrackbarGreen, (void*) &p);

	// Run the application class
	gGLViewer = new GLViewer;
	gGLViewer->Run(argc, argv);
	
	delete gGLViewer;
	gGLViewer = NULL;
	destroyAllWindows();
	return 1;
}
