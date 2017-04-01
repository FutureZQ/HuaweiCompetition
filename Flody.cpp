#include "Flody.h"
#include<time.h>
#define UNABEL_TO_ARRIVE -1

/*
//初始化距离矩阵
*/
void copyadlist(int** object, int** source, int vlen)
{
	for (int i = 0; i < vlen; i++)
		for (int j = 0; j < vlen; j++)
			object[i][j] = source[i][j];
}
/*
//初始化路径矩阵
*/
void initPath(int** path, int** dist, int vlen)
{
	for (int i = 0; i < vlen; i++)
		for (int j = 0; j < vlen; j++)
		{
			if (dist[i][j] < INF )
				path[i][j] = j;
			else
				path[i][j] = UNABEL_TO_ARRIVE;
		}
}
// floyd 算法，mode 用于选择输出的矩阵
int** floyd(int** adlist,int vlen,int mode)
{
	int **dist = create2DArray<int>(vlen, vlen);
	int **path = create2DArray<int>(vlen, vlen);
  
	copyadlist(dist, adlist, vlen);
	initPath(path, dist, vlen);
 	for (int k = 0; k < vlen; k++)
		for (int i = 0; i < vlen; i++)
			for (int j = 0; j < vlen; j++)
			{
				if (dist[i][j] > dist[i][k] + dist[k][j] && (i != j))
				{
					dist[i][j] = dist[i][k] + dist[k][j];
					path[i][j] = path[i][k];
				}
			}
	if (mode == 0)
	{
		delete2DArrary<int>(&dist, vlen);
		return path;
	}
	else
	{
		delete2DArrary<int>(&path, vlen);
		return dist;
	}
}


int* find_path(int **path,int startpoint,int stoppoint,int vlen)
{
	int *path_line = new int[vlen+1];
	path_line[0]=0;

	if (path[startpoint][stoppoint] == UNABEL_TO_ARRIVE)//没路径
		return path_line;

	path_line[++path_line[0]]=startpoint;//有路径
	for (int i = 0; i < vlen; i++)
	{
		int per_start_point = path_line[path_line[0]];
		path_line[++path_line[0]] = path[per_start_point][stoppoint];
		if (path_line[path_line[0]] == stoppoint)
			break;
	}

	return path_line;
}
/*
//利用邻接矩阵寻找路径
*/
int* find_path2(int** adlist, int vlen,int startpoint, int stoppoint)
{
	int** path = floyd(adlist, vlen);//计算最短路径得路径矩阵
	int *path_line = find_path(path, startpoint, stoppoint, vlen);
	delete2DArrary<int>(&path, vlen);
	return path_line;
}


void printPath(Graph* pG,int * path_line)
{
	for (int i = 1; i <= path_line[0]; i++)
	{
		if (i != path_line[0])
			cout << path_line[i] << "->";

		else //末尾一个节点
		{
			//获取末尾的点是否为消费节点相连的点
			int consumer_node = getNodeConnectConsumer(pG->consumerMatrix, pG->mVexNumConsume, path_line[i]);			

			if (consumer_node != -1)//路径终点为消费节点对应得节点
			{
				if (path_line[0] != 2 || path_line[1] != path_line[path_line[0]] )//这条路径不是消费节点直连
				{
					cout << path_line[i] << "->" <<  consumer_node << "\n";		
				}
				else//直连
				{
					cout << consumer_node << "\n";
				}
			}
			else//不是连接到消费节点
				cout << path_line[i] << "\n";
			return;
		}
	}
	cout << "\n";
}


