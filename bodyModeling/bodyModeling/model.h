#ifndef _MODEL_H_
#define _MODEL_H_

#include "common.h"
#include "mesh.h"

class model
{
private:
	vector<Triangle> tri;
	vector<Point3f> scaledNormal;
	vector<Point3f> dScaledNormal;
	Point3f Cross(Triangle A)
	{
		Point3f tempa;
		Point3f tempb;

		tempa = A.VertB-  A.VertA;
		tempb = A.VertC - A.VertA;

		return tempa.cross(tempb);
	}

public:
	void createNormalModel();
	void setColor(Point3d in);
	model(vector<Mesh *> in)
	{
		
	}
	model()
	{
	}
	~model()
	{

	}

};

#endif