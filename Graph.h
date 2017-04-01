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

extern int startime;//时间记录
extern int stoptime;//时间停止记录;


template <typename T>
T** create2DArray(int line, int column)//创建二维矩阵
{
	T **array = new T*[line];
	for (int i = 0; i<line; i++)
	{
		array[i] = new T[column];  
	}
	return array;
}

template <typename T>
void delete2DArrary(T ***adress_2d_array, int dem)//删除二维矩阵
{

	T ** array_2d = *adress_2d_array;
	for (int i = 0; i < dem; i++)
		delete[] array_2d[i];
	delete array_2d;
	return;
}

typedef struct Graph
{
		int mVexNumConsume;		//消费节点数目
		int mVexNumNet;			//网络节点数目
		int mEdgNum;            // 图的边的数目
		int serverPrice;		//每一台服务器的价格

		int *** adMatrix;			//网络节点邻接矩阵
		int **consumerMatrix;		//消费节点记录矩阵

}Graph;

Graph* createGraph(char * topo[], int line_num);//导入图
void deleteGraph(Graph **graph_adress);//删除图
int getCosumerConnectNode(int **consumerMatrix, int consumer_node);//获取与消费节点连接的节点
int getNodeConnectConsumer(int **consumerMatrix, int vlen_consumer, int net_node);//获取该节点连接的消费。

int **getOneValueAdMatrix(int*** ad_matrix, int vlen, int position);//复制一份邻接矩阵


#endif

