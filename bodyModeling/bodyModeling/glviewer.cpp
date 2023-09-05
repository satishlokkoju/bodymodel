#include "glviewer.h"
#include <math.h>
using namespace std;

//-----------------------------------------------------------------------------
// GLUT callback function prototypes
//-----------------------------------------------------------------------------

void GLUTWindowRedraw(void);
void GLUTWindowResize(int w, int h);
void GLUTMouseClick(int button, int state, int x, int y);
void GLUTMouseMove(int x, int y);
void GLUTKeyDown(unsigned char key, int x, int y);
void GLUTSpecialKeyDown(int key, int x, int y);


//-----------------------------------------------------------------------------
// GLViewer: OpenGL related functions
//-----------------------------------------------------------------------------

/// Set up the camera.
void GLViewer::SetupCamera()
{
  if(mMesh)
    mMesh->BoundingBox(mAABBMin, mAABBMax);
  else
  {
    mAABBMin = Point3d(-1.0f, -1.0f, -1.0f);
    mAABBMax = Point3d(1.0f, 1.0f, 1.0f);
  }
  mCameraLookAt = (mAABBMax + mAABBMin) * 0.5f;
  
  float delta = AbsValue(mAABBMax - mAABBMin);

  if(mCameraUp.z > 0.0f)
  {
    //mCameraPosition = Point3d(mCameraLookAt.x, mCameraLookAt.y - 0.8f * delta, mCameraLookAt.z + 0.2f * delta);
	  mCameraPosition = Point3d(mCameraLookAt.x, mCameraLookAt.y - 0.8f * delta, mCameraLookAt.z + 0.4f * delta);
  }
  else
    mCameraPosition = Point3d(mCameraLookAt.x, mCameraLookAt.y + 0.2f * delta, mCameraLookAt.z + 0.8f * delta);
}

/// Initialize the GLSL shader (requires OpenGL 2.0 or better).
void GLViewer::InitShader()
{
  const GLchar * src[1];
#if 1
  string VertexShader = generateVertexShader();
  string FragmentShader = generatefragmentShader(false);
  //string phongVerShad = phongVertexShader();
  //string phongFragShad = phongFragShader();
 
  //const char *vertexShader = .c_str();

  // Load vertex shader
  mVertShader = glCreateShader(GL_VERTEX_SHADER);
  //src[0] = (const GLchar *) phongVertSrc;
  //src[0] = phongVerShad.c_str();
  src[0] = VertexShader.c_str();
  glShaderSource(mVertShader, 1, src, NULL);

  // Load fragment shader
  mFragShader = glCreateShader(GL_FRAGMENT_SHADER);	
  //src[0] = (const GLchar *) phongFragSrc;
  //src[0] = phongFragShad.c_str();
  src[0] = FragmentShader.c_str();
  glShaderSource(mFragShader, 1, src, NULL);
#else
  
  // Load vertex shader
  mVertShader = glCreateShader(GL_VERTEX_SHADER);
  src[0] = (const GLchar *) phongVertSrc;
  glShaderSource(mVertShader, 1, src, NULL);

  // Load fragment shader
  mFragShader = glCreateShader(GL_FRAGMENT_SHADER);	
  src[0] = (const GLchar *) phongFragSrc;
  glShaderSource(mFragShader, 1, src, NULL);
#endif
  int status;

  // Compile the vertex shader
  glCompileShader(mVertShader);
  glGetShaderiv(mVertShader, GL_COMPILE_STATUS, &status);
  if(!status)
  {
	GLint infoLen = 0;
	glGetShaderiv(mVertShader, GL_INFO_LOG_LENGTH, &infoLen);
	if(infoLen > 1)
	{
		char* infoLog =(char *) malloc(sizeof(char) * infoLen);
		glGetShaderInfoLog(mVertShader, infoLen, NULL, infoLog);
		printf("Error compiling shader:\n%s\n", infoLog);
		free(infoLog);
	}
	glDeleteShader(mVertShader);
    throw runtime_error("Could not compile vertex shader.");
  }
  // Compile the fragment shader
  glCompileShader(mFragShader);
  glGetShaderiv(mFragShader, GL_COMPILE_STATUS, &status);

  if(!status)
    throw runtime_error("Could not compile fragment shader.");

  // Link the shader program
  mShaderProgram = glCreateProgram();
  glAttachShader(mShaderProgram, mVertShader);
  glAttachShader(mShaderProgram, mFragShader);
  glLinkProgram(mShaderProgram);
  glGetProgramiv(mShaderProgram, GL_LINK_STATUS, &status);
  if(!status)
    throw runtime_error("Could not link shader program.");

  mUseShader = true;
}


void GLViewer::getAttribUniform()
{	
    attributeLocations.position = glGetAttribLocation(mShaderProgram, "aPosition");
    attributeLocations.normal = glGetAttribLocation(mShaderProgram, "aNormal");
    attributeLocations.texCoord = glGetAttribLocation(mShaderProgram, "aTexture");

    attributeLocations.aPositionOffsets[0] = glGetAttribLocation(mShaderProgram, "aPositionOffsets1");
    attributeLocations.aNormalOffsets[0] = glGetAttribLocation(mShaderProgram, "aNormalOffsets1");
    uniformLocations.scaleFactor[0] = glGetUniformLocation(mShaderProgram, "scaleFactor1");

    attributeLocations.aPositionOffsets[1] = glGetAttribLocation(mShaderProgram, "aPositionOffsets2");
    attributeLocations.aNormalOffsets[1] = glGetAttribLocation(mShaderProgram, "aNormalOffsets2");
    uniformLocations.scaleFactor[1] = glGetUniformLocation(mShaderProgram, "scaleFactor2");
	
    attributeLocations.aPositionOffsets[2] = glGetAttribLocation(mShaderProgram, "aPositionOffsets3");
    attributeLocations.aNormalOffsets[2] = glGetAttribLocation(mShaderProgram, "aNormalOffsets3");
    uniformLocations.scaleFactor[2] = glGetUniformLocation(mShaderProgram, "scaleFactor3");

    attributeLocations.aPositionOffsets[3] = glGetAttribLocation(mShaderProgram, "aPositionOffsets4");
    attributeLocations.aNormalOffsets[3] = glGetAttribLocation(mShaderProgram, "aNormalOffsets4");
    uniformLocations.scaleFactor[3] = glGetUniformLocation(mShaderProgram, "scaleFactor4");

    attributeLocations.aPositionOffsets[4] = glGetAttribLocation(mShaderProgram, "aPositionOffsets5");
    attributeLocations.aNormalOffsets[4] = glGetAttribLocation(mShaderProgram, "aNormalOffsets5");
    uniformLocations.scaleFactor[4] = glGetUniformLocation(mShaderProgram, "scaleFactor5");

    attributeLocations.aPositionOffsets[5] = glGetAttribLocation(mShaderProgram, "aPositionOffsets6");
    attributeLocations.aNormalOffsets[5] = glGetAttribLocation(mShaderProgram, "aNormalOffsets6");
    uniformLocations.scaleFactor[5] = glGetUniformLocation(mShaderProgram, "scaleFactor6");

	attributeLocations.aPositionOffsets[6] = glGetAttribLocation(mShaderProgram, "aPositionOffsets7");
    attributeLocations.aNormalOffsets[6] = glGetAttribLocation(mShaderProgram, "aNormalOffsets7");
    uniformLocations.scaleFactor[6] = glGetUniformLocation(mShaderProgram, "scaleFactor7");
	//shader
	shaderProgram.pMatrixUniform = glGetUniformLocation(mShaderProgram, "uPMatrix");
    shaderProgram.mvMatrixUniform = glGetUniformLocation(mShaderProgram, "uMVMatrix");
    uniformLocations.world = glGetUniformLocation(mShaderProgram, "world");
    uniformLocations.worldInverseTranspose = glGetUniformLocation(mShaderProgram, "worldInverseTranspose");
    uniformLocations.worldViewProj = glGetUniformLocation(mShaderProgram, "worldViewProj");
    uniformLocations.viewInverse = glGetUniformLocation(mShaderProgram, "viewInverse");
    uniformLocations.normalMatrix = glGetUniformLocation(mShaderProgram, "normalMatrix");
    uniformLocations.color = glGetUniformLocation(mShaderProgram, "color");
    uniformLocations.texture = glGetUniformLocation(mShaderProgram, "textureSampler");

    uniformLocations.meanScaleFactorLocation = glGetUniformLocation(mShaderProgram, "meanScaleFactor");
}
/// Initialize the texture.
void GLViewer::InitTexture(const char * aFileName)
{
  Mat image;

  // Load texture from a file
  if(aFileName)
  {
    // Check if file exists, and determine actual file name (relative or absolute)
    bool fileExists = false;
    string name = string(aFileName);
    FILE * inFile = fopen(name.c_str(), "rb");
    
	if(inFile)
      fileExists = true;

    else if(mFilePath.size() > 0)
    {
      // Try the same path as the mesh file
      name = mFilePath + string(aFileName);
      inFile = fopen(name.c_str(), "rb");
      if(inFile)
        fileExists = true;
    }
    if(inFile)
      fclose(inFile);

    if(fileExists)
    {
      cout << "Loading texture (" << aFileName << ")..." << endl;
      try
      {
        image = imread(name.c_str());
      }
      catch(exception &e)
      {
        cout << "Error loading texture: " << e.what() << endl;
      }
    }
  }

  // If no texture was loaded
  if(image.empty())
  {
    cout << "Loading texture (dummy)..." << endl;

    // Create a default, synthetic texture
    image = Mat(256, 256, 1);
    for(int y = 0; y < image.rows; ++ y)
    {
      for(int x = 0; x < image.cols; ++ x)
      {
        if(((x & 0x000f) == 0) || ((y & 0x000f) == 0))
          image.data[y * image.cols + x] = 192;
        else
          image.data[y * image.cols + x] = 255;
      }
    }
  }

  // Upload the texture to OpenGL
  if(!image.empty())
  {
    glGenTextures(1, &mTexHandle);
  }
  else
    mTexHandle = 0;

  if(mTexHandle)
  {
	cv::flip(image,image,0);
    // Determine the color format
    GLuint format;
	if(image.channels() == 3)
      format = GL_BGR;
    else if(image.channels() == 4)
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
	int compts = image.channels();
    glTexImage2D(GL_TEXTURE_2D, 0, compts, image.cols, image.rows, 0, format, GL_UNSIGNED_BYTE, (GLvoid *) &image.data[0]);
  }
}

