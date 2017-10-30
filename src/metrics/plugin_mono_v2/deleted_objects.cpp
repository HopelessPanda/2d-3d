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

using namespace std;
using namespace cv;

#define THLD 70

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


int check_neighbours(int i, int j, int* map_checked, object &obj, int m_width, int m_height, BYTE *error_map)
{
    /*
    int log = 1;
    int what = obj.index;
    // cout << "recieved " << i << ' ' << j << endl;
    for (int k = -1; k < 2; k++) {
        for (int l = -1; l < 2; l++) {
            if (i + k < 10 || i + k > m_height - 10 || j + l < 10 || j + l > m_width - 10 || (k == 0 && l == 0)) continue;
            int index = (i + k)*m_width + (j + l);
            if (log == what) cout << i + k << "-" << j + l << " is checked " << map_checked[index] << endl;              // 442150
            if (map_checked[index] > -1 && check_error(error_map, index)) {
                if (log == what) cout << i + k << "-" << j + l << " is error " << check_error(error_map, index) << endl;
                obj.obj_coords.push_back(index);
                map_checked[index] = map_checked[i*m_width + j];
                // if (log == what) cout << "recursive\n";
                // check_neighbours(i + k, j + l, map_checked, obj, m_width, m_height, error_map);
                // if (log == what) cout << "returning from recursion\n";
            }
            else {
                //obj.edge_coords.push_back(index);
            }
            map_checked[index] = -1;
        }
    }
    */
    return 0;

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

double CMonoPlugin::FindDeleted(double alpha, int framenumber)
{
    LOG log(false);
    log.vis_grey_image(m_Mono.GetData(), m_height, m_width, "mono.png");
    log.vis_grey_image(m_Left.GetData(), m_height, m_width, "left.png");

    cMV* me_NtoL = m_analyser.GetME(3)->Field(1); // NtoL
    cMV* me_NtoR = m_analyser.GetME(4)->Field(1); // NtoR
    cMV* me_LtoR = m_analyser.GetME(0)->Field(0); // LtoR

    BYTE* map_y = (BYTE*)malloc(m_height*m_width * sizeof(BYTE));
    BYTE* map_error_NtoL = (BYTE*)malloc(m_height*m_width * sizeof(BYTE));
    BYTE* map_x_NtoL = (BYTE*)malloc(m_height*m_width * sizeof(BYTE));
    double max_error = 0, min_error = 10000000;

    for (int i = 0; i < m_height; i++) {
        for (int j = 0; j < m_width; j++) {
            int index = i*m_width + j;
            map_x_NtoL[index] = abs(me_NtoL[index].x);

            // find max and min in error confidence
            if (abs(me_NtoL[index].error) > max_error) max_error = abs(me_NtoL[index].error);
            if (abs(me_NtoL[index].error) < min_error) min_error = abs(me_NtoL[index].error);
        }
    }
    log.vis_grey_image(map_x_NtoL, m_height, m_width, "x_vector_NtoL.png");

    double coef = (double)255 / (max_error - min_error);
    cout << max_error << ' ' << min_error << ' ' << coef << endl;
    for (int i = 0; i < m_height; i++) {
        for (int j = 0; j < m_width; j++) {
            int index = i*m_width + j;
            map_error_NtoL[index] = abs(me_NtoL[index].error) * coef; // for vis
        }
    }
    //log.vis_grey_image(map_error_NtoL, m_height, m_width, "error_in_NtoL.png");

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
            if (map_error_NtoL[index] > THLD) { // found vector with large error - candidate

                object found_obj;
                pair<int, int> e, w;
                found_obj.index = count;
                q.push(std::make_pair(i, j));

                while (!q.empty()) {
                    //cout << "front " << q.front().first << ' ' << q.front().second << endl;
                    w.first = e.first = q.front().first;
                    w.second = e.second = q.front().second;
                    while (e.second < m_width - 1 && map_error_NtoL[e.first*m_width + e.second] > THLD && map_checked[e.first*m_width + e.second] == 0) {
                        e.second++;
                    }
                    if (e.second < m_width - 1)
                        found_obj.edge_coords.push_back(std::make_pair(e.first, e.second + 1));
                    while (w.second > 0 && map_error_NtoL[w.first*m_width + w.second] > THLD && map_checked[e.first*m_width + e.second] == 0) {
                        w.second--;
                    }
                    if (w.second > 0)
                        found_obj.edge_coords.push_back(std::make_pair(w.first, w.second - 1));
                    for (int k = w.second; k < e.second; k++) {
                        found_obj.obj_coords.push_back(std::make_pair(e.first, k));
                        map_checked[e.first*m_width + k] = 1;
                        if (e.first - 1 > 0) {
                            if (map_error_NtoL[(e.first - 1)*m_width + k] > THLD && map_checked[(e.first - 1)*m_width + k] == 0) q.push(std::make_pair(e.first - 1, k));
                            else found_obj.edge_coords.push_back(std::make_pair(e.first - 1, k));
                        }
                        if (e.first + 1 < m_height) {
                            if (map_error_NtoL[(e.first + 1)*m_width + k] > THLD && map_checked[(e.first + 1)*m_width + k] == 0) q.push(std::make_pair(e.first + 1, k));
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
    BYTE *depth_NtoL = (BYTE*)malloc(m_width*m_height * sizeof(BYTE));
    m_analyser.L_TO_N.GetDepthMap(depth_NtoL, 1);
    //log.vis_grey_image(depth_NtoL, m_height, m_width, "_depth.png", framenumber);

    for (int i = 0; i < all.size(); i++) {
        vector<int> depths;
        for (auto j = all[i].edge_coords.begin(); j != all[i].edge_coords.end(); j++) {
            depths.push_back(depth_NtoL[j->first * m_width + j->second]);
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

    // check color, if differs -> deleted
    /*
    for (int i = 0; i < all.size(); i++) {
        vector<int> xs;
        int av_x, av_y, sum_x = 0, sum_y = 0;
        for (auto j = all[i].edge_coords.begin(); j != all[i].edge_coords.end(); j++) {
            xs.push_back(me_NtoL[j->first*m_width + j->second].x);
            sum_x += me_NtoL[j->first*m_width + j->second].x;
            sum_y += me_NtoL[j->first*m_width + j->second].y;
        }
        av_x = sum_x / all[i].edge_coords.size();
        av_y = sum_y / all[i].edge_coords.size();

        m_Mono.ConvertToType(0);
        m_Left.ConvertToType(0);
        int m_b = 0, m_g = 0, m_r = 0;
        int l_b = 0, l_g = 0, l_r = 0;
        for (auto k = all[i].obj_coords.begin(); k != all[i].obj_coords.end(); k++) {
            int j = k->first + av_y;
            if (j < 0 || j > m_height - 1) continue;
            int t = k->second + av_x;
            if (t < 0 || t > m_width - 1) continue;
            m_b += m_Mono.GetData()[(j*m_width + t) * 3];     // = b(i, j);
            m_g += m_Mono.GetData()[(j*m_width + t) * 3 + 1]; // = g(i, j);
            m_r += m_Mono.GetData()[(j*m_width + t) * 3 + 2]; // = r(i, j);

            l_b += m_Left.GetData()[(j*m_width + t) * 3];     // = b(i, j);
            l_g += m_Left.GetData()[(j*m_width + t) * 3 + 1]; // = g(i, j);
            l_r += m_Left.GetData()[(j*m_width + t) * 3 + 2]; // = r(i, j);
        }
        int total = all[i].obj_coords.size();
        m_b /= total;
        m_g /= total;
        m_r /= total;

        l_b /= total;
        l_g /= total;
        l_r /= total;

        if (abs(m_b - l_b) > 3 || abs(m_g - l_g) > 3 || abs(m_r - l_r) > 3) {
            log.print("great difference in color, gooood");
        }
        else {
            log.print("small difference in color");
            //all.erase(all.begin() + i);
        }
    }
    */
    if (all.size() == 0) return 0;

    // find coordinates of found del object   
    if (all.size() > 0) {
        //fstream fr, fr_coord;
        //fr.open("d_frame order.txt", std::ios_base::app);
        //fr_coord.open("d_coordinates in frame and area.txt", std::ios_base::app);

        //fr << framenumber<< endl;

        for (int i = 0; i < all.size(); i++) {
            all[i].obj_coords;
            sort(all[i].edge_coords.begin(), all[i].edge_coords.end(), comp_hor);
            int x_min = all[i].edge_coords.back().second;
            int x_max = all[i].edge_coords.front().second;
            sort(all[i].edge_coords.begin(), all[i].edge_coords.end(), comp_ver);
            int y_max = all[i].edge_coords.front().first;
            int y_min = all[i].edge_coords.back().first;
            //fr_coord << framenumber << ' ' << x_min << ' ' << x_max << ' ' << y_min << ' ' << y_max << endl;
			all[i].area = (x_max - x_min)*(y_max - y_min) / (double)(m_width*m_height);
        }
		sort(all.begin(), all.end(), comp_area);
		del_square = all.front().area;
		//fr_coord << del_square << endl;

        //fr.close();
        //fr_coord.close();
    }

    // make changemap
    if (log.log) {
        for (int i = 0; i < m_height; i++) {
            for (int j = 0; j < m_width; j++) {
                int index = i*m_width + j;
                if (map_checked[index] != 0) map_y[index] = 255;
                else map_y[index] = 0;
            }
        }
    }

    free(map_error_NtoL);
    free(map_checked);
    free(map_x_NtoL);
    free(map_y);
	return del_square;
}