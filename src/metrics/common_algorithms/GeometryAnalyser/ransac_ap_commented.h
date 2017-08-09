#pragma once
#include "vec_pair.h"
#include <opencv2/core/core.hpp>
#include <stdlib.h> 
#include <time.h>

//детерминация рандомайзера!
//короче, чтоб и рандом был, и чтоб на разных компах
//результат совпадал. генерим 30000 рандомных чисел 
//и используем для RANSAC. 
#define RANDOM_SIZE 30000
//seed для начала генерации рандома
#define SEED 1350
//сколько векторов выбирать 
//(сколько идет на анализ на каждой итерации)
#define INPUT_SIZE 3
//порог, до которого считается, что вектор
//подошел к найденному преобразованию
#define TR_ERROR 0.55f
//сколько итераций фигачить
#define ITERATIONS 10000
//магические константы для бредовой фильтрации. переделать, но не ухудшить!
#define TR_DYN 0.8
#define TR_FIX 30

//минимальное количество векторов для RANSAC.
#define TR_CONF 50

//что анализируем, господа?
enum TRANSFORM_TYPE { GEOMETRY = 0, POSITION = 1 };

//шняга для представления аффинного преобразования

struct aTransform
{
	double a; //rotation parameter
	double b; //scaling parameter
	double c; //shift parameter
	double z; //used for position, was also used for perspective
};

//он тут главный
class Ransac
{
public:
	Ransac(); //создавашка
	~Ransac(); //разрушашка
	void Init(int type = GEOMETRY); //инициализирует все массивы (в т.ч. и рандомайзер)
	void Load(vecPairArray src); //сюда загружать набор векторов
	int Analyse(); //запустить анализ
	int GetTotal(); //сколько всего есть векторов
	int GetFit(); //сколько векторов подошло к найденным параметрам
	aTransform Result();
private:
	vecPairArray m_array; //массив информации о векторах (вход)
	int* random_array; //детерминация рандомизатора. тупо массив рандомных чисел
	int m_type; //тип анализа. геометрия / позиция
	int random_count; //сколько рандомных чисел имеем
	int randomIndex(); //берет следующее рандомное число
	aTransform m_transform; //здесь наилучшее найденное преобразование будет.
	void makeRandomArray(vecPairArray &src); //сгенерить кучу рандомных чиселок
	inline double findError(aTransform trm, vecPair pair); //оценить, насколько не влез конкретный вектор в это преобразование
	int findInliers(aTransform trm); //найти сколько векторов влезло по порогу
	aTransform findTransform(vecPairArray src); //вычислить преобразование по векторам (решить СЛАУ по Гауссу)
};