/// Set up the scene lighting.
void GLViewer::SetupLighting()
{
  GLfloat pos[4], ambient[4], diffuse[4], specular[4];

  // Set scene lighting properties
  glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
  glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
  ambient[0] = 0.2;
  ambient[1] = 0.2;
  ambient[2] = 0.2;
  ambient[3] = 1.0;
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);

  // Set-up head light (GL_LIGHT0)
  pos[0] = mCameraPosition.x;
  pos[1] = mCameraPosition.y;
  pos[2] = mCameraPosition.z;
  pos[3] = 1.0f;
  glLightfv(GL_LIGHT0, GL_POSITION, pos);
  ambient[0] = 0.0f;
  ambient[1] = 0.0f;
  ambient[2] = 0.0f;
  ambient[3] = 1.0f;
  glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
  diffuse[0] = 0.8f;
  diffuse[1] = 0.8f;
  diffuse[2] = 0.8f;
  diffuse[3] = 1.0f;
  glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
  specular[0] = 1.0f;
  specular[1] = 1.0f;
  specular[2] = 1.0f;
  specular[3] = 1.0f;
  glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
  glEnable(GL_LIGHT0);
}

/// Set up the material.
void GLViewer::SetupMaterial()
{
  GLfloat specular[4], emission[4];

  // Set up the material
  specular[0] = 0.3f;
  specular[1] = 0.3f;
  specular[2] = 0.3f;
  specular[3] = 1.0f;
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
  emission[0] = 0.0f;
  emission[1] = 0.0f;
  emission[2] = 0.0f;
  emission[3] = 1.0f;
  glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emission);
  glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 40.0f);

  // Use color material for the diffuse and ambient components
  glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
  glEnable(GL_COLOR_MATERIAL);
}

void GLViewer::translate(Mat &in,double tx,double ty, double tz)
{
	in.at<float>(3,0) += in.at<float>(0,0) * tx + in.at<float>(1,0) * ty + in.at<float>(2,0) * tz;
    in.at<float>(3,1) += in.at<float>(0,1) * tx + in.at<float>(1,1) * ty + in.at<float>(2,1) * tz;
    in.at<float>(3,2) += in.at<float>(0,2) * tx + in.at<float>(1,2) * ty + in.at<float>(2,2) * tz;
    in.at<float>(3,3) += in.at<float>(0,3) * tx + in.at<float>(1,3) * ty + in.at<float>(2,3) * tz;
}

#define M_PI       3.14159265358979323846

void GLViewer::rotate(Mat &inputMat,double angle,double x,double y ,double z)
{
	double mag = sqrt(x*x + y*y + z*z);
    double sinAngle = sin(angle * M_PI / 180.0);
    double cosAngle = cos(angle * M_PI / 180.0);

    if (mag > 0)
    {
      double xx, yy, zz, xy, yz, zx, xs, ys, zs;
      double oneMinusCos;

      x /= mag;
      y /= mag;
      z /= mag;

      xx = x * x;
      yy = y * y;
      zz = z * z;
      xy = x * y;
      yz = y * z;
      zx = z * x;
      xs = x * sinAngle;
      ys = y * sinAngle;
      zs = z * sinAngle;
      oneMinusCos = 1.0 - cosAngle;
	
	  Mat rotMat(4,4,CV_32F);

      rotMat.at<float>(0,0) = (oneMinusCos * xx) + cosAngle;
      rotMat.at<float>(0,1) = (oneMinusCos * xy) - zs;
      rotMat.at<float>(0,2) = (oneMinusCos * zx) + ys;
      rotMat.at<float>(0,3) = 0.0;

      rotMat.at<float>(1,0) = (oneMinusCos * xy) + zs;
      rotMat.at<float>(1,1) = (oneMinusCos * yy) + cosAngle;
      rotMat.at<float>(1,2) = (oneMinusCos * yz) - xs;
      rotMat.at<float>(1,3) = 0.0;

      rotMat.at<float>(2,0) = (oneMinusCos * zx) - ys;
      rotMat.at<float>(2,1) = (oneMinusCos * yz) + xs;
      rotMat.at<float>(2,2) = (oneMinusCos * zz) + cosAngle;
      rotMat.at<float>(2,3) = 0.0;

      rotMat.at<float>(3,0) = 0.0;
      rotMat.at<float>(3,1) = 0.0;
      rotMat.at<float>(3,2) = 0.0;
      rotMat.at<float>(3,3) = 1.0;

      inputMat = inputMat*rotMat;
	}
}

void GLViewer::frustrum(Mat &inputMat,double left,double right,double bottom,double top, double nearZ, double farZ)
{
    double deltaX = right - left;
    double deltaY = top - bottom;
    double deltaZ = farZ - nearZ;
    Mat frust;

    if ( (nearZ <= 0.0) || (farZ <= 0.0) || (deltaX <= 0.0) || (deltaY <= 0.0) || (deltaZ <= 0.0) )
         return;

	frust =Mat::zeros(4,4,CV_32F);

    frust.at<float>(0,0) = 2.0 * nearZ / deltaX;
    frust.at<float>(0,1) = frust.at<float>(0,2) = frust.at<float>(0,3) = 0.0;

    frust.at<float>(1,1) = 2.0 * nearZ / deltaY;
    frust.at<float>(1,0) = frust.at<float>(1,2) = frust.at<float>(1,3) = 0.0;

    frust.at<float>(2,0) = (right + left) / deltaX;
    frust.at<float>(2,1) = (top + bottom) / deltaY;
    frust.at<float>(2,2) = -(nearZ + farZ) / deltaZ;
    frust.at<float>(2,3) = -1.0;

    frust.at<float>(3,2) = -2.0 * nearZ * farZ / deltaZ;
    frust.at<float>(3,0) = frust.at<float>(3,1) = frust.at<float>(3,3) = 0.0;
	
	inputMat = inputMat*frust;

}

/// Draw a mesh
void GLViewer::DrawMesh(Mesh * aMesh)
{

  // We always have vertices
  glVertexPointer(3, GL_FLOAT, 0, &aMesh->mVertices[0]);
  glEnableClientState(GL_VERTEX_ARRAY);

  // Do we have normals?
  if(aMesh->mNormals.size() == aMesh->mVertices.size())
  {
    glNormalPointer(GL_FLOAT, 0, &aMesh->mNormals[0]);
    glEnableClientState(GL_NORMAL_ARRAY);
  }

  // Do we have texture coordinates?
  if(aMesh->mTexCoords.size() == aMesh->mVertices.size())
  {
    glTexCoordPointer(2, GL_FLOAT, 0, &aMesh->mTexCoords[0]);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  }

  // Do we have colors?
  if(aMesh->mColors.size() == aMesh->mVertices.size())
  {
    glColorPointer(4, GL_FLOAT, 0, &aMesh->mColors[0]);
    glEnableClientState(GL_COLOR_ARRAY);
  }

  // Use glDrawElements to draw the triangles...
  glShadeModel(GL_SMOOTH);
  if(GLEW_VERSION_1_2)
    glDrawRangeElements(GL_TRIANGLES, 0, aMesh->mVertices.size() - 1, aMesh->mIndices.size(), GL_UNSIGNED_INT, &aMesh->mIndices[0]);
  else
    glDrawElements(GL_TRIANGLES, aMesh->mIndices.size(), GL_UNSIGNED_INT,&aMesh->mIndices[0]);

  // We do not use the client state anymore...
  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_NORMAL_ARRAY);
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
  glDisableClientState(GL_COLOR_ARRAY);
}
struct MyVertex
{
float x, y, z;        //Vertex
float nx, ny, nz;     //Normal
float s0, t0;         //Texcoord0
};
   //Define this somewhere in your header file
