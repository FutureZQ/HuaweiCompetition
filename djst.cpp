#include"djst.h"
int* inv_array(int** path_line_address)
{
	int *path_line = *path_line_address;
	//路径反序
	int path_line_length = path_line[0];
	int* new_path_line = new int[path_line_length+1];
	for (int i = 1; i <= path_line_length; i++)
	{
		new_path_line[i] = path_line[path_line_length - i + 1];
	}
	new_path_line[0] = path_line[0];
	delete[] path_line;
	return new_path_line;
}
int* obtainPath(int* sort_path, int startpoint, int stoppoint, int node_number)
{
	int* path_line = new int[node_number + 1];
	path_line[0] = 0;
	if (sort_path[stoppoint] == -1)//路径不存在
	{
		return path_line;
	}
	path_line[++path_line[0]] = stoppoint;
	int next_point = stoppoint;

	for (int i = 2; i <= node_number; i++)//一定存在路径
	{
		path_line[++path_line[0]] = sort_path[next_point];
		if (sort_path[next_point] == startpoint)
			break;
		next_point = sort_path[next_point];
	}
	//return path_line;
	return inv_array(&path_line);
}

int*  findPath_Djsttra(int** dist_matrix, int node_number, int startpoint)
{
	int sort_point, min_dis;
	int* find_flag = new int[node_number];
	int* sort_dist = new int[node_number];//存放最短路径
	int* sort_path = new int[node_number];//存放路径	

	//初始化
	for (int i = 0; i< node_number; i++)
	{
		find_flag[i] = 0;
		sort_dist[i] = dist_matrix[startpoint][i];
		if (dist_matrix[startpoint][i] != INF)
			sort_path[i] = startpoint;
		else
			sort_path[i] = -1;
	}

	//主循环
	for (int i = 0; i < node_number; i++)
	{
		min_dis = INF;
		sort_point = -1;
		for (int j = 0; j < node_number; j++)
		{
			if (!find_flag[j] && sort_dist[j] < min_dis)
			{
				sort_point = j;
				min_dis = sort_dist[j];
			}
		}
		if (sort_point == -1)//没有有路径的更短路径了
		{
			//没找到路径
			delete[] find_flag;
			delete[] sort_dist;
			return sort_path;
		}		
		find_flag[sort_point] = 1;//表示找到了该点的最短路径		
		for (int i = 0; i<node_number; i++)
		{
			if (!find_flag[i] && min_dis + dist_matrix[sort_point][i] < sort_dist[i] )
			{
				sort_dist[i] = min_dis + dist_matrix[sort_point][i];
				sort_path[i] = sort_point;
			}
		}
	}	
	delete[] find_flag;
	delete[] sort_dist;
	return sort_path;
}