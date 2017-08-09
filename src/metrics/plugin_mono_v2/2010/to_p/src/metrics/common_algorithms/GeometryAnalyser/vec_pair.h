#pragma once
struct vecPair
{
	double sX;
	double sY;
	double vX;
	double vY;
	double mX;
};

struct vecPairArray
{
	int count;
	vecPair* array;
};