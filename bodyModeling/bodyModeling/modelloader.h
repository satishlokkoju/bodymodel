#ifndef _modelloader_H_
#define _modelloader_H_

#include "common.h"
#include "model.h"
#include "mesh.h"
class modelloader
{

public:
	modelloader()
	{
	 //mean = new model;
	}
	modelloader(vector<Mesh *> allmeshesin)
	{
		allmeshes = allmeshesin;
	 //mean = new model;
	}
	~modelloader()
	{
	 //mean = new model;
	}
	void setModelViewer();
	void setShapeDir();
	void createMesh(std::string name);
	void createCurrentModel();
	model getCurrentModel();
	void setCurrentModelColor(Point3d in);
	vector<Mesh *> getMeshes();
private:
	//Function StartModelViewerFunction;
	string shapeData;
	vector<float> means;
	vector<float> covariances;
	model currentModel;
	vector<Mesh *> allmeshes;
};

#endif