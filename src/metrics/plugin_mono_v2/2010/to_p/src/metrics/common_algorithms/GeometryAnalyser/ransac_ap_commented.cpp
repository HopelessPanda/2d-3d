#include "ransac_ap.h"

//конструктор
Ransac::Ransac()
{
	m_type = GEOMETRY; //по дефолту считаем геометрию
	random_array = NULL; //не выделяем память, пока не потребуется
}

//деструктор
Ransac::~Ransac()
{
	if (random_array != NULL) //не забываем освободить, если выделяли
		free(random_array);
}

//инициализируем ранзак
void Ransac::Init(int type)
{
	m_type = type; //геометрия или позиция, они потом по-разному считаются
	random_array = (int*)malloc(RANDOM_SIZE*sizeof(int)); //детерминация рандомизатора
	random_count = 0; 
	srand(SEED); //запускаем фиксированным сидом
	for (int i = 0; i < RANDOM_SIZE; i++) //и забиваем массив псевдослучайными числами 
		random_array[i] = rand(); //таким образом, они всегда одни и те же при каждом запуске
		//и результаты не будут меняться при каждом прогоне
	m_transform.a = m_transform.c = m_transform.z = 0.0f; //нулевое преобразование, один-в-один
	m_transform.b = 1.0f; //при подсчете используется предыдущее вычисленное преобразование
	//потому желательно инициализировать его прямо здесь .
}

//загрузить пачку векторов для анализа извне.
//блин, тут бы "(vecPairArray &src)" было лучше. поменять!
void Ransac::Load(vecPairArray src)
{
	m_array = src;
}

//случайным образом выбрать какой-то вектор из пачки
//при этом использовать наш псевдослучайный массив и
//увеличить его счётчик. 
//% m_array.count - выбрать число от 0 до m_array.count
int Ransac::randomIndex()
{
	return random_array[(random_count++) % RANDOM_SIZE] % m_array.count;
}

//просто заполняет переданный vecPairArray
//нужным числом случайно выбранных векторов
void Ransac::makeRandomArray(vecPairArray &src)
{
	for (int i = 0; i < INPUT_SIZE; i++)
		src.array[i] = m_array.array[randomIndex()];
	src.count = INPUT_SIZE;
}

//поиск преобразования, просто решая СЛАУ
aTransform Ransac::findTransform(vecPairArray src)
{
	//на случай ошибки, инициализируем нулевым преобразованием
	aTransform res = { 0 };
	res.b = 1.0; //коэффициент зума у нулевого преобразования - единица.
	if (src.count < 3) //нам нужно не менее трёх векторов. иначе вернем нулевое преобр-е
		return res;

	cv::Mat matrix(3, 3, CV_64F); //матрица известных значений
	cv::Mat matrix_inv(3, 3, CV_64F); //обратная к ней
	cv::Mat column(3, 1, CV_64F); //столбец свободных членов
	cv::Mat result(3, 1, CV_64F); //столбец результата

	for (int i = 0; i < 3; i++) //заполняем матрицы!
	{
		//если считаем геометрию, конечный результат преобразования - конечная точка вертикального вектора (анализ вертикальных составляющих)
		//иначе - конечная точка горизонтального вектора! (анализируем горизонтальные составляющие)
		double z = m_type == GEOMETRY ? src.array[i].sY + src.array[i].vY / 4.0f : src.array[i].sX + src.array[i].vX / 4.0f;
		
		//далее заполнение матрицы из трёх векторов известных коэффициентов.
		//для геометрии аппроксимируем формулу a*sX + b*sY + c*1.0, 
		//для позиции соответственно a*sX + b*Depth + c*1.0,
		//потому заполняем соответственно:
		
		matrix.at<double>(i, 0) = src.array[i].sX; //первый элемент вектора "исходных данных". 
												//по нему находится коэффициент 'a'
		if (m_type == GEOMETRY) //второй коэффициент у геометрии и позиции различен
			matrix.at<double>(i, 1) = src.array[i].sY; //для определения зума, засунем сюда верт. координату вектора
		else
			matrix.at<double>(i, 1) = src.array[i].mX; //а для позиции - горизонтальную составляющую вектора поля L-R 
		//ещё раз причём тут вообще позиция.
		//идея в том, что при отсутствии зума, моно находится где-то ровно между левым и правым ракурсами
		//тогда горизонтальные вектора из Left в Mono будут являться одной и той же частью
		//векторов из Left в Right. вот это коэффициент и ищем. конечно, это далеко не всегда так.
		
		matrix.at<double>(i, 2) = 1.0f; //константный сдвиг, не зависящий ни от чего. коэффициент 'c'
		
		column.at<double>(i) = z; //а вот это должно получиться
		result.at<double>(i) = 0; //сюда получим аппроксимированный вектор коэффициентов
	}
	//всё оч просто. если Ax = B, где A - матрица известных значений, а B - столбец результата,
	//то просто находим x как inv(A)*И   (домножили обе части слева на inv(A))
	
	cv::invert(matrix, matrix_inv);  //находим inv(A)
	result = matrix_inv*column; // найдём результат

	//поменяем порядок коэффициентов для позиции
	if (m_type == GEOMETRY)
	{
		res.a = result.at<double>(0);
		res.b = result.at<double>(1);
		res.c = result.at<double>(2);
		res.z = 0;
	}
	else
	{
		res.b = result.at<double>(0); //внезапно раз мы домножали на sX при анализе горизонтальных
									  //составляющих, то это именно коэффициент зума.
									  //коэффициент вращения - когда домножаем на __другую__ координату
		res.z = result.at<double>(1);
		res.c = result.at<double>(2);
		res.a = 0;
	}

	return res;
}

