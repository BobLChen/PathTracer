#pragma once

#include <iostream>

#include "math/Vector2.h"
#include "math/Vector3.h"
#include "math/Vector4.h"

/***********************************************************************************
	Created:	17:9:2002
	FileName: 	hdrloader.h
	Author:		Igor Kravtchenko
	
	Info:		Load HDR image and convert to a set of float32 RGB triplet.
************************************************************************************/

/* 
	This is modified version of the original code. Addeed code to build marginal & conditional densities for IBL importance sampling
*/

class HDRData 
{
public:
	HDRData() 
		: width(0)
		, height(0)
		, cols(nullptr)
		, marginalDistData(nullptr)
		, conditionalDistData(nullptr) 
	{

	}

	~HDRData() 
	{ 
		if (cols) 
		{
			delete cols; 
			cols = nullptr;
		}
		
		if (marginalDistData) 
		{
			delete marginalDistData; 
			marginalDistData = nullptr;
		}
		
		if (conditionalDistData) 
		{
			delete conditionalDistData; 
			conditionalDistData = nullptr;
		}
	}

	int width, height;
	// each pixel takes 3 float32, each component can be of any value...
	float* cols;
	Vector2* marginalDistData;    // y component holds the pdf
	Vector2* conditionalDistData; // y component holds the pdf
};

class HDRLoader 
{
private:
	static void BuildDistributions(HDRData* res);
public:
	static HDRData* Load(const char *fileName);
};
