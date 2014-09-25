#ifndef _MODEL_VIEWER_H_
#define _MODEL_VIEWER_H_

#include "common.h"
#include "model.h"
#include "vertexShader.h"

class model_viewer
{
public:
	model_viewer()
	{
	}
	
	model_viewer(vector<model> in)
	{
	}
	
	void initgl();
	//void drawAxes(float length);
	int run();
	void setModel(model in);
	void startViewer();
private:
	int width;
	int height;

};

#endif