#define BUFFER_OFFSET(i) ((char *)NULL + (i))
void GLViewer::DrawDebugMesh()
{
  MyVertex pvertex[3];
  //VERTEX 0
  pvertex[0].x = 0.0;
  pvertex[0].y = 0.0;
  pvertex[0].z = 0.0;
  pvertex[0].nx = 0.0;
  pvertex[0].ny = 0.0;
  pvertex[0].nz = 1.0;
  pvertex[0].s0 = 0.0;
  pvertex[0].t0 = 0.0;
  //VERTEX 1
  pvertex[1].x = 1.0;
  pvertex[1].y = 0.0;
  pvertex[1].z = 0.0;
  pvertex[1].nx = 0.0;
  pvertex[1].ny = 0.0;
  pvertex[1].nz = 1.0;
  pvertex[1].s0 = 1.0;
  pvertex[1].t0 = 0.0;
  //VERTEX 2
  pvertex[2].x = 0.0;
  pvertex[2].y = 1.0;
  pvertex[2].z = 0.0;
  pvertex[2].nx = 0.0;
  pvertex[2].ny = 0.0;
  pvertex[2].nz = 1.0;
  pvertex[2].s0 = 0.0;
  pvertex[2].t0 = 1.0;
 
  GLuint VertexVBOID;
  glGenBuffers(1, &VertexVBOID);
  glBindBuffer(GL_ARRAY_BUFFER, VertexVBOID);
  glBufferData(GL_ARRAY_BUFFER, sizeof(MyVertex)*3, &pvertex[0].x, GL_STATIC_DRAW);
 
  ushort pindices[3];
  pindices[0] = 0;
  pindices[1] = 1;
  pindices[2] = 2;
  GLuint IndexVBOID;
  glGenBuffers(1, &IndexVBOID);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexVBOID);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ushort)*3, pindices, GL_STATIC_DRAW);
  
  glBindBuffer(GL_ARRAY_BUFFER, VertexVBOID);
  glEnableVertexAttribArray(0);    //We like submitting vertices on stream 0 for no special reason
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(MyVertex), BUFFER_OFFSET(0));   //The starting point of the VBO, for the vertices
  glEnableVertexAttribArray(1);    //We like submitting normals on stream 1 for no special reason
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(MyVertex), BUFFER_OFFSET(12));     //The starting point of normals, 12 bytes away
  glEnableVertexAttribArray(2);    //We like submitting texcoords on stream 2 for no special reason
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(MyVertex), BUFFER_OFFSET(24));   //The starting point of texcoords, 24 bytes away
 
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexVBOID);
  //To render, we can either use glDrawElements or glDrawRangeElements
  //The is the number of indices. 3 indices needed to make a single triangle
  glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, BUFFER_OFFSET(0));   //The starting point of the IBO
  //0 and 3 are the first and last vertices
  //glDrawRangeElements(GL_TRIANGLES, 0, 3, 3, GL_UNSIGNED_SHORT, BUFFER_OFFSET(0));   //The starting point of the IBO
  //glDrawRangeElements may or may not give a performance advantage over glDrawElements
}
/// Draw a mesh
void GLViewer::DrawMeshProgPL(modelloader * aml)
{
	GLenum erro;
	Mesh *aMesh = aml->getMeshes()[0];

	int positions_byte_length = aMesh->mVertices.size()*sizeof(Point3f);
	int normals_byte_length = aMesh->mNormals.size()*sizeof(Point3f);
	int textur_byte_length = aMesh->mTexCoords.size()*sizeof(Point2f);

	int positionsOffset = 0;
    int normalsOffset = positionsOffset + positions_byte_length;
    int texCoordsOffset = normalsOffset + normals_byte_length;
	
    int initial_offsets_offset = texCoordsOffset + textur_byte_length;
    int positionsOffsetsOffsets[7];
    int normalsOffsetsOffsets[7];

	int positionsOffsetsAttributeLocations[7];
    int normalsOffsetsAttributeLocations[7];

    for (int i = 0; i < number_of_offset_meshes; i++)
    {
		positionsOffsetsOffsets[i] = initial_offsets_offset + i * (positions_byte_length + normals_byte_length);
        normalsOffsetsOffsets[i] = positionsOffsetsOffsets[i] + positions_byte_length;
    }

	GLuint IBOID;
	glGenBuffers(1,&IBOID);

	int numElements = aMesh->mIndices.size();

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBOID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,numElements*sizeof(int), &aMesh->mIndices[0], GL_STATIC_DRAW);
	
	erro = glGetError();
    if (erro != GL_NO_ERROR)
    {
        throw runtime_error("GL Error: " + erro);
    }

	GLuint VBOID;
	GLuint VAOID;
	glGenBuffers(1,&VBOID);
	glBindBuffer(GL_ARRAY_BUFFER,VBOID);
	glBufferData(GL_ARRAY_BUFFER,positions_byte_length + normals_byte_length + textur_byte_length + number_of_offset_meshes * positions_byte_length + number_of_offset_meshes * normals_byte_length,NULL,GL_STATIC_DRAW);
	
	erro = glGetError();
    if (erro != GL_NO_ERROR)
    {
        throw runtime_error("GL Error: " + erro);
    }
    glBufferSubData(GL_ARRAY_BUFFER, positionsOffset,positions_byte_length ,&aMesh->mVertices[0]);
	erro = glGetError();
    if (erro != GL_NO_ERROR)
    {
        throw runtime_error("GL Error: " + erro);
    }
    
	glBufferSubData(GL_ARRAY_BUFFER, normalsOffset, normals_byte_length,&aMesh->mNormals[0]);
    
	erro = glGetError();
    if (erro != GL_NO_ERROR)
    {
        throw runtime_error("GL Error: " + erro);
    }
	
	glBufferSubData(GL_ARRAY_BUFFER, texCoordsOffset, textur_byte_length,&aMesh->mTexCoords[0]);
	
	erro = glGetError();
    if (erro != GL_NO_ERROR)
    {
        throw runtime_error("GL Error: " + erro);
    }

    for (int i = 0; i < number_of_offset_meshes; i++)
    {
        glBufferSubData(GL_ARRAY_BUFFER, positionsOffsetsOffsets[i],1 ,&(aml->getMeshes()[i]->mVertices[0]));
        glBufferSubData(GL_ARRAY_BUFFER, normalsOffsetsOffsets[i],1,&(aml->getMeshes()[i]->mNormals[0]));
    }
    
	//VAO for the quad *********************
	glGenVertexArrays(1, &VAOID);
	glBindVertexArray(VAOID);
 
	//Bind the VBO and setup pointers for the VAO
	glBindBuffer(GL_ARRAY_BUFFER, VBOID);

    //glBindBuffer(GL_ARRAY_BUFFER, vboId);
	 if (attributeLocations.position != -1)
     {
		glVertexAttribPointer(attributeLocations.position, 3, GL_FLOAT, false, 0, (const void*)positionsOffset);
		glEnableVertexAttribArray(attributeLocations.position);
	 }

	 if (attributeLocations.normal != -1)
	 {
		glVertexAttribPointer(attributeLocations.normal, 3, GL_FLOAT, false, 0, (const void*)normalsOffset);
		glEnableVertexAttribArray(attributeLocations.normal);
	 }
     
	 if (attributeLocations.texCoord != -1)
     {
        glVertexAttribPointer(attributeLocations.texCoord, 2, GL_FLOAT, false, 0, (const void*)texCoordsOffset);
        glEnableVertexAttribArray(attributeLocations.texCoord);
     }

    for (int i = 0; i < number_of_offset_meshes; i++)
    {
        // console.log(positionsOffsetsAttributeLocations[i])
        glVertexAttribPointer(positionsOffsetsAttributeLocations[i], 3, GL_FLOAT, false, 0, (const void*)positionsOffsetsOffsets[i]);
        glEnableVertexAttribArray(positionsOffsetsAttributeLocations[i]);
        // console.log(normalsOffsetsAttributeLocations[i])
        glVertexAttribPointer(normalsOffsetsAttributeLocations[i], 3, GL_FLOAT, false, 0, (const void*)normalsOffsetsOffsets[i]);
        glEnableVertexAttribArray(normalsOffsetsAttributeLocations[i]);
    }
  
	erro = glGetError();
    if (erro != GL_NO_ERROR)
    {
        throw runtime_error("GL Error: " + erro);
    }
	glBindVertexArray(VAOID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBOID);  	
	// gl.LightModeli(gl.GL_LIGHT_MODEL_TWO_SIDE, gl.GL_TRUE);
    glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_INT, 0);
    // gl.drawElements(gl.LINES, numElements, gl.UNSIGNED_SHORT, 0);
}


