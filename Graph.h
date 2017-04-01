#ifndef __GRAPH_H__
#define __GRAPH_H__
#include <string>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <vector>
#include <stdio.h>

using namespace std;

#define INF 65535
#define CUT 0

extern int startime;//ʱ���¼
extern int stoptime;//ʱ��ֹͣ��¼;


template <typename T>
T** create2DArray(int line, int column)//������ά����
{
	T **array = new T*[line];
	for (int i = 0; i<line; i++)
	{
		array[i] = new T[column];  
	}
	return array;
}

template <typename T>
void delete2DArrary(T ***adress_2d_array, int dem)//ɾ����ά����
{

	T ** array_2d = *adress_2d_array;
	for (int i = 0; i < dem; i++)
		delete[] array_2d[i];
	delete array_2d;
	return;
}

typedef struct Graph
{
		int mVexNumConsume;		//���ѽڵ���Ŀ
		int mVexNumNet;			//����ڵ���Ŀ
		int mEdgNum;            // ͼ�ıߵ���Ŀ
		int serverPrice;		//ÿһ̨�������ļ۸�

		int *** adMatrix;			//����ڵ��ڽӾ���
		int **consumerMatrix;		//���ѽڵ��¼����

}Graph;

Graph* createGraph(char * topo[], int line_num);//����ͼ
void deleteGraph(Graph **graph_adress);//ɾ��ͼ
int getCosumerConnectNode(int **consumerMatrix, int consumer_node);//��ȡ�����ѽڵ����ӵĽڵ�
int getNodeConnectConsumer(int **consumerMatrix, int vlen_consumer, int net_node);//��ȡ�ýڵ����ӵ����ѡ�

int **getOneValueAdMatrix(int*** ad_matrix, int vlen, int position);//����һ���ڽӾ���


#endif

