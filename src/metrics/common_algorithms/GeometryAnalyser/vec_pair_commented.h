#pragma once

//структура с инфой о векторе между парой точек
struct vecPair
{
	double sX; //начало, X
	double sY; //начало, Y
	double vX; //длина,  X
	double vY; //длина,  Y
	double mX; //глубина (для анализа 2D)
};

//массив вышеуказанных шняг
struct vecPairArray
{
	int count; //сколько их тут
	vecPair* array;
};