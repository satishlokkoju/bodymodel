#include "Mesh.h"

void Mesh::test()
{

}

Mesh::Mesh(vector<Point3f> verts,vector<Point3i> faces, vector<Point2f> texts,vector<int>indices)
{
	mVertices = verts;
	Faces = faces;
	mTexCoords = texts;
	mIndices = indices;

	CalculateNormals();
	createVertexNormals();
}


void Mesh::CalculateNormals()
{
	mNormals.resize(mVertices.size());
	for(unsigned int i =0;i<Faces.size();i++)
	{
        Point3i  t = Faces[i];
		
		Point3f a = mVertices[t.y]-mVertices[t.x];
		Point3f b = mVertices[t.z]-mVertices[t.y];
		
		Point3f c = a.cross(b);
		scaled_normals.push_back(c);
    // ...and add it to all three triangle vertices' smooth normals
		for(unsigned int j = 0; j < 3; ++ j)
		{
			 //mNormals[mIndices[i * 3 + j]] += c;
		}
		
		mNormals[t.x] += c;
		mNormals[t.y] += c;
		mNormals[t.z] += c;
		
		//printf("faces: %d\n",i);
	}
}

void Mesh::createVertexNormals()
{
	vector<Point3f> tempVerNormals(mVertices.size());

	for(int i =0;i<Faces.size();i++)
	{
		int a = Faces[i].x;	// 3 vertices of the face
		int b = Faces[i].y;
		int c = Faces[i].z;
		
		tempVerNormals[a].x += scaled_normals[i].x;
		tempVerNormals[a].y += scaled_normals[i].y;
		tempVerNormals[a].z += scaled_normals[i].z;
		
		tempVerNormals[b].x += scaled_normals[i].x;
		tempVerNormals[b].y += scaled_normals[i].y;
		tempVerNormals[b].z += scaled_normals[i].z;
		
		tempVerNormals[c].x += scaled_normals[i].x;
		tempVerNormals[c].y += scaled_normals[i].y;
		tempVerNormals[c].z += scaled_normals[i].z;
	}
	vertex_normals = tempVerNormals;
}
bool Mesh::HasTexCoords()
{

	return false;
}
bool Mesh::HasNormals()
{
	return false;
}
void Mesh::Clear()
{

}
/* */

/// Calculate the bounding box for the mesh
void Mesh::BoundingBox(Point3d &aMin, Point3d &aMax)
{
  if(mVertices.size() < 1)
    aMin = aMax = Point3d(0.0f, 0.0f, 0.0f);
  else
    aMin = aMax = mVertices[0];
  for(unsigned int i = 1; i < mVertices.size(); ++ i)
  {
    if(mVertices[i].x < aMin.x)
      aMin.x = mVertices[i].x;
    else if(mVertices[i].x > aMax.x)
      aMax.x = mVertices[i].x;
    if(mVertices[i].y < aMin.y)
      aMin.y = mVertices[i].y;
    else if(mVertices[i].y > aMax.y)
      aMax.y = mVertices[i].y;
    if(mVertices[i].z < aMin.z)
      aMin.z = mVertices[i].z;
    else if(mVertices[i].z > aMax.z)
      aMax.z = mVertices[i].z;
  }
}