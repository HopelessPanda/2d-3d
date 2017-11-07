#include "metric.h"
#include "im_vis.h"
#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <time.h>
#include <iomanip>
#include <utility>
#include <algorithm>
#include <queue>
#include <limits>
#include "depth_estimator.hpp"

using namespace std;
using namespace cv;


#define CL_THLD 50
#define THLD 70
#define ME_LENGTH 8
int MAX_ERROR, MIN_ERROR;

struct object {
	int index;
	vector<pair<int, int>> obj_coords;
	vector<pair<int, int>> edge_coords;
	double area;
};

bool check_error(BYTE *error_map, int index)
{
	return (error_map[index] > THLD);
}

bool comp_hor(pair<int, int> a, pair<int, int> b)
{
	return a.second < b.second;
}

bool comp_ver(pair<int, int> a, pair<int, int> b)
{
	return a.first < b.first;
}

bool comp_area(object a, object b) {
	return a.area < b.area;
}

BYTE check_ME_and_ERROR(BYTE* xL, BYTE* xR, BYTE* yL, BYTE* yR, long* lERROR, long* rERROR, int index) {
	long threshold = MAX_ERROR*0.05;
	if (xL[index] > ME_LENGTH && xR[index] > ME_LENGTH && lERROR[index] > threshold && rERROR[index] > threshold) return 255;
	if (yL[index] > ME_LENGTH && yR[index] > ME_LENGTH && lERROR[index] > threshold && rERROR[index] > threshold) return 255;
	
	return 0;
}

