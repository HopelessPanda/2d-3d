#include "ransac_ap.h"

Ransac::Ransac()
{
	m_type = GEOMETRY;
	random_array = NULL;
}

Ransac::~Ransac()
{
	if (random_array != NULL)
		free(random_array);
}

void Ransac::Init(int type)
{
	m_type = type;
	random_array = (int*)malloc(RANDOM_SIZE*sizeof(int));
	random_count = 0;
	srand(SEED);
	for (int i = 0; i < RANDOM_SIZE; i++)
		random_array[i] = rand();
	m_transform.a = m_transform.c = m_transform.z = 0.0f;
	m_transform.b = 1.0f;
}

void Ransac::Load(vecPairArray src)
{
	m_array = src;
}

int Ransac::randomIndex()
{
	return random_array[(random_count++) % RANDOM_SIZE] % m_array.count;
}

void Ransac::makeRandomArray(vecPairArray &src)
{
	for (int i = 0; i < INPUT_SIZE; i++)
		src.array[i] = m_array.array[randomIndex()];
	src.count = INPUT_SIZE;
}

aTransform Ransac::findTransform(vecPairArray src)
{
	aTransform res = { 0 };
	res.b = 1.0;
	if (src.count < 3)
		return res;

	cv::Mat matrix(3, 3, CV_32F);
	cv::Mat matrix_inv(3, 3, CV_32F);
	cv::Mat column(3, 1, CV_32F);
	cv::Mat result(3, 1, CV_32F);

	for (int i = 0; i < 3; i++) 
	{
		float z = m_type == GEOMETRY ? src.array[i].sY + src.array[i].vY / 4.0f : src.array[i].sX + src.array[i].vX / 4.0f;
		matrix.at<float>(i, 0) = src.array[i].sX;
		if (m_type == GEOMETRY)
			matrix.at<float>(i, 1) = src.array[i].sY;
		else
			matrix.at<float>(i, 1) = src.array[i].mX;
		matrix.at<float>(i, 2) = 1.0f;
		column.at<float>(i) = z;
		result.at<float>(i) = 0;
	}
	cv::invert(matrix, matrix_inv);
	result = matrix_inv*column;

	if (m_type == GEOMETRY)
	{
		res.a = result.at<float>(0);
		res.b = result.at<float>(1);
		res.c = result.at<float>(2);
		res.z = 0;
	}
	else
	{
		res.b = result.at<float>(0);
		res.z = result.at<float>(1);
		res.c = result.at<float>(2);
		res.a = 0;
	}

	return res;
}

float Ransac::findError(aTransform trm, vecPair pair)
{
	if (m_type == GEOMETRY)
		return abs(pair.sY + pair.vY / 4.0f - (pair.sX * trm.a + pair.sY * trm.b + trm.c));
	else
		return abs(pair.sX + pair.vX / 4.0f - (pair.mX * trm.z + pair.sX * trm.b + trm.c));
}

int Ransac::findInliers(aTransform trm)
{
	int count = 0;
	for (int i = 0; i < m_array.count; i++)
		if (findError(trm, m_array.array[i]) < TR_ERROR)
			count++;
	return count;
}
enum PAR_TYPE { PAR_A, PAR_B, PAR_C, PAR_Z };

template <PAR_TYPE letter>
int Comparator(const void* left, const void* right)
{
	int result = 0;
	double diff;
	PAR_TYPE thing = letter;
	const aTransform* tr_left, *tr_right;
	tr_left = (const aTransform*)left;
	tr_right = (const aTransform*)right;
	switch (letter)
	{
	case PAR_A: diff = tr_left->a - tr_right->a; break;
	case PAR_B: diff = tr_left->b - tr_right->b; break;
	case PAR_C: diff = tr_left->c - tr_right->c; break;
	case PAR_Z: diff = tr_left->z - tr_right->z; break;
	}
	if (diff > 0) result = 1;
	else if (diff < 0) result = -1;
	else result = 0;

	return result;
}

int Ransac::Analyse()
{
	if (m_array.count < TR_CONF)
		return -1;
	aTransform *trArray = (aTransform*)calloc(ITERATIONS, sizeof(aTransform));
	int count = 0;
	aTransform zeroTransform = { 0 };
	zeroTransform.b = 1.0f;
	int zero_inliers = findInliers(zeroTransform);
	int max_inliers = MAX(findInliers(m_transform), zero_inliers);
	vecPairArray temp;
	temp.array = (vecPair*)malloc(INPUT_SIZE*sizeof(vecPair));
	for (int i = 0; i < ITERATIONS / 20; i++)
	{
		makeRandomArray(temp);
		max_inliers = MAX(max_inliers, findInliers(findTransform(temp)));
	}
	free(temp.array);

	int magic_treshold = int(max_inliers - (max_inliers - zero_inliers) * (1.0f - TR_DYN) - TR_FIX);

	for (int i = 0; i < ITERATIONS; i++)
	{
		temp.array = (vecPair*)malloc(INPUT_SIZE*sizeof(vecPair));
		makeRandomArray(temp);
		aTransform temp_res = findTransform(temp);
		if (findInliers(temp_res) > magic_treshold)
				trArray[count++] = temp_res;
		free(temp.array);
	}
	if (count == 0)
		m_transform = zeroTransform;
	else
	{
		qsort(trArray, count, sizeof(aTransform), Comparator<PAR_A>);
		m_transform.a = trArray[count / 2].a;
		qsort(trArray, count, sizeof(aTransform), Comparator<PAR_B>);
		m_transform.b = trArray[count / 2].b;
		qsort(trArray, count, sizeof(aTransform), Comparator<PAR_C>);
		m_transform.c = trArray[count / 2].c;
		qsort(trArray, count, sizeof(aTransform), Comparator<PAR_Z>);
		m_transform.z = trArray[count / 2].z;
		if (abs(m_transform.b) < 0.1 || abs(m_transform.b) > 10.0)
			m_transform = zeroTransform;
	}
	free(trArray);
	return 0;
}

aTransform Ransac::Result()
{
	return m_transform;
}

int Ransac::GetTotal()
{
	return m_array.count;
}
int Ransac::GetFit()
{
	return findInliers(m_transform);
}