#ifndef __KMEANS__
#define __KMEANS__
#include "Graph.h"
#include "Flody.h"

int* findPath_Djsttra(int** dist_matrix, int node_number, int startpoint);
int* obtainPath(int* sort_path, int startpoint, int stoppoint, int node_number);


typedef struct MeansStruct
{
	int type_number; //�������
	int* k_center;  //K�������
	int **k_list;   //���ÿһ��Ľڵ�


}MeansStruct;

void deleteMeanStruct(MeansStruct **means_struct_adress);
MeansStruct* k_means(Graph *pG, int k_kind);//k����
MeansStruct* k_means2(Graph *pG, int k_kind);//��ʤ��������

#endif