// Load a file to the mesh
void GLViewer::LoadFemale(const char *aOverrideTexture)
{

}
// Load a file to the mesh
void GLViewer::LoadMale(const char *aOverrideTexture)
{
	modelloader *ml = new modelloader();

	//initialize vertices
	// vectors for female mean vetices and faces

	ml->createMesh("male");

	if(mMesh)
		delete mMesh;

	mMesh = ml->getMeshes()[0];

	// Set window title
	string windowCaption = string("Body viewer - Male");
	glutSetWindowTitle(windowCaption.c_str());

	// If the file did not contain any normals, calculate them now...
	if(mMesh->mNormals.size() != mMesh->mVertices.size())
	{
		cout << "Calculating normals..." << flush;
		mTimer.Push();
		mMesh->CalculateNormals();
		cout << "done (" << int(mTimer.PopDelta() * 1000.0 + 0.5) << " ms)" << endl;
	}

	// Load the texture
	if(mTexHandle)
	glDeleteTextures(1, &mTexHandle);

	mTexHandle = 0;

	if(mMesh->mTexCoords.size() == mMesh->mVertices.size())
	{
		string texFileName = mMesh->mTexFileName;
		if(aOverrideTexture)
			texFileName = string(aOverrideTexture);

		if(texFileName.size() > 0)
			InitTexture(texFileName.c_str());
		else
			InitTexture(0);
	}

	// Load the mesh into a displaylist
	if(mDisplayList)
		glDeleteLists(mDisplayList, 1);

	mDisplayList = glGenLists(1);
	glNewList(mDisplayList, GL_COMPILE);
	
	if(mUseShader)
	{
		glUseProgram(mShaderProgram);
		getAttribUniform();
		
		Mat model = Mat::zeros(4,4,CV_32F);
		Mat view = Mat::zeros(4,4,CV_32F);
		Mat proj = Mat::zeros(4,4,CV_32F);
		Mat mvp =  Mat::zeros(4,4,CV_32F);
		Mat normalMatrix = Mat::zeros(4,4,CV_32F);

		for(int i =0; i<4; i++)
		{
			model.at<float>(i,i) = 1.0;
			view.at<float>(i,i) = 1.0;
			proj.at<float>(i,i) = 1.0;
			mvp.at<float>(i,i) = 1.0;
			normalMatrix.at<float>(i,i) = 1.0;
		}

		double frustrumH = tan(45.0/360*M_PI)*0.1;
		double frustrumW = frustrumH*mWidth/mHeight;
			
		frustrum(proj,-frustrumW,frustrumW,-frustrumH,frustrumH,0.1,100);

		translate(model,0.0,-1.0,-3.0);
		double xrot = 10;
		double yrot = 10;
		rotate(model,xrot,1,0,0);
		rotate(model,yrot,0,1,0);

		mvp = mvp*model;
		mvp = mvp*proj;
		Mat	worldInverseTranspose = model.inv();
        worldInverseTranspose.t();
        Mat viewInverse = view.inv();
        normalMatrix = (normalMatrix*model).inv().t();

		// update the uniforms and attirbs
		if(uniformLocations.world!=-1)
			glUniformMatrix4fv(uniformLocations.world,1,GL_FALSE,(float *)model.data);
		if(uniformLocations.worldInverseTranspose!=-1)
			glUniformMatrix4fv(uniformLocations.worldInverseTranspose,1,GL_FALSE,(float *)worldInverseTranspose.data);
		if(uniformLocations.worldViewProj!=-1)
			glUniformMatrix4fv(uniformLocations.worldViewProj,1,GL_FALSE,(float *)mvp.data);
		if(uniformLocations.viewInverse!=-1)
			glUniformMatrix4fv(uniformLocations.viewInverse,1,GL_FALSE,(float *)viewInverse.data);
		if(uniformLocations.normalMatrix!=-1)
			glUniformMatrix4fv(uniformLocations.normalMatrix,1,GL_FALSE,(float *)normalMatrix.data);

		double scale_factor_sum = 0.0;
        for (int i = 0; i < 7; i++)
        {
	 	    if(uniformLocations.scaleFactor[i]!=-1)
				glUniform1f(uniformLocations.scaleFactor[i], 0.0);
            
			scale_factor_sum += 0.0;
        } 
		
		GLenum erro = glGetError();
		
		if (erro != GL_NO_ERROR)
		{
			throw runtime_error("GL Error: " + erro);
		}
		 if(uniformLocations.meanScaleFactorLocation!=-1)
			glUniform1f(uniformLocations.meanScaleFactorLocation, 1.0 - scale_factor_sum);

		 GLfloat color[4] ={0.5, 0.65, 1, 1};
         
		 if(uniformLocations.color!=-1)
			glUniform4fv(uniformLocations.color,1,color);
	}

	DrawMeshProgPL(ml);
	//DrawDebugMesh();
	//DrawMesh(ml->getMeshes()[0]);
	glEndList();

	// Init the camera for the new mesh
	mCameraUp = Point3d(0.0f, 0.0f, 1.0f);
	SetupCamera();
	delete ml;
}


// Load a file to the mesh
void GLViewer::LoadFile(const char * aFileName, const char * aOverrideTexture)
{
  // Get the file size
  ifstream f(aFileName, ios::in | ios::binary);

  if(f.fail())
    throw runtime_error("Unable to open the file.");

  f.seekg(0, ios_base::end);
  long tmpFileSize = (long) f.tellg();
  f.close();

  // Load the mesh
  cout << "Loading " << aFileName << "..." << flush;
  mTimer.Push();
  Mesh * newMesh = new Mesh();
  try
  {
    ImportMesh(aFileName, newMesh);
  }
  catch(exception &e)
  {
    delete newMesh;
    throw;
  }
  if(mMesh)
    delete mMesh;

  mMesh = newMesh;
  cout << "done (" << int(mTimer.PopDelta() * 1000.0 + 0.5) << " ms)" << endl;

  // Get the file name (excluding the path), and the path (excluding the file name)
  mFileName = ExtractFileName(string(aFileName));
  mFilePath = ExtractFilePath(string(aFileName));

  // The temporary file size is now the official file size...
  mFileSize = tmpFileSize;

  // Set window title
  string windowCaption = string("Body viewer - ") + mFileName;
  glutSetWindowTitle(windowCaption.c_str());

  // If the file did not contain any normals, calculate them now...
  if(mMesh->mNormals.size() != mMesh->mVertices.size())
  {
    cout << "Calculating normals..." << flush;
    mTimer.Push();
    mMesh->CalculateNormals();
    cout << "done (" << int(mTimer.PopDelta() * 1000.0 + 0.5) << " ms)" << endl;
  }

  // Load the texture
  if(mTexHandle)
    glDeleteTextures(1, &mTexHandle);

  mTexHandle = 0;
  
  if(mMesh->mTexCoords.size() == mMesh->mVertices.size())
  {
    string texFileName = mMesh->mTexFileName;
    if(aOverrideTexture)
      texFileName = string(aOverrideTexture);

    if(texFileName.size() > 0)
      InitTexture(texFileName.c_str());
    else
      InitTexture(0);
  }

  // Setup texture parameters for the shader
  if(mUseShader)
  {
    glUseProgram(mShaderProgram);

    // Set the uUseTexture uniform
    GLint useTexLoc = glGetUniformLocation(mShaderProgram, "uUseTexture");
    if(useTexLoc >= 0)
      glUniform1i(useTexLoc, glIsTexture(mTexHandle));

    // Set the uTex uniform
    GLint texLoc = glGetUniformLocation(mShaderProgram, "uTex");
    if(texLoc >= 0)
      glUniform1i(texLoc, 0);

    glUseProgram(0);
  }

  // Load the mesh into a displaylist
  if(mDisplayList)
    glDeleteLists(mDisplayList, 1);
  mDisplayList = glGenLists(1);
  glNewList(mDisplayList, GL_COMPILE);
  DrawMesh(mMesh);
  glEndList();

  // Init the camera for the new mesh
  mCameraUp = Point3d(0.0f, 0.0f, 1.0f);
  SetupCamera();
}

