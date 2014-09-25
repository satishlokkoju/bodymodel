#ifndef _GLVIEWER_H_
#define _GLVIEWER_H_

#include <stdexcept>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <GL/glew.h>

#ifdef __APPLE_CC__
  #include <GLUT/glut.h>
#else
  #include <C:\opencv\freeglut-2.8.0\include/GL/glut.h>
#endif

#include "mesh.h"
#include "meshio.h"
#include "sysdialog.h"
#include "systimer.h"
#include "common.h"
#include "vertexShader.h"
#include "model_viewer.h"
#include "modelloader.h"


using namespace std;


// We need PI
#ifndef PI
  #define PI 3.141592653589793238462643f
#endif

// Configuration constants
#define FOCUS_TIME        0.1
#define DOUBLE_CLICK_TIME 0.25


//-----------------------------------------------------------------------------
// GLSL source code (generated from source by bin2c)
//-----------------------------------------------------------------------------

#include "phong_vert.h"
#include "phong_frag.h"


//-----------------------------------------------------------------------------
// Icon bitmaps
//-----------------------------------------------------------------------------

#include "icons/icon_open.h"
#include "icons/icon_save.h"
#include "icons/icon_texture.h"
#include "icons/icon_help.h"


//-----------------------------------------------------------------------------
// The GLViewer application class (declaration)
//-----------------------------------------------------------------------------

class GLButton;
class AttribLocations
{
public:
	int position;
	int normal;
	int texCoord;

	int aPositionOffsets[7];
	int aNormalOffsets[7];

};

class UniformLocations
{
public:
	int color;
	int texture;
	int normal;
	int world;
	int worldInverseTranspose;
	int worldViewProj;
	int normalMatrix;
	int viewInverse;
	int meanScaleFactorLocation;
	int scaleFactor[7];
};

class ShaderProgram
{
public:
	int pMatrixUniform;
	int mvMatrixUniform;
};

class GLViewer 
{
  private:
    // File information for the current mesh
    string mFileName, mFilePath;
    long mFileSize;

    // Window state cariables
    int mWidth, mHeight;
    GLint mDepthBufferResolution;
    int mOldMouseX, mOldMouseY;
    double mLastClickTime;
    bool mMouseRotate;
    bool mMouseZoom;
    bool mMousePan;
    bool mFocusing;
    Point3d mFocusStartPos;
    Point3d mFocusEndPos;
    double mFocusStartTime;
    double mFocusEndTime;
    double mFocusStartDistance;
    double mFocusEndDistance;
	int number_of_offset_meshes;
    // Camera state
    Point3d mCameraPosition;
    Point3d mCameraLookAt;
    Point3d mCameraUp;
    GLdouble mModelviewMatrix[16];
    GLdouble mProjectionMatrix[16];
    GLint mViewport[4];

    // Mesh information
    Mesh * mMesh;
    Point3d mAABBMin, mAABBMax;
    GLuint mDisplayList;
    GLuint mTexHandle;

    // Polygon rendering mode (fill / line)
    GLenum mPolyMode;

    // GLSL objects
    bool mUseShader;
    GLuint mShaderProgram;
    GLuint mVertShader;
    GLuint mFragShader;

    // List of GUI buttons
    list<GLButton *> mButtons;

    // Master timer resource
    SysTimer mTimer;

    /// Set up the camera.
    void SetupCamera();

    /// Initialize the GLSL shader (requires OpenGL 2.0 or better).
    void InitShader();

    /// Initialize the texture.
    void InitTexture(const char * aFileName);

    /// Set up the scene lighting.
    void SetupLighting();

    /// Set up the material.
    void SetupMaterial();

    /// Draw a mesh
    void DrawMesh(Mesh * aMesh);
    void DrawMeshProgPL(modelloader * ml);
	void  DrawDebugMesh();
    /// Load a file to the mesh
    void LoadFile(const char * aFileName, const char * aOverrideTexture);

    /// Load a texture file
    void LoadTexture(const char * aFileName);

    /// Draw an outline box.
    void DrawOutlineBox(int x1, int y1, int x2, int y2, float r, float g, float b, float a);

    /// Draw a string using GLUT. The string is shown on top of an alpha-blended
    /// quad.
    void DrawString(string aString, int x, int y);

    /// Draw 2D overlay
    void Draw2DOverlay();

    /// Get 3D coordinate under the mouse cursor.
    bool WinCoordTo3DCoord(int x, int y, Point3d &aPoint);

    /// Update the focus position of the camera.
    void UpdateFocus();
	void frustrum(Mat &input,double a,double b,double c,double d, double e, double f);
	void translate(Mat &in,double x,double y, double z);
	void rotate(Mat &in,double angle,double x,double y ,double z);
  public:
    /// Constructor
    GLViewer();

    /// Destructor
    ~GLViewer();

    /// Open another file
    void ActionOpenFile();

    /// Save the file
    void ActionSaveFile();

    /// Open a texture file
    void ActionOpenTexture();

    /// Toggle wire frame view on/off
    void ActionToggleWireframe();

    /// Fit model to the screen (re-focus)
    void ActionFitToScreen();

    /// Set camera up direction to Y
    void ActionCameraUpY();

    /// Set camera up direction to Z
    void ActionCameraUpZ();

    /// Zoom camera one step in
    void ActionZoomIn();

    /// Zoom camera one step out
    void ActionZoomOut();

    /// Exit program
    void ActionExit();

    /// Show a help dialog
    void ActionHelp();

    /// Redraw function.
    void WindowRedraw(void);