//определим, насколько не влез вектор в найденое преобразование
double Ransac::findError(aTransform trm, vecPair pair)
{
	//по сути, смотрим, куда должен был прийти вектор по преобразованию и находим разницу с тем,
	//что у нас нашлось в ME. не забываем поделить длину на 4
	if (m_type == GEOMETRY)
		return abs(pair.sY + pair.vY / 4.0f - (pair.sX * trm.a + pair.sY * trm.b + trm.c));
	else
		return abs(pair.sX + pair.vX / 4.0f - (pair.mX * trm.z + pair.sX * trm.b + trm.c));
}

//подсчёт количества хорошо влезших векторов
//для конкретного преобразования.
//фиксированный порог - не круто :(
int Ransac::findInliers(aTransform trm)
{
	int count = 0;
	for (int i = 0; i < m_array.count; i++)
		if (findError(trm, m_array.array[i]) < TR_ERROR)
			count++;
	return count;
}

//сравниватель для сортировки. ничего особенного
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

//главная процедура поиска преобразования!
//первая тут в очереди на рефакторинг
int Ransac::Analyse()
{
	if (m_array.count < TR_CONF) //сильно мало векторов на входе - нафиг, нафиг. и считать бесполезно (а может и нет? о_О)
		return -1;
	aTransform *trArray = (aTransform*)calloc(ITERATIONS, sizeof(aTransform)); //сюда запихиваем всё хорошее, что нашли
	int count = 0; //сколько уже нашли
	aTransform zeroTransform = { 0 }; //инициализируем нулевое преобразование
	zeroTransform.b = 1.0f;
	int zero_inliers = findInliers(zeroTransform); // находим сколько векторов под него подходят
	int max_inliers = MAX(findInliers(m_transform), zero_inliers); //не забываем про преобразование с предыдущего кадра. пригодится хотя б для определения границы
	vecPairArray temp; //сюда будем засовывать пачки векторов на анализ
	temp.array = (vecPair*)malloc(INPUT_SIZE*sizeof(vecPair)); //выделяем под него память
	for (int i = 0; i < ITERATIONS / 20; i++) //предварительный небольшой прогончик для примерного определения наилучшей границы
	{
		makeRandomArray(temp);//заполняем его случайными векторами
		max_inliers = MAX(max_inliers, findInliers(findTransform(temp))); //считаем преобразование, сколько векторов влезло, засовываем в максимум
	}
	free(temp.array);

	//ну вот совсем магическим образом находим границу, выше которой считаем преобразование хорошим
	int magic_treshold = int(max_inliers - (max_inliers - zero_inliers) * (1.0f - TR_DYN) - TR_FIX);

#pragma omp parallel for private (temp)
	for (int i = 0; i < ITERATIONS; i++) //теперь уже большой прогончик с запоминанием
	{
		temp.array = (vecPair*)malloc(INPUT_SIZE*sizeof(vecPair)); //опять массив, куда пихаем
		makeRandomArray(temp); //заполняем его случайными векторами
		aTransform temp_res = findTransform(temp); //вот мы его нашли
		if (findInliers(temp_res) > magic_treshold)
#pragma omp critical
			trArray[count++] = temp_res; //запоминаем, если крутое
		free(temp.array); //нафиг я его перевыделяю постоянно? а чтоб параллелизация нормально отработала!
		//кстати псевдорандомизация в параллелизации здесь работает только благодаря медианной фильтрации и фиксированному числу шагов!!
		//убрать хоть одно - и значения будут каждый раз другие.
	}
	if (count == 0) //ничего не нашли? печалька. берём нулевое. P.S. если предыдущее более крутое - неплохо бы взять его! переделать
		m_transform = zeroTransform;
	else
	{
		//тупейшая медианная фильтрация для сглаживания неточностей ранзака.
		//изначально выбиралось наикрутейшее преобразование, но потом оказалось, что
		//их может быть несколько, причем совершенно разных - и параметры колбасило
		//потому теперь вот так.
		qsort(trArray, count, sizeof(aTransform), Comparator<PAR_A>);
		m_transform.a = trArray[count / 2].a;
		qsort(trArray, count, sizeof(aTransform), Comparator<PAR_B>);
		m_transform.b = trArray[count / 2].b;
		qsort(trArray, count, sizeof(aTransform), Comparator<PAR_C>);
		m_transform.c = trArray[count / 2].c;
		qsort(trArray, count, sizeof(aTransform), Comparator<PAR_Z>);
		m_transform.z = trArray[count / 2].z;
		if (abs(m_transform.b) < 0.1 || abs(m_transform.b) > 10.0) //типа совсем криво нашлось. зум такой не бывает. отбрасываем.
			m_transform = zeroTransform;
	}
	free(trArray);
	return 0;
}

//получить посчитанный результат
aTransform Ransac::Result()
{
	return m_transform;
}

//сколько всего векторов на анализе
int Ransac::GetTotal()
{
	return m_array.count;
}

//и сколько влезло в найденную трансформацию
//раньше использовалось для определения качества,
//насколько хорошее преобразование нашлось, потом как-то подзабил
int Ransac::GetFit()
{
	return findInliers(m_transform);
}