// Load a texture file
void GLViewer::LoadTexture(const char * aFileName)
{
  // Load the texture
  if(mTexHandle)
    glDeleteTextures(1, &mTexHandle);
  mTexHandle = 0;
  if(mMesh->mTexCoords.size() == mMesh->mVertices.size())
    InitTexture(aFileName);

  // Setup texture parameters for the shader
  if(mUseShader)
  {
    glUseProgram(mShaderProgram);

    // Set the uUseTexture uniform
    GLint useTexLoc = glGetUniformLocation(mShaderProgram, "uUseTexture");
    if(useTexLoc >= 0)
      glUniform1i(useTexLoc, glIsTexture(mTexHandle));

    // Set the uTex uniform
    GLint texLoc = glGetUniformLocation(mShaderProgram, "uTex");
    if(texLoc >= 0)
      glUniform1i(texLoc, 0);

    glUseProgram(0);
  }
}

// Draw an outline box.
void GLViewer::DrawOutlineBox(int x1, int y1, int x2, int y2, float r, float g, float b, float a)
{
  // Draw a blended box
  // Note: We add (1,1) to the (x2,y2) corner to cover the entire pixel range
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glBegin(GL_QUADS);
  glColor4f(r, g, b, 0.7f * a);
  glVertex2i(x1, y1);
  glVertex2i(x2+1, y1);
  glColor4f(r, g, b, 0.7f * a + 0.3f);
  glVertex2i(x2+1, y2+1);
  glVertex2i(x1, y2+1);
  glEnd();
  glDisable(GL_BLEND);

  // Draw a solid outline
  glPushMatrix();
  glTranslatef(0.5f, 0.5f, 0.0f);  // Compensate for 0.5 pixel center offset
  glColor4f(r, g, b, 1.0f);
  glBegin(GL_LINE_LOOP);
  glVertex2i(x1, y1-1);
  glVertex2i(x2, y1-1);
  glVertex2i(x2+1, y1);
  glVertex2i(x2+1, y2);
  glVertex2i(x2, y2+1);
  glVertex2i(x1, y2+1);
  glVertex2i(x1-1, y2);
  glVertex2i(x1-1, y1);
  glEnd();
  glPopMatrix();
}

// Draw a string using GLUT. The string is shown on top of an alpha-blended
// quad.
void GLViewer::DrawString(string aString, int x, int y)
{
  // Calculate the size of the string box
  int x0 = x, y0 = y;
  int x1 = x0, y1 = y0;
  int x2 = x0, y2 = y0;
  for(unsigned int i = 0; i < aString.size(); ++ i)
  {
    int c = (int) aString[i];
    if(c == (int) 10)
    {
      x2 = x;
      y2 += 13;
    }
    else if(c != (int) 13)
    {
      x2 += glutBitmapWidth(GLUT_BITMAP_8_BY_13, c);
      if(x2 > x1) x1 = x2;
    }
  }
  y1 = y2 + 13;

  // Draw a alpha blended box
  DrawOutlineBox(x0-4, y0-3, x1+4, y1+4, 0.3f, 0.3f, 0.3f, 0.6f);

  // Print the text
  glColor3f(1.0f, 1.0f, 1.0f);
  x2 = x;
  y2 = y + 13;
  for(unsigned int i = 0; i < aString.size(); ++ i)
  {
    int c = (int) aString[i];
    if(c == (int) 10)
    {
      x2 = x;
      y2 += 13;
    }
    else if(c != (int) 13)
    {
      glRasterPos2i(x2, y2);
      glutBitmapCharacter(GLUT_BITMAP_8_BY_13, c);
      x2 += glutBitmapWidth(GLUT_BITMAP_8_BY_13, c);
    }
  }
}

// Draw 2D overlay
void GLViewer::Draw2DOverlay()
{
  // Setup the matrices for a width x height 2D screen
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0.0, (double) mWidth, (double) mHeight, 0.0, -1.0, 1.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  // Setup the rendering pipeline for 2D rendering
  glDisable(GL_LIGHTING);
  glDisable(GL_DEPTH_TEST);

  // Render an info string
  if(mMesh)
  {
    stringstream s;
    s << mFileName << " (" << (mFileSize + 512) / 1024 << "KB)" << endl;
    s << mMesh->mVertices.size() << " vertices" << endl;
    s << mMesh->mIndices.size() / 3 << " triangles";
    DrawString(s.str(), 10, mHeight - 50);
  }

  // Calculate buttons bounding box, and draw it as an outline box
  int x1 = 9999, y1 = 9999, x2 = 0, y2 = 0;
  for(list<GLButton *>::iterator b = mButtons.begin(); b != mButtons.end(); ++ b)
  {
    if((*b)->mPosX < x1)
		x1 = (*b)->mPosX;
    
	if(((*b)->mPosX + (*b)->mWidth) > x2) 
		x2 = (*b)->mPosX + (*b)->mWidth;
    
	if((*b)->mPosY < y1) 
		y1 = (*b)->mPosY;

    if(((*b)->mPosY + (*b)->mHeight) > y2) 
		y2 = (*b)->mPosY + (*b)->mHeight;
  }
  DrawOutlineBox(x1-5, y1-5, x2+5, y2+5, 0.3f, 0.3f, 0.3f, 0.6f);

  // Render all the buttons (last = on top)
  for(list<GLButton *>::iterator b = mButtons.begin(); b != mButtons.end(); ++ b)
    (*b)->Redraw();
}

/// Get 3D coordinate under the mouse cursor.
bool GLViewer::WinCoordTo3DCoord(int x, int y, Point3d &aPoint)
{
  // Read back the depth value at at (x, y)
  GLfloat z = 0.0f;
  glReadPixels(x,  mHeight - y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, (GLvoid *) &z);
  if((z > 0.0f) && (z < 1.0f))
  {
    // Convert the window coordinate to space coordinates
    GLdouble objX, objY, objZ;
    gluUnProject((GLdouble) x, (GLdouble) (mHeight - y), (GLdouble) z,  mModelviewMatrix, mProjectionMatrix, mViewport, &objX, &objY, &objZ);
    aPoint = Point3d((float) objX, (float) objY, (float) objZ);
    return true;
  }
  else
    return false;
}

Point3d Normalize(Point3d v)
{
  float len = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
  if(len > 1e-20f)
    len = 1.0f / len;
  else
    len = 1.0f;
  return Point3d(v.x * len, v.y * len, v.z * len);
}

/// Update the focus position of the camera.
void GLViewer::UpdateFocus()
{
  double w = (mTimer.GetTime() - mFocusStartTime) / (mFocusEndTime - mFocusStartTime);
  Point3d dir = Normalize(mCameraPosition - mCameraLookAt);
  if(w < 1.0)
  {
    w = pow(w, 0.2);
    mCameraLookAt = mFocusStartPos + (mFocusEndPos - mFocusStartPos) * w;
    mCameraPosition = mCameraLookAt + dir * (mFocusStartDistance + (mFocusEndDistance - mFocusStartDistance) * w);
  }
  else
  {
    mCameraLookAt = mFocusEndPos;
    mCameraPosition = mCameraLookAt + dir * mFocusEndDistance;
    mFocusing = false;
  }
  glutPostRedisplay();
}


//-----------------------------------------------------------------------------
// Actions (user activated functions)
//-----------------------------------------------------------------------------

/// Open another file
void GLViewer::ActionOpenFile()
{
#if 0
	LoadMale("male");
#else

	std::string	 input = "D:\\Graphics\\3dModels\\inputImage.obj";
	std::string filename = ExtractFileName(input);
	std::string filepath = ExtractFilePath(input);
	
	size_t pathPos = filename.rfind(".");
	std::string justfilename = filename.substr(0,pathPos);

	std::string texFile = filepath+"\\"+justfilename+"_tex.jpeg";
	try
    {
      LoadFile(input.c_str(), texFile.c_str());

      glutPostRedisplay();
    }
    catch(exception &e)
    {
      SysMessageBox mb;
      mb.mMessageType = SysMessageBox::mtError;
      mb.mCaption = "Error";
      mb.mText = string(e.what());
      mb.Show();
    }

#endif
}

