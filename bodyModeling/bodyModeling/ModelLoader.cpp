/*---------------------------------------------------------*/ 
/*                    <copyright notice>                   */ 
/*                                                         */
/*	The Code is proprietary to Viewtek Solutions Pvt. Ltd. */ 
/*	   Mail to satish@viewtek.in for any clarifications.   */ 
/*                                                         */ 
/*			<Copyright (C) 2013 Satish Lakkoju.>           */ 
/*				   <All Rights Reserved>                   */ 
/*---------------------------------------------------------*/ 


#include "stdio.h"
#include "modelloader.h"
#include "mean_male.h"
#include "mean_female.h"


#define SIZEOFVER 10777
#define SIZEOFFACE 21550

void modelloader::createMesh(std::string name)
{
	vector<Point3f> vertices;
	vector<Point3i> faces;
	vector<Point2f> tex;
	vector<int> tempmIndices;

	if(name.compare("male"))
	{
		for(int i=0;i<SIZEOFVER ;i++)
		{
			vertices.push_back(Point3f(mean_vertices[i][0],mean_vertices[i][1],mean_vertices[i][2]));
			tex.push_back(Point2f(0,0));
		}
		
		for(int i=0;i<SIZEOFFACE ;i++)
		{
			faces.push_back(Point3i(mean_faces[i][0],mean_faces[i][1],mean_faces[i][2]));
			tempmIndices.push_back(mean_faces[i][0]);
			tempmIndices.push_back(mean_faces[i][1]);
			tempmIndices.push_back(mean_faces[i][2]);
		}
	}
	else
	{
		for(int i=0;i<SIZEOFVER ;i++)
		{
			vertices.push_back(Point3f(femalemean_vertices[i][0],femalemean_vertices[i][1],femalemean_vertices[i][2]));
			tex.push_back(Point2f(0,0));
		}
		
		for(int i=0;i<SIZEOFFACE ;i++)
		{
			faces.push_back(Point3i(femalemean_faces[i][0],femalemean_faces[i][1],femalemean_faces[i][2]));
			tempmIndices.push_back(femalemean_faces[i][0]);
			tempmIndices.push_back(femalemean_faces[i][1]);
			tempmIndices.push_back(femalemean_faces[i][2]);
		}
	}
	Mesh *mesh = new Mesh(vertices,faces,tex,tempmIndices);
	allmeshes.push_back(mesh);

	//
}

void modelloader::createCurrentModel()
{
	currentModel = model(allmeshes);
}


model modelloader::getCurrentModel()
{
	return currentModel;
}

void modelloader::setCurrentModelColor(Point3d in)
{
	currentModel.setColor(in);
}

vector<Mesh *> modelloader::getMeshes()
{
	return allmeshes;
}