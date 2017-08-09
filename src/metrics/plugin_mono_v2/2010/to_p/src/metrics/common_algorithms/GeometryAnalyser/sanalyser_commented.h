#include "me_inst.h"
#include "ransac_ap.h"

//не юзается
enum SAVE_TYPE { O_LEFT = 0, O_RIGHT = 1, O_MONO = 2, M_STEREO = 3, M_MONO = 4, M_SSIM };
enum RES_TYPE { GEOM = 0, POS = 1 };
//что это за кадр
enum POSITION { LEFT = 0, RIGHT = 1, MONO = 2 };
//что это за преобразование (Geometry_Left-Right / Geometry_Left-Mono / Position_Left-Mono...)
enum RESULT { G_LR = 0, G_LM = 1, P_LM = 2, P_RM = 3 };

//он здесь главный
class StereoAnalyser
{
public:
	StereoAnalyser(); //конструктор
	void Init(int width, int height); //инициализирует массивы
	void InitStereo(PNG_Image * left, PNG_Image * right); //создаёт инстанс ME для стерео
	void InitMono(PNG_Image * mono, bool disable_stereo); //то же, но для моно (чтоб не забивать память, когда моно не нужно - грузим его отдельно)
	void Analyse(); //запустить анализ
	void Load(PNG_Image * src, POSITION t); //загрузить картинку указанного типа
	int SaveVis(int vis_type); //не сделано
	double GetTotal(int t); //сколько всего векторов после фильтрации (в процентах 0.0 ... 1.0)
	double GetFit(int t); //сколько подошло к найденному преобразованию (0.0 ... 1.0)
	ME_Instance *  GetME(int t); //достать мяшку
	aTransform GetResults(RESULT t); //достать что насчитали
	~StereoAnalyser(); //деструктор
private:
	int m_width; //шинигами
	int m_height; //высота
	bool m_process_stereo; //считать ли стерео (чтоб при анализе моно забить на стерео вообще)
	PNG_Image *o_left; //картинки
	PNG_Image *o_right;
	PNG_Image *o_mono;
	BYTE* m_ssim; //штука для анализа разницы 2D / 3D
	void TransformMono(); //сделать 2D кадр максимально похожим на 3D
	ME_Instance L_TO_R; //мяшки
	ME_Instance L_TO_M;
	ME_Instance R_TO_M;
	Ransac TR_G_S; //инстансы RANSAC
	Ransac TR_G_M;
	Ransac TR_P_ML;
	Ransac TR_P_MR;
	bool isStereoInit; //запущены ли мяшки для стерео
	bool isMonoInit; // то же, но для моно
};