/// Save the file
void GLViewer::ActionSaveFile()
{
  if(!mMesh)
  {
    SysMessageBox mb;
    mb.mMessageType = SysMessageBox::mtError;
    mb.mCaption = "Save File";
    mb.mText = string("No mesh has been loaded.");
    mb.Show();

    return;
  }

  SysSaveDialog sd;
  sd.mFilters.push_back(string("Wavefront geometry file (.obj)|*.obj"));
  sd.mFileName = mFileName;

  if(sd.Show())
  {
    try
    {
      Options opt;

      // Do not export normals that do not come from the original file
      if(!mMesh->mOriginalNormals)
        opt.mNoNormals = true;

      // Export the mesh
      ExportMesh(sd.mFileName.c_str(), mMesh, opt);
    }
    catch(exception &e)
    {
      SysMessageBox mb;
      mb.mMessageType = SysMessageBox::mtError;
      mb.mCaption = "Error";
      mb.mText = string(e.what());
      mb.Show();
    }
  }
}

/// Open a texture file
void GLViewer::ActionOpenTexture()
{
  if(!mMesh || (mMesh->mTexCoords.size() < 1))
  {
    SysMessageBox mb;
    mb.mMessageType = SysMessageBox::mtError;
    mb.mCaption = "Open Texture File";
    mb.mText = string("This mesh does not have any texture coordinates.");
    mb.Show();
    return;
  }

  SysOpenDialog od;
  od.mCaption = string("Open Texture File");
  od.mFilters.push_back(string("All supported texture files|*.jpg;*.jpeg;*.png"));
  od.mFilters.push_back(string("JPEG|*.jpg;*.jpeg"));
  od.mFilters.push_back(string("PNG|*.png"));
  if(od.Show())
  {
    try
    {
      LoadTexture(od.mFileName.c_str());
      mMesh->mTexFileName = ExtractFileName(od.mFileName);
      glutPostRedisplay();
    }
    catch(exception &e)
    {
      SysMessageBox mb;
      mb.mMessageType = SysMessageBox::mtError;
      mb.mCaption = "Error";
      mb.mText = string(e.what());
      mb.Show();
    }
  }
}

/// Toggle wire frame view on/off
void GLViewer::ActionToggleWireframe()
{
  if(mPolyMode == GL_LINE)
    mPolyMode = GL_FILL;
  else
    mPolyMode = GL_LINE;
  glutPostRedisplay();
}

/// Fit model to the screen (re-focus)
void GLViewer::ActionFitToScreen()
{
  double now = mTimer.GetTime();
  mFocusStartTime = now;
  mFocusEndTime = now + FOCUS_TIME;
  mFocusStartPos = mCameraLookAt;
  mFocusStartDistance = AbsValue(mCameraLookAt - mCameraPosition);
  mFocusEndPos = (mAABBMax + mAABBMin) * 0.5f;
  mFocusEndDistance = 0.825 * AbsValue(mAABBMax - mAABBMin);
  mFocusing = true;
  UpdateFocus();
  glutPostRedisplay();
}

/// Set camera up direction to Y
void GLViewer::ActionCameraUpY()
{
  mCameraUp = Point3d(0.0f, 1.0f, 0.0f);
  SetupCamera();
  glutPostRedisplay();
}

/// Set camera up direction to Z
void GLViewer::ActionCameraUpZ()
{
  mCameraUp = Point3d(0.0f, 0.0f, 1.0f);
  SetupCamera();
  glutPostRedisplay();
}

/// Zoom camera one step in
void GLViewer::ActionZoomIn()
{
  double now = mTimer.GetTime();
  mFocusStartTime = now;
  mFocusEndTime = now + FOCUS_TIME;
  mFocusStartPos = mCameraLookAt;
  mFocusStartDistance = AbsValue(mCameraLookAt - mCameraPosition);
  mFocusEndPos = mCameraLookAt;
  mFocusEndDistance = (1.0/1.5) * mFocusStartDistance;
  mFocusing = true;
  UpdateFocus();
  glutPostRedisplay();
}

/// Zoom camera one step out
void GLViewer::ActionZoomOut()
{
  double now = mTimer.GetTime();
  mFocusStartTime = now;
  mFocusEndTime = now + FOCUS_TIME;
  mFocusStartPos = mCameraLookAt;
  mFocusStartDistance = AbsValue(mCameraLookAt - mCameraPosition);
  mFocusEndPos = mCameraLookAt;
  mFocusEndDistance = 1.5 * mFocusStartDistance;
  mFocusing = true;
  UpdateFocus();
  glutPostRedisplay();
}

/// Exit program
void GLViewer::ActionExit()
{
  // Note: In freeglut you can do glutLeaveMainLoop(), which is more graceful
  exit(0);
}

/// Show a help dialog
void GLViewer::ActionHelp()
{
  stringstream helpText;
  helpText << "BodyVisualizer TrialRoom ViewTek" << endl;
  helpText << "Copyright (c) 2008-2013 Satish Lakkoju" << endl << endl;
  helpText << "Keyboard actions:" << endl;
  helpText << "  W - Toggle wire frame view on/off" << endl;
  helpText << "  F - Fit model to the screen" << endl;
  helpText << "  Y - Set Y as the up axis (change camera view)" << endl;
  helpText << "  Z - Set Z as the up axis (change camera view)" << endl;
  helpText << "  +/- - Zoom in/out with the camera" << endl;
  helpText << "  ESC - Exit program" << endl << endl;
  helpText << "Mouse control:" << endl;
  helpText << "  Left button - Rotate camera" << endl;
  helpText << "  Middle button or wheel - Zoom camera" << endl;
  helpText << "  Right button - Pan camera" << endl;
  helpText << "  Double click - Focus on indicated surface";

  SysMessageBox mb;
  mb.mMessageType = SysMessageBox::mtInformation;
  mb.mCaption = "Help";
  mb.mText = helpText.str();
  mb.Show();
}


//-----------------------------------------------------------------------------
// GLUT callback functions
//-----------------------------------------------------------------------------


GLuint v,f,f2,p;
float lpos[4] = {1,0.5,1,0};

void changeSize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if(h == 0)
		h = 1;

	float ratio = 1.0* w / h;

	// Reset the coordinate system before modifying
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	// Set the viewport to be the entire window
    glViewport(0, 0, w, h);

	// Set the correct perspective.
	gluPerspective(45,ratio,1,1000);
	glMatrixMode(GL_MODELVIEW);


}
float a = 0;

void renderScene(void) {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	gluLookAt(0.0,0.0,5.0, 
		      0.0,0.0,-1.0,
			  0.0f,1.0f,0.0f);

	glLightfv(GL_LIGHT0, GL_POSITION, lpos);
	glRotatef(a,0,1,1);
	glutSolidTeapot(1);
	a+=0.1;

	glutSwapBuffers();
}

void processNormalKeys(unsigned char key, int x, int y) {

	if (key == 27) 
		exit(0);
}

#define printOpenGLError() printOglError(__FILE__, __LINE__)

int printOglError(char *file, int line)
{
    //
    // Returns 1 if an OpenGL error occurred, 0 otherwise.
    //
    GLenum glErr;
    int    retCode = 0;

    glErr = glGetError();
    while (glErr != GL_NO_ERROR)
    {
        printf("glError in file %s @ line %d: %s\n", file, line, gluErrorString(glErr));
        retCode = 1;
        glErr = glGetError();
    }
    return retCode;
}


void printShaderInfoLog(GLuint obj)
{
    int infologLength = 0;
    int charsWritten  = 0;
    char *infoLog;

	glGetShaderiv(obj, GL_INFO_LOG_LENGTH,&infologLength);

    if (infologLength > 0)
    {
        infoLog = (char *)malloc(infologLength);
        glGetShaderInfoLog(obj, infologLength, &charsWritten, infoLog);
		printf("%s\n",infoLog);
        free(infoLog);
    }
}

void printProgramInfoLog(GLuint obj)
{
    int infologLength = 0;
    int charsWritten  = 0;
    char *infoLog;

	glGetProgramiv(obj, GL_INFO_LOG_LENGTH,&infologLength);

    if (infologLength > 0)
    {
        infoLog = (char *)malloc(infologLength);
        glGetProgramInfoLog(obj, infologLength, &charsWritten, infoLog);
		printf("%s\n",infoLog);
        free(infoLog);
    }
}




char *textFileRead(char *fn) {


	FILE *fp;
	char *content = NULL;

	int count=0;

	if (fn != NULL) {
		fp = fopen(fn,"rt");

		if (fp != NULL) {
      
      fseek(fp, 0, SEEK_END);
      count = ftell(fp);
      rewind(fp);

			if (count > 0) {
				content = (char *)malloc(sizeof(char) * (count+1));
				count = fread(content,sizeof(char),count,fp);
				content[count] = '\0';
			}
			fclose(fp);
		}
	}
	return content;
}