double CMonoPlugin::FindDeleted(double alpha, int framenum)
{
	LOG log(true, 0, true);
	cout << "looking for deleted objects\n";
	//log.vis_grey_image(m_Mono.GetData(), m_height, m_width, "mono.png");
	//log.vis_grey_image(m_Left.GetData(), m_height, m_width, "left.png");

	cMV* me_MtoL = m_analyser.GetME(1)->Field(1); // MtoL
	cMV* me_LtoM = m_analyser.GetME(1)->Field(0); // LtoM
	cMV* me_MtoR = m_analyser.GetME(2)->Field(1); // MtoR
	cMV* me_RtoM = m_analyser.GetME(2)->Field(0); // MtoR
	cMV* me_LtoR = m_analyser.GetME(0)->Field(0); // LtoR

	BYTE* map_y = (BYTE*)malloc(m_height*m_width * sizeof(BYTE));
	long* map_error_MtoL = (long*)malloc(m_height*m_width * sizeof(long));
	long* map_error_MtoR = (long*)malloc(m_height*m_width * sizeof(long));
	BYTE* map_x_MtoL = (BYTE*)malloc(m_height*m_width * sizeof(BYTE));
	BYTE* map_x_MtoR = (BYTE*)malloc(m_height*m_width * sizeof(BYTE));
	BYTE* map_y_MtoL = (BYTE*)malloc(m_height*m_width * sizeof(BYTE));
	BYTE* map_y_MtoR = (BYTE*)malloc(m_height*m_width * sizeof(BYTE));
	BYTE* map_x_compensated_L = (BYTE*)malloc(m_height*m_width * sizeof(BYTE));
	BYTE* map_x_compensated_R = (BYTE*)malloc(m_height*m_width * sizeof(BYTE));
	BYTE* map_y_compensated_L = (BYTE*)malloc(m_height*m_width * sizeof(BYTE));
	BYTE* map_y_compensated_R = (BYTE*)malloc(m_height*m_width * sizeof(BYTE));
	BYTE *depth_MtoR = (BYTE*)malloc(m_width*m_height * sizeof(BYTE));
	m_analyser.R_TO_M.GetDepthMap(depth_MtoR, 1);
	BYTE *depth_MtoL = (BYTE*)malloc(m_width*m_height * sizeof(BYTE));
	m_analyser.L_TO_M.GetDepthMap(depth_MtoL, 1);
	BYTE *depth_LtoR = (BYTE*)malloc(m_width*m_height * sizeof(BYTE));
	m_analyser.L_TO_R.GetDepthMap(depth_LtoR, 1);
	log.vis_normalized(depth_MtoL, m_height, m_width, "depth_MtoL.png", framenum);
	log.vis_normalized(depth_MtoR, m_height, m_width, "depth_MtoR.png", framenum);
	log.vis_normalized(depth_LtoR, m_height, m_width, "depth_LtoR.png", framenum);
	MAX_ERROR = 0;
	MIN_ERROR = LONG_MAX;

	//match depths
	unsigned int source_hist[256];
	histogram(m_width, m_height, depth_LtoR, source_hist);
	unsigned int LtoM_hist[256];
	histogram(m_width, m_height, depth_MtoL, LtoM_hist);
	histogram_match(m_width, m_height, depth_MtoL, LtoM_hist, source_hist);
	log.vis_normalized(depth_MtoL, m_height, m_width, "depth_MtoL.png", framenum);

	// compute min and max error in ME vectors from mono to left
	for (int i = 0; i < m_height; i++) {
		for (int j = 0; j < m_width; j++) {
			int index = i*m_width + j;
			// visualize x components of ME vectors from mono to left and right
			map_x_MtoL[index] = abs(me_MtoL[index].x);
			map_x_MtoR[index] = abs(me_MtoR[index].x);
			map_y_MtoL[index] = abs(me_MtoL[index].y);
			map_y_MtoR[index] = abs(me_MtoR[index].y);
			map_x_compensated_L[index] = abs(map_x_MtoL[index] - abs(me_LtoM[index].x));
			map_x_compensated_R[index] = abs(map_x_MtoR[index] - abs(me_RtoM[index].x));
			map_y_compensated_L[index] = abs(abs(me_MtoL[index].y) - abs(me_LtoM[index].y));
			map_y_compensated_R[index] = abs(abs(me_MtoR[index].y) - abs(me_RtoM[index].y));
			
			map_error_MtoL[index] = abs(me_MtoL[index].error);
			map_error_MtoR[index] = abs(me_MtoR[index].error);
			if (map_error_MtoL[index] < MIN_ERROR) MIN_ERROR = map_error_MtoR[index];
			if (map_error_MtoR[index] > MAX_ERROR) MAX_ERROR = map_error_MtoR[index];
		}
	}
	log.vis_normalized(map_x_MtoR, m_height, m_width, "x_vector_MtoR.png", framenum);
	log.vis_normalized(map_x_MtoL, m_height, m_width, "x_vector_MtoL.png", framenum);
	log.vis_normalized(map_y_MtoL, m_height, m_width, "y_vector_MtoL.png", framenum);
	log.vis_normalized(map_y_MtoR, m_height, m_width, "y_vector_MtoR.png", framenum);
	log.vis_normalized(map_x_compensated_L, m_height, m_width, "x_vector_compensated_L.png", framenum);
	log.vis_normalized(map_x_compensated_R, m_height, m_width, "x_vector_compensated_R.png", framenum);
	log.vis_normalized(map_y_compensated_L, m_height, m_width, "y_vector_compensated_L.png", framenum);
	log.vis_normalized(map_y_compensated_R, m_height, m_width, "y_vector_compensated_R.png", framenum);

	BYTE* map_change = (BYTE*)malloc(m_height*m_width * sizeof(BYTE));
	for (int i = 0; i < m_height; i++) {
		for (int j = 0; j < m_width; j++) {
			int index = i*m_width + j;
			map_change[index] = check_ME_and_ERROR(map_x_compensated_L, map_x_compensated_R, map_y_compensated_L, map_y_compensated_R,
				map_error_MtoL, map_error_MtoR, index);
		}
	}
	log.vis_grey_image(map_change, m_height, m_width, "change_map_me.png", framenum);

	BYTE* map_change_color = (BYTE*)malloc(m_height*m_width * sizeof(BYTE));
	for (int i = 0; i < m_height; i++) {
		for (int j = 0; j < m_width; j++) {
			int index = i*m_width + j;
			map_change_color[index] = color_check(&m_Mono, &m_Left, i, j, me_MtoL[index].x) ? 255 : 0;
		}
	}
	log.vis_grey_image(map_change_color, m_height, m_width, "change_map_color.png", framenum);

	return 0;

	int* map_checked = (int*)malloc(m_height*m_width * sizeof(int));
	for (int i = 0; i < m_height*m_width; i++) map_checked[i] = 0;

	bool drawn = false;
	int count = 1;

	vector<object> all;
	queue<pair<int, int>> q;

	for (int i = 3; i < m_height - 3; i++) {
		for (int j = 3; j < m_width - 3; j++) {
			int index = i*m_width + j;
			if (map_checked[index] != 0) { continue; }
			if (map_error_MtoL[index] > THLD) { // found vector with large error - candidate

				object found_obj;
				pair<int, int> e, w;
				found_obj.index = count;
				q.push(std::make_pair(i, j));

				while (!q.empty()) {
					//cout << "front " << q.front().first << ' ' << q.front().second << endl;
					w.first = e.first = q.front().first;
					w.second = e.second = q.front().second;
					while (e.second < m_width - 1 && map_error_MtoL[e.first*m_width + e.second] > THLD && map_checked[e.first*m_width + e.second] == 0) {
						e.second++;
					}
					if (e.second < m_width - 1)
						found_obj.edge_coords.push_back(std::make_pair(e.first, e.second + 1));
					while (w.second > 0 && map_error_MtoL[w.first*m_width + w.second] > THLD && map_checked[e.first*m_width + e.second] == 0) {
						w.second--;
					}
					if (w.second > 0)
						found_obj.edge_coords.push_back(std::make_pair(w.first, w.second - 1));
					for (int k = w.second; k < e.second; k++) {
						found_obj.obj_coords.push_back(std::make_pair(e.first, k));
						map_checked[e.first*m_width + k] = 1;
						if (e.first - 1 > 0) {
							if (map_error_MtoL[(e.first - 1)*m_width + k] > THLD && map_checked[(e.first - 1)*m_width + k] == 0) q.push(std::make_pair(e.first - 1, k));
							else found_obj.edge_coords.push_back(std::make_pair(e.first - 1, k));
						}
						if (e.first + 1 < m_height) {
							if (map_error_MtoL[(e.first + 1)*m_width + k] > THLD && map_checked[(e.first + 1)*m_width + k] == 0) q.push(std::make_pair(e.first + 1, k));
							else found_obj.edge_coords.push_back(std::make_pair(e.first + 1, k));
						}
					}
					q.pop();
				}

				count++;
				if (found_obj.obj_coords.size() > 1000) {
					all.push_back(found_obj);
				}
			}
		}
	}
	log.print("finished search, total objects", all.size());
	if (all.size() == 0) return 0;

	//check depth of neighbouring pixels
	//BYTE *depth_MtoL = (BYTE*)malloc(m_width*m_height * sizeof(BYTE));
	m_analyser.L_TO_N.GetDepthMap(depth_MtoL, 1);
	//log.vis_grey_image(depth_MtoL, m_height, m_width, "_depth.png", framenum);

	for (int i = 0; i < all.size(); i++) {
		vector<int> depths;
		for (auto j = all[i].edge_coords.begin(); j != all[i].edge_coords.end(); j++) {
			depths.push_back(depth_MtoL[j->first * m_width + j->second]);
		}
		sort(depths.begin(), depths.end());

		vector<pair<int, int>> freq;
		int tmp = depths.front();
		int count = 0;
		for (auto j = depths.begin(); j != depths.end(); j++) {
			if (tmp == *j) count++;
			else {
				freq.push_back(std::make_pair(tmp, count));
				count = 1;
				tmp = *j;
			}
		}
		sort(freq.begin(), freq.end(), comp_hor);
		int m1 = freq[freq.size() - 1].first;
		int m2 = freq[freq.size() - 2].first;
		cout << m1 << ' ' << m2 << endl;
		if (abs(m1 - m2) > 50) {
			log.print("occlusion");
			all.erase(all.begin() + i);
		}

	}
	if (all.size() == 0) return 0;

	for (int i = 0; i < all.size(); i++) {
		sort(all[i].obj_coords.begin(), all[i].obj_coords.end(), comp_ver);
		int height = abs(all[i].obj_coords.front().first - all[i].obj_coords.back().first);

		vector<int> widths;
		for (int y = all[i].obj_coords.front().first; y < all[i].obj_coords.back().first; y++) {
			vector<int> cur_y_horizontal_coords;
			for (auto x = all[i].obj_coords.begin(); x < all[i].obj_coords.end(); x++) {
				if (x->first == y) cur_y_horizontal_coords.push_back(x->second);
			}
			sort(cur_y_horizontal_coords.begin(), cur_y_horizontal_coords.end());
			widths.push_back(abs(cur_y_horizontal_coords.front() - cur_y_horizontal_coords.back()));
		}

		double outliers;
		for (auto w = widths.begin(); w < widths.end(); w++) {
			//cout << height << " " << *w << endl;
			if ((*w) * 4 > height) {
				outliers += 1;
			}
		}
		if (outliers / (double)widths.size() > 0.01) all.erase(all.begin() + i);

	}

	// find coordinates of found del object   
	if (all.size() > 0) {
		//fstream fr, fr_coord;
		//fr.open("d_frame order.txt", std::ios_base::app);
		//fr_coord.open("d_coordinates in frame and area.txt", std::ios_base::app);

		//fr << framenum<< endl;

		for (int i = 0; i < all.size(); i++) {
			all[i].obj_coords;
			sort(all[i].edge_coords.begin(), all[i].edge_coords.end(), comp_hor);
			int x_min = all[i].edge_coords.back().second;
			int x_max = all[i].edge_coords.front().second;
			sort(all[i].edge_coords.begin(), all[i].edge_coords.end(), comp_ver);
			int y_max = all[i].edge_coords.front().first;
			int y_min = all[i].edge_coords.back().first;
			//fr_coord << framenum << ' ' << x_min << ' ' << x_max << ' ' << y_min << ' ' << y_max << endl;
			all[i].area = (x_max - x_min)*(y_max - y_min) / (double)(m_width*m_height);
		}
		sort(all.begin(), all.end(), comp_area);
		del_square = all.front().area;
		//fr_coord << del_square << endl;

		//fr.close();
		//fr_coord.close();
	}


	// make changemap
	int x = 0, y = 0;
	double square = 0;
	BYTE* change_map = (BYTE*)malloc(m_height*m_width * sizeof(BYTE));
	for (int i = 0; i < m_height*m_width; i++) change_map[i] = 0;
	for (int k = 0; k < all.size(); k++) {
		for (auto it = all[k].obj_coords.begin(); it < all[k].obj_coords.end(); it++) {
			change_map[it->first*m_width + it->second] = 255;
			square += 1;
		}
	}
	//PNG_Image::SaveArrayToPNG(change_map, m_width, m_height, "change_map.png");
	log.vis_grey_image(change_map, m_height, m_width, "change_Map", framenum);
	return square / (double)(m_width*m_height);
}

