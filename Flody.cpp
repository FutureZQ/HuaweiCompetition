#include "Flody.h"
#include<time.h>
#define UNABEL_TO_ARRIVE -1

/*
//��ʼ���������
*/
void copyadlist(int** object, int** source, int vlen)
{
	for (int i = 0; i < vlen; i++)
		for (int j = 0; j < vlen; j++)
			object[i][j] = source[i][j];
}
/*
//��ʼ��·������
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
// floyd �㷨��mode ����ѡ������ľ���
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

	if (path[startpoint][stoppoint] == UNABEL_TO_ARRIVE)//û·��
		return path_line;

	path_line[++path_line[0]]=startpoint;//��·��
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
//�����ڽӾ���Ѱ��·��
*/
int* find_path2(int** adlist, int vlen,int startpoint, int stoppoint)
{
	int** path = floyd(adlist, vlen);//�������·����·������
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

		else //ĩβһ���ڵ�
		{
			//��ȡĩβ�ĵ��Ƿ�Ϊ���ѽڵ������ĵ�
			int consumer_node = getNodeConnectConsumer(pG->consumerMatrix, pG->mVexNumConsume, path_line[i]);			

			if (consumer_node != -1)//·���յ�Ϊ���ѽڵ��Ӧ�ýڵ�
			{
				if (path_line[0] != 2 || path_line[1] != path_line[path_line[0]] )//����·���������ѽڵ�ֱ��
				{
					cout << path_line[i] << "->" <<  consumer_node << "\n";		
				}
				else//ֱ��
				{
					cout << consumer_node << "\n";
				}
			}
			else//�������ӵ����ѽڵ�
				cout << path_line[i] << "\n";
			return;
		}
	}
	cout << "\n";
}