int textFileWrite(char *fn, char *s) {

	FILE *fp;
	int status = 0;

	if (fn != NULL) {
		fp = fopen(fn,"w");

		if (fp != NULL) {
			
			if (fwrite(s,sizeof(char),strlen(s),fp) == strlen(s))
				status = 1;
			fclose(fp);
		}
	}
	return(status);
}




void setShaders() {

	char *vs = NULL,*fs = NULL,*fs2 = NULL;

	v = glCreateShader(GL_VERTEX_SHADER);
	f = glCreateShader(GL_FRAGMENT_SHADER);
	f2 = glCreateShader(GL_FRAGMENT_SHADER);

	vs = textFileRead("minimal.vert");
	fs = textFileRead("minimal.frag");

	const char * vv = vs;
	const char * ff = fs;

	glShaderSource(v, 1, &vv,NULL);
	glShaderSource(f, 1, &ff,NULL);

	free(vs);free(fs);

	glCompileShader(v);
	glCompileShader(f);

	printShaderInfoLog(v);
	printShaderInfoLog(f);
	printShaderInfoLog(f2);

	p = glCreateProgram();
	glAttachShader(p,v);
	glAttachShader(p,f);

	glLinkProgram(p);
	printProgramInfoLog(p);

	glUseProgram(p);

}


/// Redraw function.
void GLViewer::WindowRedraw_temp(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	gluLookAt(0.0,0.0,5.0, 
		      0.0,0.0,-1.0,
			  0.0f,1.0f,0.0f);

	glLightfv(GL_LIGHT0, GL_POSITION, lpos);
	glRotatef(a,0,1,1);
	glutSolidTeapot(1);
	a+=0.1;

	glutSwapBuffers();
}
void GLViewer::WindowRedraw(void)
{
  // Get buffer properties
  glGetIntegerv(GL_DEPTH_BITS, &mDepthBufferResolution);

  // Set the viewport to be the entire window
  glViewport(0, 0, mWidth, mHeight);

  // Clear the buffer(s)
  glClear(GL_DEPTH_BUFFER_BIT);

  // Draw a gradient background
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glDisable(GL_LIGHTING);
  glDisable(GL_DEPTH_TEST);
  glBegin(GL_QUADS);
  glColor3f(0.4f, 0.5f, 0.7f);
  glVertex3f(-1.0f, -1.0f, 0.5f);
  glColor3f(0.3f, 0.4f, 0.7f);
  glVertex3f(1.0f, -1.0f, 0.5f);
  glColor3f(0.1f, 0.1f, 0.2f);
  glVertex3f(1.0f, 1.0f, 0.5f);
  glColor3f(0.1f, 0.15f, 0.24f);
  glVertex3f(-1.0f, 1.0f, 0.5f);
  glEnd();

  // Calculate screen ratio (width / height)
  float ratio;
  if(mHeight == 0)
    ratio = 1.0f;
  else
    ratio = (float) mWidth / (float) mHeight;

  // Calculate optimal near and far Z clipping planes
  float farZ = AbsValue(mAABBMax - mAABBMin) + AbsValue(mCameraPosition - mCameraLookAt);

  if(farZ < 1e-20f)
    farZ = 1e-20f;
  
  float nearZ;

  if(mDepthBufferResolution >= 24)
    nearZ = 0.0001f * farZ;
  else
    nearZ = 0.01f * farZ;

  // Set up perspective projection
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(60.0f, ratio, nearZ, farZ);

  // Set up the camera modelview matrix
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(mCameraPosition.x, mCameraPosition.y, mCameraPosition.z, mCameraLookAt.x, mCameraLookAt.y, mCameraLookAt.z, mCameraUp.x, mCameraUp.y, mCameraUp.z);
  
  // Read back camera matrices
  glGetDoublev(GL_MODELVIEW_MATRIX, mModelviewMatrix);
  glGetDoublev(GL_PROJECTION_MATRIX, mProjectionMatrix);
  glGetIntegerv(GL_VIEWPORT, mViewport);

  // Set up the lights
  SetupLighting();

  // Enable material shader
  if(mUseShader)
    glUseProgram(mShaderProgram);
  else
    glEnable(GL_LIGHTING);

  // Draw the mesh
  SetupMaterial();
  glEnable(GL_DEPTH_TEST);
  glPolygonMode(GL_FRONT_AND_BACK, mPolyMode);
  if(mTexHandle)
  {
    glBindTexture(GL_TEXTURE_2D, mTexHandle);
    glEnable(GL_TEXTURE_2D);
    glColor3f(1.0f, 1.0f, 1.0f);
  }
  else
    glColor3f(0.9f, 0.86f, 0.7f);

  if(mDisplayList)
    glCallList(mDisplayList);

  glDisable(GL_TEXTURE_2D);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  // Disable material shader
  if(mUseShader)
    glUseProgram(0);
  else
    glDisable(GL_LIGHTING);

  // Draw 2D overlay (information text etc)
  Draw2DOverlay();
  // Swap buffers
  glutSwapBuffers();

  // Focusing?
  if(mFocusing)
  {
    UpdateFocus();
    glutPostRedisplay();
  }
}

/// Resize function.
void GLViewer::WindowResize(int w, int h)
{
  // Store the new window size
  mWidth = w;
  mHeight = h;
}

/// Mouse click function
void GLViewer::MouseClick(int button, int state, int x, int y)
{
  bool clickConsumed = false;
  if(button == GLUT_LEFT_BUTTON)
  {
    // Check if any of the GUI buttons were clicked
    for(list<GLButton *>::iterator b = mButtons.begin(); b != mButtons.end(); ++ b)
    {
      if((*b)->MouseClick(state, x, y))
        clickConsumed = true;
    }
    if(!clickConsumed)
    {
      if(state == GLUT_DOWN)
      {
        double now = mTimer.GetTime();
        if((now - mLastClickTime) < DOUBLE_CLICK_TIME)
        {
          // Double click occured
          Point3d mouseCoord3D;
          if(WinCoordTo3DCoord(x, y, mouseCoord3D))
          {
            mFocusStartTime = now;
            mFocusEndTime = now + FOCUS_TIME;
            mFocusStartPos = mCameraLookAt;
            mFocusEndPos = mouseCoord3D;
            mFocusStartDistance = AbsValue(mCameraLookAt - mCameraPosition);
            mFocusEndDistance = mFocusStartDistance;
            mFocusing = true;
          }
          mLastClickTime = -1000.0;
        }
        else
        {
          // Single click occured
          mMouseRotate = true;
          mLastClickTime = now;
        }
      }
      else if(state == GLUT_UP)
        mMouseRotate = false;
    }
  }
  else if(button == GLUT_MIDDLE_BUTTON)
  {
    if(state == GLUT_DOWN)
      mMouseZoom = true;
    else if(state == GLUT_UP)
      mMouseZoom = false;
  }
  else if(button == GLUT_RIGHT_BUTTON)
  {
    if(state == GLUT_DOWN)
      mMousePan = true;
    else if(state == GLUT_UP)
      mMousePan = false;
  }
  else if(button == 3) // Mouse wheel up on some systems
  {
    if(state == GLUT_DOWN)
      ActionZoomIn();
  }
  else if(button == 4) // Mouse wheel down on some systems
  {
    if(state == GLUT_DOWN)
      ActionZoomOut();
  }

  mOldMouseX = x;
  mOldMouseY = y;

  // Focusing?
  if(mFocusing)
  {
    UpdateFocus();
    glutPostRedisplay();
  }
}

