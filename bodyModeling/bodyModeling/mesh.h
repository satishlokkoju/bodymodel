#ifndef _MESH_H_
#define _MESH_H_
#include "commonBM.h"

class Mesh
{
private:
public:
	void test();
	void Clear();
	bool HasTexCoords();

	bool HasNormals();
	
	String mComment;
	Mesh()
	{

	}
	Mesh(vector<Point3f> verts,vector<Point3i> faces, vector<Point2f> texts,vector<int>indices);

	~Mesh()
	{

	}

	void CalculateNormals();
	void BoundingBox(Point3d &aMin, Point3d &aMax);
	void createVertexNormals();

	string mTexFileName;
	bool mOriginalNormals;

	vector<Point3f> mVertices;
	vector<Point3f> mNormals;
	vector<int> mIndices;
	vector<Point3i> Faces;
	
	vector<Point3f> scaled_normals;
	vector<Point3f> vertex_normals;
	
	vector<Point2f> mTexCoords;
	vector<Point3f> mColors;
};


#endif