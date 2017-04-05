#include "FindServer.h"

PathInfo* global_op_path_info = NULL;
//����·��
void adjustPathInfo(PathInfo* path_info)
{
	for (unsigned int i = 0; i < path_info->path_lines.size(); i++)
	{
		path_info->path_lines[i] = inv_array(&path_info->path_lines[i]);
	}
}
//��¼��ʹ�õķ�����
vector<int> FindserverNode(PathInfo *path_info)
{
	vector<int> servers;
	for (unsigned int i = 0; i < path_info->path_lines.size(); i++)
	{
		int flag = 0;
		int position = path_info->path_lines[i][0];
		int node = path_info->path_lines[i][position];
		for (unsigned int j = 0; j < servers.size(); j++)
		{
			if (servers[j] == node)
				flag = 1;
		}
		if (flag == 0)
			servers.push_back(node);
	}
	return servers;
}

void caculateTotalPriceAndServerNumber(PathInfo *path_info, int server_price)
{
	int cost = 0;
	for (unsigned int i = 0; i < path_info->path_lines.size(); i++)
	{
		cost += path_info->path_band[i] * path_info->path_cost[i];
	}
	vector<int> server_used = FindserverNode(path_info);
	cost += server_price*server_used.size();
	path_info->total_price = cost;
	path_info->node_number = server_used.size();
	server_used.clear();

}
int getConsumerBand(int** band_matrix, int consumer_node)
{
	return band_matrix[consumer_node][consumer_node];
}
int caculateLinePrice(int** cost_matrix, int *path_line)
{
	int length = path_line[0];
	if (length == 0)
		return -1;
	int cost = 0;
	for (int i = 1; i < length; i++)
	{
		int startpoint = path_line[i];
		int stoppoint = path_line[i + 1];
		cost += cost_matrix[startpoint][stoppoint];
	}
	return cost;
}
int caculateBand(int** band_matrix, int *path_line)
{
	int length = path_line[0];
	if (length == 0)
		return 0;
	int min = INF;
	for (int i = 1; i < length; i++)
	{
		int startpoint = path_line[i];
		int stoppoint = path_line[i + 1];
		if (min > band_matrix[startpoint][stoppoint])
		{
			min = band_matrix[startpoint][stoppoint];
		}
	}
	return min;
}
void updateMatrix(int** band_matrix, int** cost_matrix, int* path_line, int cutband)
{
	int path_length = path_line[0];
	int consumer_node = path_line[1];//�������ѽڵ�Ĵ���
	if (band_matrix[consumer_node][consumer_node] != INF) //��ʾ�յ������ѽڵ�
	{
		band_matrix[consumer_node][consumer_node] -= cutband;
		if (band_matrix[consumer_node][consumer_node] <= 0)
			cost_matrix[consumer_node][consumer_node] = INF;
	}

	if (path_line[1] == path_line[2])//�ڵ㵽�ڵ������·��
		return;

	for (int i = 1; i < path_length; i++)//�����ڵ�����ߵĴ���
	{
		int startpoint = path_line[i];
		int stoppoint = path_line[i + 1];
		band_matrix[startpoint][stoppoint] -= cutband;
		if (band_matrix[startpoint][stoppoint] <= 0)
			cost_matrix[startpoint][stoppoint] = INF;
	}
}

void obtainServerlist(int* object_list,vector<int> server_individual, int length)
{
	object_list[0] = 0;
	for (int i = 0; i < length; i++)
	{
		if (server_individual[i] == 1)
			object_list[++object_list[0]] = i;
	}
}

void deletePathInfo(PathInfo** path_info_address)
{
	PathInfo* path_info = *path_info_address;
	for (unsigned int i = 0; i < path_info->path_lines.size(); i++)
	{
		delete[] path_info->path_lines[i];
	}
	path_info->path_lines.clear();
	path_info->path_band.clear();
	path_info->path_cost.clear();
	delete path_info;
}

