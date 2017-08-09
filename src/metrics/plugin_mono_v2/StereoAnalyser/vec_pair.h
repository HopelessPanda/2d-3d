#pragma once
struct vecPair
{
	float sX;
	float sY;
	float vX;
	float vY;
	float mX;
};

struct vecPairArray
{
	int count;
	vecPair* array;
};