bool CMonoPlugin::color_check(PNG_Image *a, PNG_Image *b, const int i, const int j, int x) {
	BYTE dB = abs(a->b(j, i) - b->b(j + x, i));
	BYTE dG = abs(a->g(j, i) - b->g(j + x, i));
	BYTE dR = abs(a->r(j, i) - b->r(j + x, i));
	return dB > CL_THLD && dG > CL_THLD && dG > CL_THLD;
	///std::sqrt(std::pow(dB, 2) + std::pow(dG, 2) + std::pow(dR, 2));
}

int cur_y;
bool comp_hor_cur(pair<int, int> a, pair<int, int> b)
{
	if (a.first == cur_y && b.first == cur_y)
		return a.second < b.second;
}

double CMonoPlugin::FindDeletedTwoVersions(BYTE* depth_map, int framenum) {
	int sum = 0;
	for (int i = 0; i < m_width*m_height; i++) sum += depth_map[i];
	if (sum == 0) { cout << "no depth\n"; return 0; }

	int* map_checked = (int*)malloc(m_height*m_width * sizeof(int));
	for (int i = 0; i < m_height*m_width; i++) map_checked[i] = 0;

	bool drawn = false;
	int count = 1;

	vector<object> all;
	queue<pair<int, int>> q;
	m_Left.ConvertToType(BGR_I);
	m_Mono = t_Mono;
	m_Mono.ConvertToType(BGR_I);

	for (int i = 3; i < m_height - 3; i++) {
		for (int j = 3; j < m_width - 3; j++) {
			int index = i*m_width + j;
			if (map_checked[index] != 0) { continue; }
			if (color_check(&m_Mono, &m_Left, i, j)) { // found pixel with large color difference - candidate

				object found_obj;
				pair<int, int> e, w;
				found_obj.index = count;
				q.push(std::make_pair(i, j));

				while (!q.empty()) {
					//cout << "front " << q.front().first << ' ' << q.front().second << endl;
					w.first = e.first = q.front().first;
					w.second = e.second = q.front().second;
					while (e.second < m_width - 1 && color_check(&m_Mono, &m_Left, e.first, e.second) && map_checked[e.first*m_width + e.second] == 0) {
						e.second++;
					}
					if (e.second < m_width - 1)
						found_obj.edge_coords.push_back(std::make_pair(e.first, e.second + 1));
					while (w.second > 0 && color_check(&m_Mono, &m_Left, w.first, w.second) && map_checked[e.first*m_width + e.second] == 0) {
						w.second--;
					}
					if (w.second > 0)
						found_obj.edge_coords.push_back(std::make_pair(w.first, w.second - 1));
					for (int k = w.second; k < e.second; k++) {
						found_obj.obj_coords.push_back(std::make_pair(e.first, k));
						map_checked[e.first*m_width + k] = 1;
						if (e.first - 1 > 0) {
							if (color_check(&m_Mono, &m_Left, (e.first - 1), +k) && map_checked[(e.first - 1)*m_width + k] == 0) q.push(std::make_pair(e.first - 1, k));
							else found_obj.edge_coords.push_back(std::make_pair(e.first - 1, k));
						}
						if (e.first + 1 < m_height) {
							if (color_check(&m_Mono, &m_Left, (e.first + 1), k) && map_checked[(e.first + 1)*m_width + k] == 0) q.push(std::make_pair(e.first + 1, k));
							else found_obj.edge_coords.push_back(std::make_pair(e.first + 1, k));
						}
					}
					q.pop();
				}

				count++;
				if (found_obj.obj_coords.size() > 1000) {
					all.push_back(found_obj);
				}
			}
		}
	}
	//cout << "finished search, total objects" << all.size() << endl;

	for (int i = 0; i < all.size(); i++) {
		vector<int> depths;
		for (auto j = all[i].edge_coords.begin(); j != all[i].edge_coords.end(); j++) {
			depths.push_back(depth_map[j->first * m_width + j->second]);
		}
		sort(depths.begin(), depths.end());

		/*vector<pair<int, int>> freq;
		int tmp = depths.front();
		int count = 0;
		for (auto j = depths.begin(); j != depths.end(); j++) {
			if (tmp == *j) count++;
			else {
				freq.push_back(std::make_pair(tmp, count));
				count = 1;
				tmp = *j;
			}
		}
		sort(freq.begin(), freq.end(), comp_hor);
		int m1 = freq[freq.size() - 1].first;
		int m2 = freq[freq.size() - 2].first;
		cout << m1 << ' ' << m2 << endl;*/
		int m1 = depths.front();
		int m2 = depths.back();
		//cout << m1 << ' ' << m2 << endl;
		if (abs(m1 - m2) > 20) {
			//cout << "occlusion\n";
			all.erase(all.begin() + i);
		}

	}

	//checking shape
	for (int i = 0; i < all.size(); i++) {
		sort(all[i].obj_coords.begin(), all[i].obj_coords.end(), comp_ver);
		int height = abs(all[i].obj_coords.front().first - all[i].obj_coords.back().first);

		vector<int> widths;
		for (int y = all[i].obj_coords.front().first; y < all[i].obj_coords.back().first; y++) {
			cur_y = y;
			vector<int> cur_y_horizontal_coords;
			for (auto x = all[i].obj_coords.begin(); x < all[i].obj_coords.end(); x++) {
				if (x->first == y) cur_y_horizontal_coords.push_back(x->second);
			}
			sort(cur_y_horizontal_coords.begin(), cur_y_horizontal_coords.end());
			widths.push_back(abs(cur_y_horizontal_coords.front() - cur_y_horizontal_coords.back()));
		}

		double outliers;
		for (auto w = widths.begin(); w < widths.end(); w++) {
			//cout << height << " " << *w << endl;
			if ((*w) * 4 > height) {
				outliers += 1;
			}
		}
		if(outliers/(double)widths.size() > 0.01) all.erase(all.begin() + i);

	}

	//cout << "not occlusions " << all.size() << endl;

	// find coordinates of found del object   
	if (all.size() > 0) {
		fstream fr_coord;
		//fr_coord.open("d_coordinates in frame and area.txt", std::ios_base::app);


		for (int i = 0; i < all.size(); i++) {
			all[i].obj_coords;
			sort(all[i].edge_coords.begin(), all[i].edge_coords.end(), comp_hor);
			int x_min = all[i].edge_coords.back().second;
			int x_max = all[i].edge_coords.front().second;
			sort(all[i].edge_coords.begin(), all[i].edge_coords.end(), comp_ver);
			int y_max = all[i].edge_coords.front().first;
			int y_min = all[i].edge_coords.back().first;
			//fr_coord << framenum << ' ' << x_min << ' ' << x_max << ' ' << y_min << ' ' << y_max << endl;
			all[i].area = (x_max - x_min)*(y_max - y_min) / (double)(m_width*m_height);
		}
		sort(all.begin(), all.end(), comp_area);
		del_square = all.front().area;
		//fr_coord << del_square << endl;
		//fr_coord.close();
	}

	// make changemap
	int x = 0, y = 0;
	double square = 0;
	BYTE* change_map = (BYTE*)malloc(m_height*m_width * sizeof(BYTE));
	for (int i = 0; i < m_height*m_width; i++) change_map[i] = 0;
	for (int k = 0; k < all.size(); k++) {
		for (auto it = all[k].obj_coords.begin(); it < all[k].obj_coords.end(); it++) {
			change_map[it->first*m_width + it->second] = 255;
			square += 1;
		}
	}
	//PNG_Image::SaveArrayToPNG(change_map, m_width, m_height, "change_map.png");
	return square / (double) (m_width*m_height);
}