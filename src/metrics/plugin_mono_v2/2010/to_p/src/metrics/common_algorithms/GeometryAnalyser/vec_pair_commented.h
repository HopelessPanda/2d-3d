#pragma once

//��������� � ����� � ������� ����� ����� �����
struct vecPair
{
	double sX; //������, X
	double sY; //������, Y
	double vX; //�����,  X
	double vY; //�����,  Y
	double mX; //������� (��� ������� 2D)
};

//������ ������������� ����
struct vecPairArray
{
	int count; //������� �� ���
	vecPair* array;
};