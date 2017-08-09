#include "metric.h"
#include "im_vis.h"
#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <time.h>
#include <algorithm>

using namespace std;
using namespace cv;

bool CMonoPlugin::FitBrightness(PNG_Image mono, PNG_Image left)
{
	bool wastransform = false;

	left.SaveToPNG("left.png");
	mono.SaveToPNG("mono.png");
	Mat lef(m_height, m_width, CV_8UC1, left.GetData());
	Mat mon(m_height, m_width, CV_8UC1, mono.GetData());
	Mat dst_ref, dst_mon;

	cvtColor(lef, lef, CV_GRAY2RGB);
	//cvtColor(mon, dst_mon, CV_BGR2GRAY );

	//equalizeHist(lef, lef);
	//equalizeHist(mon, mon);

	Mat ycrcb;

    cvtColor(lef,ycrcb,CV_RGB2YCrCb);
	cout << "converted" << endl;

    vector<Mat> channels;
    split(ycrcb,channels);
	cout << "split" << endl;

    equalizeHist(channels[0], channels[0]);
	cout << "equal" << endl;

    Mat result;
    merge(channels,ycrcb);

    cvtColor(ycrcb,result,CV_YCrCb2RGB);

namedWindow("eq", CV_WINDOW_AUTOSIZE );

imshow("eq", result );
waitKey(0);

	cvtColor(result,lef,CV_RGB2GRAY);
	
	BYTE *dataMat = lef.data;
	PNG_Image g;
	g.Set(m_Left.width(), m_Left.height(), 4, dataMat);
	g.SaveToPNG("lef.png");
	
	dataMat = mon.data;
	g.Set(m_Left.width(), m_Left.height(), 4, dataMat);
	g.SaveToPNG("mon.png");

	/*dataMat = dst_ref.data;
	cout << "datamat" << endl;
	PNG_Image f;
	f.Set(m_Left.width(), m_Left.height(), 4, dataMat);
	cout << "set" << endl;
	m_Left = f;
	m_Left.ConvertToType(4);
	cout << "converted" << endl;
	m_Left.SaveToPNG("br_left.png");*/
	//PNG_Image::SaveArrayToPNG(f.GetData(), m_width, m_height, "br_fit.png");
	//f.SaveToPNG("br_fit.png");
	cout << "saved" << endl;
	
  
	//waitKey(0);

	return wastransform;
}