    /// Redraw function.
    void WindowRedraw_temp(void);
    /// Resize function.
    void WindowResize(int w, int h);

    /// Mouse click function
    void MouseClick(int button, int state, int x, int y);

    /// Mouse move function
    void MouseMove(int x, int y);

    /// Keyboard function
    void KeyDown(unsigned char key, int x, int y);

    /// Keyboard function (special keys)
    void SpecialKeyDown(int key, int x, int y);

    /// Run the application
    void Run(int argc, char **argv);
	void LoadMale(const char *aOverrideTexture);
	void LoadFemale(const char *aOverrideTexture);
	// Attrib  locations
	AttribLocations attributeLocations;
	//uniform locations
	UniformLocations uniformLocations;
	//shader locations
	ShaderProgram shaderProgram;

	// Alloc attrib
	void getAttribUniform();
};



//-----------------------------------------------------------------------------
// A class for OpenGL rendered GUI buttons
//-----------------------------------------------------------------------------

class GLButton {
  private:
    // Texture handle
    GLuint mTexHandle;

    // Highlight on/off
    bool mHighlight;

  public:
    /// Constructor.
    GLButton()
    {
      mTexHandle = 0;
      mPosX = 0;
      mPosY = 0;
      mWidth = 32;
      mHeight = 32;
      mHighlight = false;
      mParent = NULL;
    }

    /// Destructor.
    virtual ~GLButton()
    {
      if(mTexHandle)
        glDeleteTextures(1, &mTexHandle);
    }

    /// Set glyph for this button.
    void SetGlyph(const unsigned char * aBitmap, int aWidth, int aHeight,int aComponents)
    {
      // Update the button size
      mWidth = aWidth;
      mHeight = aHeight;

      // Upload the texture to OpenGL
      if(mTexHandle)
        glDeleteTextures(1, &mTexHandle);
      glGenTextures(1, &mTexHandle);
      if(mTexHandle)
      {
        // Determine the color format
        GLuint format;
        if(aComponents == 3)
          format = GL_RGB;
        else if(aComponents == 4)
          format = GL_RGBA;
        else
          format = GL_LUMINANCE;

        glBindTexture(GL_TEXTURE_2D, mTexHandle);

        if(GLEW_VERSION_1_4)
        {
          // Generate mipmaps automatically and use them
          glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
          glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        }
        else
        {
          glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        }
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTexImage2D(GL_TEXTURE_2D, 0, aComponents, aWidth, aHeight, 0, format, GL_UNSIGNED_BYTE, (GLvoid *) aBitmap);
      }
    }

    /// Redraw function.
    void Redraw()
    {
      // Set opacity of the icon
      if(mHighlight)
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
      else
        glColor4f(1.0f, 1.0f, 1.0f, 0.7f);

      // Enable texturing
      if(mTexHandle)
      {
        glBindTexture(GL_TEXTURE_2D, mTexHandle);
        glEnable(GL_TEXTURE_2D);
      }

      // Enable blending
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

      // Draw the icon as a textured quad
      glBegin(GL_QUADS);
      glTexCoord2f(0.0f, 0.0f);
      glVertex2i(mPosX, mPosY);
      glTexCoord2f(1.0f, 0.0f);
      glVertex2i(mPosX + mWidth, mPosY);
      glTexCoord2f(1.0f, 1.0f);
      glVertex2i(mPosX + mWidth, mPosY + mHeight);
      glTexCoord2f(0.0f, 1.0f);
      glVertex2i(mPosX, mPosY + mHeight);
      glEnd();

      // We're done
      glDisable(GL_BLEND);
      glDisable(GL_TEXTURE_2D);
    }

    /// Mouse move function. The function returns true if the state of the
    /// button has changed.
    bool MouseMove(int x, int y)
    {
      bool hit = (x >= mPosX) && (x < (mPosX + mWidth)) &&
                 (y >= mPosY) && (y < (mPosY + mHeight));
      bool changed = (mHighlight != hit);
      mHighlight = hit;
      return changed;
    }

    /// Mouse click function.
    bool MouseClick(int aState, int x, int y)
    {
      bool hit = (x >= mPosX) && (x < (mPosX + mWidth)) &&
                 (y >= mPosY) && (y < (mPosY + mHeight));
      if(hit && (aState == GLUT_DOWN))
        DoAction();
      return hit;
    }

    /// The action function that will be performed when a button is clicked.
    virtual void DoAction() {}

    GLint mPosX, mPosY;
    GLint mWidth, mHeight;
    GLViewer * mParent;
};

//-----------------------------------------------------------------------------
// Customized button classes (implementing different actions)
//-----------------------------------------------------------------------------

class OpenButton: public GLButton 
{
  public:
    void DoAction()
    {
      if(!mParent)
        return;
      mParent->ActionOpenFile();
    }
};


class maleButton: public GLButton 
{
  public:
    void DoAction()
    {
      if(!mParent)
        return;
	  mParent->LoadMale("male");
    }
};

class femaleButton: public GLButton 
{
  public:
    void DoAction()
    {
      if(!mParent)
        return;
	  mParent->LoadFemale("male");
    }
};


class SaveButton: public GLButton 
{
  public:
    void DoAction()
    {
      if(!mParent)
        return;
      mParent->ActionSaveFile();
    }
};

class OpenTextureButton: public GLButton {
  public:
    void DoAction()
    {
      if(!mParent)
        return;
      mParent->ActionOpenTexture();
    }
};

class HelpButton: public GLButton {
  public:
    void DoAction()
    {
      if(!mParent)
        return;
      mParent->ActionHelp();
    }
};

#endif