//�ָ����ѽڵ����ĵĴ���
void resetMatrixAndPathInfo(Graph* pG, int** band_matrix, int** cost_matrix,PathInfo* path_info,int consumer_node)
{
	int** origin_cost_matrix = getOneValueAdMatrix(pG->adMatrix, pG->mVexNumNet, 1);
	int path_end_index = path_info->path_lines.size() - 1;//Ӧ�ò������ȫ��Ϊ�յ����
	while(path_end_index>=0 && path_info->path_lines[path_end_index][1] == consumer_node)
	{
		int *path_line = path_info->path_lines[path_end_index];
		int band = path_info->path_band[path_end_index];
		if (path_line[1] == path_line[2])////ֱ���ڵ�����쳣����������½ڵ�ֱ���Ľڵ㲻���õ��ú���
		{
			band_matrix[consumer_node][consumer_node] += band;//�ָ����ѽڵ�����
			cost_matrix[consumer_node][consumer_node] = 0;
			delete2DArrary<int>(&origin_cost_matrix, pG->mVexNumNet);
			delete[] path_line;
			path_info->path_lines.pop_back();//������·
			path_info->path_cost.pop_back();
			path_info->path_band.pop_back();
			path_end_index = path_info->path_lines.size() - 1;//����ĩβ����
			printf("resetMatrixAndPathInfo Error��������������\n");//�����쳣����������½ڵ�ֱ���Ľڵ㲻���õ��ú���
			//system("pause");
		}
		for (int i = 1; i < path_line[0]; i++)//�ָ�����ͼ۸�
		{
			int startpoint = path_line[i];
			int stoppoint = path_line[i + 1];
			band_matrix[startpoint][stoppoint] += band;//���ش���
			cost_matrix[startpoint][stoppoint] = origin_cost_matrix[startpoint][stoppoint];//�ָ��۸�
		}
		band_matrix[consumer_node][consumer_node] += band;//�ָ����ѽڵ�����
		cost_matrix[consumer_node][consumer_node] = 0;

		delete[] path_line;
		path_info->path_lines.pop_back();//������·
		path_info->path_cost.pop_back();
		path_info->path_band.pop_back();
		path_end_index = path_info->path_lines.size() - 1;//����ĩβ����
	}
	delete2DArrary<int>(&origin_cost_matrix, pG->mVexNumNet);
}
//����������·�����ѽڵ����ĵĴ���
int caculatePathBand2Consumer(PathInfo *path_info,int consumer_node)
{
	int path_end_index = path_info->path_lines.size() - 1;
	int cost = 0;
	while (path_end_index>=0 && (path_info->path_lines[path_end_index][1] == consumer_node) )
	{
		cost += path_info->path_cost[path_end_index] * path_info->path_band[path_end_index];
		path_end_index--;
	}
	return cost;
}

PathInfo* findServersAndPath(Graph* pG,vector<int> server_individual)
{
	int consumer_number = pG->mVexNumConsume;//����ڵ���Ŀ
	int netnode_number = pG->mVexNumNet;//���ѽڵ���Ŀ
	int** band_matrix = getOneValueAdMatrix(pG->adMatrix, netnode_number,0);//�������
	int** cost_matrix = getOneValueAdMatrix(pG->adMatrix, netnode_number, 1);//���Ѿ���
	
	int* serverlist = new int[netnode_number];//�洢�������б�
	obtainServerlist(serverlist, server_individual, netnode_number);//���������Ŀ������������б���

	int* consumerlist = new int[consumer_number];//�����б�
	for (int i = 0; i < consumer_number; i++)
	{
		consumerlist[i] = pG->consumerMatrix[i][1];
	}


	PathInfo *path_info = new PathInfo;
	for (int i = 0; i < consumer_number; i++)
	{
		int recaculate_flag = 0;
		int server_number = serverlist[0];
		int* opt_path_line = NULL;
		int min_cost=INF;
		int *Path = findPath_Djsttra(cost_matrix, netnode_number, consumerlist[i]);
		for (int j = 1; j <= server_number; j++)
		{
			int* path_line = obtainPath(Path, consumerlist[i], serverlist[j], netnode_number);
			if (path_line[0] == 0) //û�ҵ�·��
			{
				delete[] path_line;
				continue;
			}
			int line_cost = caculateLinePrice(cost_matrix, path_line);
			if (min_cost > line_cost)
			{

				min_cost = line_cost;
				delete[] opt_path_line;
				opt_path_line = path_line;
			}
			else
			{
				delete[] path_line;
			}
		}
		delete[] Path;
		if (opt_path_line != NULL)
		{
			int line_cost = caculateLinePrice(cost_matrix, opt_path_line);
			int line_band = caculateBand(band_matrix, opt_path_line);
			int consumer_need_band = getConsumerBand(band_matrix, opt_path_line[1]);
			if (consumer_need_band > line_band)
			{
				i--;//����һ��·��
				recaculate_flag = 1;
			}
			int min_cost = line_band > consumer_need_band ? consumer_need_band : line_band; //ȡ��Сֵ
			updateMatrix(band_matrix,cost_matrix, opt_path_line, min_cost);
			
			//��¼ֵ
			path_info->path_lines.push_back(opt_path_line);
			path_info->path_cost.push_back(line_cost);
			path_info->path_band.push_back(min_cost);
		}
		else//���·�����
		{
			resetMatrixAndPathInfo(pG, band_matrix, cost_matrix, path_info, consumerlist[i]);
			serverlist[++serverlist[0]] = consumerlist[i];
			i--; //���¿�ʼ����
			recaculate_flag = 1;
		}
		if (recaculate_flag == 0 && opt_path_line !=NULL && opt_path_line[1]!= opt_path_line[2])//��ʾ�����ѽڵ��Ѿ�������
		{
			int cost = caculatePathBand2Consumer(path_info, consumerlist[i]);
			if (cost > pG->serverPrice)//�Ƿ���ڲ���������ĳɱ�
			{
				resetMatrixAndPathInfo(pG, band_matrix, cost_matrix, path_info, consumerlist[i]);
				serverlist[++serverlist[0]] = consumerlist[i];
				i--; //���¿�ʼ����
			}
		}
	}
	caculateTotalPriceAndServerNumber(path_info, pG->serverPrice);//�����ܼ۸�

	//����ռ�

	delete2DArrary<int>(&band_matrix, netnode_number);
	delete2DArrary<int>(&cost_matrix, netnode_number);
	delete[] serverlist;
	delete[] consumerlist;


	return path_info;
}
