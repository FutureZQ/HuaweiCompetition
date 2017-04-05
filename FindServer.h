#ifndef __FINDSERVERS_H__
#define __FINDSERVERS_H__
#include "Graph.h"
#include "Flody.h"
#include "djst.h"
#include "k_means.h"

typedef struct PathInfo
{
	vector<int*> path_lines;//���·��
	vector<int> path_cost;
	vector<int> path_band;
	int node_number;
	int total_price;

}PathInfo;
void deletePathInfo(PathInfo** path_info_address);
PathInfo* findServersAndPath(Graph* pG, vector<int> server_individual);
void adjustPathInfo(PathInfo* path_info);//����·������

extern PathInfo* global_op_path_info;

#endif