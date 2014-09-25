#include "model.h"

void model::createNormalModel()
{
	for(unsigned int i =0; i < tri.size(); i++)
	{
		scaledNormal.push_back(Cross(tri[i]));
	}
}

void model::setColor(Point3d in)
{

}