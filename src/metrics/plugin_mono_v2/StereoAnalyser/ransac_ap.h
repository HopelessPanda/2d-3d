#pragma once
#include "vec_pair.h"
#include <opencv2/core/core.hpp>
#include <stdlib.h> 
#include <time.h>
#define RANDOM_SIZE 50000
#define SEED 1350
#define INPUT_SIZE 3
#define TR_ERROR 0.55f
#define ITERATIONS 10000
#define TR_DYN 0.8
#define TR_FIX 30
#define TR_CONF 50

enum TRANSFORM_TYPE {GEOMETRY = 0, POSITION = 1};

struct aTransform
{
	float a;
	float b;
	float c;
	float z;
};

class Ransac
{
public:
	Ransac();
	~Ransac();
	void Init(int type = GEOMETRY);
	void Load(vecPairArray src);
	int Analyse();
	int GetTotal();
	int GetFit();
	aTransform Result();
private:
	vecPairArray m_array;
	int* random_array;
	int m_type;
	int random_count;
	int randomIndex();
	aTransform m_transform;
	void makeRandomArray(vecPairArray &src);
	inline float findError(aTransform trm, vecPair pair);
	int findInliers(aTransform trm);
	aTransform findTransform(vecPairArray src);
};