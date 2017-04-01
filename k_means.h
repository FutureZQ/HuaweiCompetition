#ifndef __KMEANS__
#define __KMEANS__
#include "Graph.h"
#include "Flody.h"

int* findPath_Djsttra(int** dist_matrix, int node_number, int startpoint);
int* obtainPath(int* sort_path, int startpoint, int stoppoint, int node_number);


typedef struct MeansStruct
{
	int type_number; //类的数量
	int* k_center;  //K类的中心
	int **k_list;   //存放每一类的节点


}MeansStruct;

void deleteMeanStruct(MeansStruct **means_struct_adress);
MeansStruct* k_means(Graph *pG, int k_kind);//k聚类
MeansStruct* k_means2(Graph *pG, int k_kind);//向超胜方法聚类

#endif