/// Mouse move function
void GLViewer::MouseMove(int x, int y)
{
  bool needsRedraw = false;

  float deltaX = (float) x - (float) mOldMouseX;
  float deltaY = (float) y - (float) mOldMouseY;
  mOldMouseX = x;
  mOldMouseY = y;

  if(mMouseRotate)
  {
    // Calculate delta angles
    float scale = 3.0f;
    if(mHeight > 0)
      scale /= (float) mHeight;
    float deltaTheta = -scale * deltaX;
    float deltaPhi = -scale * deltaY;

    // Adjust camera angles
    Point3d viewVector = mCameraPosition - mCameraLookAt;
    float r = sqrtf(viewVector.x * viewVector.x + viewVector.y * viewVector.y + viewVector.z * viewVector.z);
    float phi, theta;

    if(r > 1e-20f)
    {
      if(mCameraUp.z > 0.0f)
      {
        phi = acosf(viewVector.z / r);
        theta = atan2f(viewVector.y, viewVector.x);
      }
      else
      {
        phi = acosf(viewVector.y / r);
        theta = atan2f(-viewVector.z, viewVector.x);
      }
    }
    else
    {
      if(mCameraUp.z > 0.0f)
        phi = viewVector.z > 0.0f ? 0.05f * PI : 0.95f * PI;
      else
        phi = viewVector.y > 0.0f ? 0.05f * PI : 0.95f * PI;
      theta = 0.0f;
    }
    phi += deltaPhi;
    theta += deltaTheta;
    if(phi > (0.95f * PI))
      phi = 0.95f * PI;
    else if(phi < (0.05f * PI))
      phi = 0.05f * PI;

    // Update the camera position
    if(mCameraUp.z > 0.0f)
    {
      viewVector.x = r * cos(theta) * sin(phi);
      viewVector.y = r * sin(theta) * sin(phi);
      viewVector.z = r * cos(phi);
    }
    else
    {
      viewVector.x = r * cos(theta) * sin(phi);
      viewVector.y = r * cos(phi);
      viewVector.z = -r * sin(theta) * sin(phi);
    }
    mCameraPosition = mCameraLookAt + viewVector;

    needsRedraw = true;
  }
  else if(mMouseZoom)
  {
    // Calculate delta angles
    float scale = 3.0f;
    if(mHeight > 0)
      scale /= (float) mHeight;
    float zoom = scale * deltaY;

    // Adjust camera zoom
    Point3d viewVector = mCameraPosition - mCameraLookAt;
    viewVector = viewVector * powf(2.0f, zoom);

    // Update the camera position
    mCameraPosition = mCameraLookAt + viewVector;

    needsRedraw = true;
  }
  else if(mMousePan)
  {
    // Calculate delta movement
    float scale = 1.0f * AbsValue(mCameraPosition - mCameraLookAt);

    if(mHeight > 0)
      scale /= (float) mHeight;
    
	float panX = scale * deltaX;
    float panY = scale * deltaY;

    // Calculate camera movement
    Point3d viewDir = norm(mCameraPosition - mCameraLookAt);
	Point3d rightDir = norm(viewDir.cross( mCameraUp));
	Point3d upDir = norm(rightDir.cross(viewDir));
    Point3d moveDelta = rightDir * panX + upDir * panY;

    // Update the camera position
    mCameraPosition += moveDelta;
    mCameraLookAt += moveDelta;

    needsRedraw = true;
  }
  else
  {
    // Call mouse move for all the GUI buttons
    for(list<GLButton *>::iterator b = mButtons.begin(); b != mButtons.end(); ++ b)
    {
      if((*b)->MouseMove(x, y))
        needsRedraw = true;
    }
  }

  // Redraw?
  if(needsRedraw)
    glutPostRedisplay();
}

/// Keyboard function
void GLViewer::KeyDown(unsigned char key, int x, int y)
{
  if(key == 15)       // CTRL+O
    ActionOpenFile();
  else if(key == 19)  // CTRL+S
    ActionSaveFile();
  else if(key == 'w')
    ActionToggleWireframe();
  else if(key == 'f')
    ActionFitToScreen();
  else if(key == 'y')
    ActionCameraUpY();
  else if(key == 'z')
    ActionCameraUpZ();
  else if(key == '+')
    ActionZoomIn();
  else if(key == '-')
    ActionZoomOut();
  else if(key == 27)  // ESC
    ActionExit();
}

/// Keyboard function (special keys)
void GLViewer::SpecialKeyDown(int key, int x, int y)
{
  if(key == GLUT_KEY_F1)
    ActionHelp();
}


//-----------------------------------------------------------------------------
// Application main code
//-----------------------------------------------------------------------------

/// Constructor
GLViewer::GLViewer()
{
  // Clear internal state
  mFileName = "";
  mFilePath = "";
  mFileSize = 0;
  mWidth = 1;
  mHeight = 1;
  mDepthBufferResolution = 16;
  mOldMouseX = 0;
  mOldMouseY = 0;
  mMouseRotate = false;
  mMouseZoom = false;
  mMousePan = false;
  mCameraUp = Point3d(0.0f, 0.0f, 1.0f);
  mFocusStartPos = Point3d(0.0f, 0.0f, 0.0f);
  mFocusEndPos = Point3d(0.0f, 0.0f, 0.0f);
  mFocusStartTime = 0.0;
  mFocusEndTime = 0.0;
  mFocusStartDistance = 1.0;
  mFocusEndDistance = 1.0;
  mFocusing = false;
  mLastClickTime = -1000.0;
  mDisplayList = 0;
  mPolyMode = GL_FILL;
  mTexHandle = 0;
  mUseShader = false;
  mShaderProgram = 0;
  mVertShader = 0;
  mFragShader = 0;
  number_of_offset_meshes = 0;
  mMesh = NULL;
}

/// Destructor
GLViewer::~GLViewer()
{
  // Free all GUI buttons
  for(list<GLButton *>::iterator b = mButtons.begin(); b != mButtons.end(); ++ b)
    delete (*b);

  // Free the mesh
  if(mMesh)
    delete mMesh;
}

/// Run the application
void GLViewer::Run(int argc, char **argv)
{
  try
  {
    // Init GLUT
    glutInit(&argc, argv);
	// On Linux, there is currently a bug in OpenCV that returns 
	// zero for both width and height here (at least for video from file)
	// hence the following override to global variable defaults: 
	mWidth = 640;
	mHeight = 480;

    // Create the glut window
    glutInitWindowSize(mWidth, mHeight);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow("BodyVisualizer-ViewTek");

    // Init GLEW (for OpenGL 2.x support)
    if(glewInit() != GLEW_OK)
      throw runtime_error("Unable to initialize GLEW.");

    // Load the phong shader, if we can
	if(GLEW_VERSION_2_0)
	{
		InitShader();
		glUseProgram(mShaderProgram);	
		//glUseProgram(0);
		//setShaders();
	}
    else if(GLEW_VERSION_1_2)
	{
      glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);
	}

    // Set the GLUT callback functions (these are bridged to the corresponding
    // class methods)
#if 1
    glutReshapeFunc(GLUTWindowResize);
    glutDisplayFunc(GLUTWindowRedraw);
    glutMouseFunc(GLUTMouseClick);
    glutMotionFunc(GLUTMouseMove);
    glutPassiveMotionFunc(GLUTMouseMove);
    glutKeyboardFunc(GLUTKeyDown);
    glutSpecialFunc(GLUTSpecialKeyDown);
#else
	
	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene);
	glutReshapeFunc(changeSize);
	glutKeyboardFunc(processNormalKeys);
#endif

    // Create GUI buttons
    GLButton * b1 = new OpenButton();
    mButtons.push_back(b1);
    b1->mParent = this;
    b1->SetGlyph(icon_open, 32, 32, 4);
    b1->mPosX = 12;
    b1->mPosY = 10;

    GLButton * b2 = new SaveButton();
    mButtons.push_back(b2);
    b2->mParent = this;
    b2->SetGlyph(icon_save, 32, 32, 4);
    b2->mPosX = 60;
    b2->mPosY = 10;
    
	GLButton * b3 = new OpenTextureButton();
    mButtons.push_back(b3);
    b3->mParent = this;
    b3->SetGlyph(icon_texture, 32, 32, 4);
    b3->mPosX = 108;
    b3->mPosY = 10;
    
	GLButton * b4 = new HelpButton();
    mButtons.push_back(b4);
    b4->mParent = this;
    b4->SetGlyph(icon_help, 32, 32, 4);
    b4->mPosX = 156;
    b4->mPosY = 10;

	// male female body models
	GLButton * b5 = new maleButton();
    mButtons.push_back(b5);
    b5->mParent = this;
    b5->SetGlyph(icon_open, 32, 32, 4);
    b5->mPosX = 204;
    b5->mPosY = 10;

	GLButton * b6 = new femaleButton();
    mButtons.push_back(b6);
    b6->mParent = this;
    b6->SetGlyph(icon_open, 32, 32, 4);
    b6->mPosX = 252;
    b6->mPosY = 10;


    // Load the file
    if(argc >= 2)
    {
      const char * overrideTexName = NULL;
      if(argc >= 3)
        overrideTexName = argv[2];
      LoadFile(argv[1], overrideTexName);
    }

    // Enter the main loop
    glutMainLoop();
  }
  catch(ctm_error &e)
  {
    SysMessageBox mb;
    mb.mMessageType = SysMessageBox::mtError;
    mb.mCaption = "Error";
    mb.mText = string("OpenCTM error: ") + string(e.what());
    mb.Show();
  }
  catch(exception &e)
  {
    SysMessageBox mb;
    mb.mMessageType = SysMessageBox::mtError;
    mb.mCaption = "Error";
    mb.mText = string(e.what());
    mb.Show();
  }
  cout << endl;
}
