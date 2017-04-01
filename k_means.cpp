#include "k_means.h"

void printMeanResult(int *k_center,int k_kind)
{
	int count = 0;
	for (int i = 0; i < k_kind; i++)
	{
	if (k_center[i] == -1)
	continue;
	count++;
	printf("第%d类，中心点为:%d \n", count, k_center[i]);
	//for (int j = 1; j <= k_list[i][0]; j++)
	//{
	//printf("%d\t", k_list[i][j]);
	//}
	printf("\n\n");
	}
	
}

/// 判断列表里是否存在指定元素
int Contain(int *list, int length, int object)
{
	for (int i = 0; i < length; i++)
	{
		if (list[i] == object)
			return 1;
	}
	return 0;
}

//判断类内是否有消费节点
int containConsumerNode(Graph *pG, int* per_k_list)
{
	for (int i = 1; i <= per_k_list[0]; i++)
	{
		if (getNodeConnectConsumer(pG->consumerMatrix, pG->mVexNumConsume, per_k_list[i]) != -1)
		{
			return 1;
		}
	}
	return 0;
}

//更新聚类的中心
void updateKCenter(int **dist, int **k_list, int* k_center, int k_kind)
{
	for (int i = 0; i < k_kind; i++)//计算每一类到所有类内节点的额距离
	{
		int new_center = -1;//初始化
		int mini_dis = INF;
		for (int j = 1; j <= k_list[i][0]; j++)
		{
			int sum_dist = 0;
			for (int k = 1; k <= k_list[i][0]; k++)
			{
				int startpoint = k_list[i][j];
				int stoppoint = k_list[i][k];
				sum_dist += dist[startpoint][stoppoint];//计算到所有类内节点的距离和
			}
			if (sum_dist < mini_dis)
			{
				mini_dis = sum_dist;
				new_center = k_list[i][j];
			}
		}
		k_center[i] = new_center;
	}
}
//联合类
void combine(int** k_list, int kind1, int kind2, int *k_center, int** dist)
{
	if (kind1 == kind2)
		return;
	int kind2_length = k_list[kind2][0];
	for (int i = 0; i < kind2_length; i++)//合并类
	{
		int k1_number = k_list[kind1][0];
		int k2_number = k_list[kind2][0];
		k_list[kind1][k1_number + 1] = k_list[kind2][k2_number];
		k_list[kind1][0]++;
		k_list[kind2][0]--;
	}
	int new_center = k_center[kind1];//初始化
	int mini_dis = INF;
	for (int i = 1; i <= k_list[kind1][0]; i++)
	{
		int sum_dist = 0;
		for (int j = 0; j <= k_list[kind1][0]; j++)
		{
			int startpoint = k_list[kind1][i];
			int stoppoint = k_list[kind1][j];
			sum_dist += dist[startpoint][stoppoint];//计算到所有类内节点的距离和
		}
		if (sum_dist < mini_dis)
		{
			mini_dis = sum_dist;
			new_center = k_list[kind1][i];
		}
	}
	k_center[kind1] = new_center;
	k_center[kind2] = -1;
}
// 寻找最近的类, 返回类的索引
int findNearClass(int** dist, int* k_center, int k_kind, int object_index)
{
	int mini_dist = INF;
	int mini_index = object_index;
	for (int i = 0; i < k_kind; i++)
	{
		if (i != object_index && k_center[i] != -1 &&
			dist[k_center[object_index]][k_center[i]]<mini_dist)
		{
			mini_dist = dist[k_center[object_index]][k_center[i]];
			mini_index = i;
		}
	}
	return mini_index;
}


//寻找计算最近一个聚类中心的距离
int findMinDis2center(int **dist, int* k_center, int object, int center_decided)
{
	int min = INF;
	for (int i = 0; i < center_decided; i++)
	{
		int center = k_center[i];
		if (min > dist[object][center])
		{
			min = dist[object][center];
		}
	}
	return min;
}

//Mean++ 选初始值
int* obtainInitialCenter(int** dist, int k_kind, int node_number)
{
	srand((unsigned)time(NULL));
	int* k_center = new int[node_number];
	int* D = new int[node_number];
	int center_decided = 1;
	k_center[0] = abs(rand() % node_number);

	for (int k = 0; k < k_kind - 1; k++)
	{
		//计算每个点到最近聚类中心的距离
		for (int i = 0; i < node_number; i++)
		{
			D[i] = findMinDis2center(dist, k_center, i, center_decided);
		}

		//根据不同概率随机选取

		int sum_dist = 0;
		for (int i = 0; i < node_number; i++)//求区间总长度
		{
			sum_dist += findMinDis2center(dist, k_center, i, center_decided);
		}
		int rand_range = rand() % sum_dist;//随机抽奖值
		int new_node;//记录新的节点
		for (int i = 0; i < node_number; i++)
		{
			rand_range -= D[i];
			if (rand_range <= 0)
			{
				new_node = i;
				break;
			}

		}
		k_center[center_decided] = new_node;
		center_decided++;
	}

	delete[] D;
	return k_center;
}
MeansStruct* k_means(Graph *pG, int k_kind)
{
	MeansStruct* means_strucr = new MeansStruct;
	int vlen = pG->mVexNumNet;
	int **adlist = getOneValueAdMatrix(pG->adMatrix, vlen, 1);
	int **dist = floyd(adlist, vlen, 1);
	int *k_center = obtainInitialCenter(dist, k_kind, vlen);//new int[k_kind];
	int ** distance2center = create2DArray<int>(k_kind, vlen);//存放每个节点到每类中心的距离	
	int **k_list = create2DArray<int>(k_kind, vlen + 1);//存放类内的节点

	//srand((unsigned)time(NULL));
	//for (int i = 0; i < k_kind; i++) //随机生成k个中心
	//{
	//	int temp = rand() % vlen;
	//	if (Contain(k_center, i, temp))
	//	{
	//		i--; //重新生成一个
	//	}
	//	else
	//	{
	//		k_center[i] = temp;
	//	}
	//}
	while (true)
	{
		int break_flag = 1;
		int* old_k_center = new int[k_kind];//储存旧中心
		for (int i = 0; i < k_kind; i++)
		{
			old_k_center[i] = k_center[i];
		}

		//计算每一点到各个聚类中心的距离
		for (int i = 0; i < k_kind; i++)
		{
			for (int j = 0; j < vlen; j++)
			{
				if (k_center[i] == -1)//该类内无节点
					distance2center[i][j] = INF;
				else
					distance2center[i][j] = dist[j][k_center[i]];
			}
		}

		//计算每个节点所属分类
		for (int i = 0; i < k_kind; i++)//初始化
		{
			k_list[i][0] = 0;//k_list[i][0]记录第i 类有几个节点
		}
		for (int i = 0; i < vlen; i++)//将节点按距离归类
		{
			int min_distance = INF;
			int min_index = 0;//记录最近的类
			for (int j = 0; j < k_kind; j++)
			{
				if (distance2center[j][i] < min_distance)
				{
					min_distance = distance2center[j][i];
					min_index = j;//所属归类
				}
			}
			int number_per_class = k_list[min_index][0];//每一类内现有节点数
			k_list[min_index][++number_per_class] = i;
			k_list[min_index][0]++;
		}

		//更新聚类中心
		updateKCenter(dist, k_list, k_center, k_kind);

		for (int i = 0; i < k_kind; i++)
		{
			if (k_center[i] != old_k_center[i])
			{
				break_flag = 0;
				break;
			}
		}
		delete[] old_k_center;
		if (break_flag == 1)//不变
			break;
	}

	//合并近距离分类
	int T = 0;//合并阈值	
	for (int i = 0; i < k_kind; i++)
	{
		for (int j = 0; j < k_kind; j++)
		{
			if (i != j && k_list[i][0] != 0 && k_list[j][0] != 0
				&& dist[k_center[i]][k_center[j]] <= T) //合并
			{
				combine(k_list, i, j, k_center, dist);
			}
		}
	}

	//合并不存在消费节点的类
	for (int i = 0; i < k_kind; i++)
	{
		if (k_list[i][0] != 0 && !containConsumerNode(pG, k_list[i]))
		{
			int near_index = findNearClass(dist, k_center, k_kind, i);
			combine(k_list, i, near_index, k_center, dist);
			if (!containConsumerNode(pG, k_list[i]))//合并后依旧不存在消费节点
			{
				i--;
			}
		}
	}


	means_strucr->k_center = k_center;
	means_strucr->k_list = k_list;
	means_strucr->type_number = k_kind;


	delete2DArrary<int>(&distance2center, k_kind);
	delete2DArrary<int>(&adlist, vlen);
	delete2DArrary<int>(&dist, vlen);


	//printMeanResult(k_center, k_kind);//打印分类结果
	return means_strucr;
}


//向超胜的方法！
MeansStruct* k_means2(Graph *pG, int k_kind)
{
	MeansStruct* means_struct = new MeansStruct;
	int vlen = pG->mVexNumNet;
	int vlen_consumer = pG->mVexNumConsume;
	int **adlist = getOneValueAdMatrix(pG->adMatrix, vlen, 1);//花费邻接矩阵
	int **dist = floyd(adlist, vlen, 1);
	int *k_center = obtainInitialCenter(dist, k_kind, vlen);//new int[k_kind];
	int *list_netnode2consumers = new int[vlen];//节点到某一类的距离和
	int **k_list_consumer = create2DArray<int>(k_kind, vlen_consumer);//存放类内的消费节点

	//srand((unsigned)time(NULL));

	//for (int i = 0; i < k_kind; i++) //随机生成k个中心
	//{
	//	int temp = abs(rand() % vlen);
	//	if (Contain(k_center, i, temp))
	//	{
	//		i--; //重新生成一个
	//	}
	//	else
	//	{
	//		k_center[i] = temp;
	//	}
	//}
	while (true)
	{
		int break_flag = 1;
		int* old_k_center = new int[k_kind];//储存旧中心
		for (int i = 0; i < k_kind; i++)
		{
			old_k_center[i] = k_center[i];
		}

		//将消费节点分类
		for (int i = 0; i < k_kind; i++)//清空列表
		{
			k_list_consumer[i][0] = 0;//k_list[i][0]记录第i 类有几个节点
		}
		for (int i = 0; i < vlen_consumer; i++)
		{
			int min_value = INF + 1;
			int min_index = -1;
			int consumer_node = getCosumerConnectNode(pG->consumerMatrix, i);
			for (int j = 0; j < k_kind; j++)
			{
				if (k_center[j] == -1)//该类为空
					continue;

				int center_point = k_center[j];
				if (dist[consumer_node][center_point] <= min_value)
				{
					min_index = j;
					min_value = dist[consumer_node][center_point];
				}
			}
			int mini_kind_node_number = k_list_consumer[min_index][0];
			k_list_consumer[min_index][++mini_kind_node_number] = consumer_node;//添加类
			k_list_consumer[min_index][0]++;
		}

		//重新计算消费中心
		for (int i = 0; i < k_kind; i++)//第i类的中心
		{
			int min_dist = INF + 1;
			int min_index = -1;
			for (int j = 0; j < vlen; j++)//计算每个节点到第i类的距离和
			{
				if (k_list_consumer[i][0] == 0)//第i类没有节点
					break;
				list_netnode2consumers[j] = 0;
				for (int k = 1; k <= k_list_consumer[i][0]; k++)
				{
					//k_list_consumer[i][k]表示第i类的第k个节点
					list_netnode2consumers[j] += dist[j][k_list_consumer[i][k]];
				}
				if (list_netnode2consumers[j] <= min_dist)
				{
					min_index = j;
					min_dist = list_netnode2consumers[j];
				}
			}
			k_center[i] = min_index;
		}
		for (int i = 0; i < k_kind; i++)
		{
			if (k_center[i] != old_k_center[i])
			{
				break_flag = 0;
				break;
			}
		}
		delete[] old_k_center;
		if (break_flag == 1)//不变
		{
			break;
		}
	}

	delete[] list_netnode2consumers;
	delete2DArrary<int>(&adlist, vlen);
	delete2DArrary<int>(&dist, vlen);


	means_struct->k_center = k_center;
	means_struct->k_list = k_list_consumer;
	means_struct->type_number = k_kind;

	//printMeanResult(k_center, k_kind);//打印分类结果
	return means_struct;

}
//删除MeansStruct
void deleteMeanStruct(MeansStruct **means_struct_adress)
{
	MeansStruct *means_struct = *means_struct_adress;
	delete2DArrary<int>(&means_struct->k_list, means_struct->type_number);
	delete[] means_struct->k_center;
	delete means_struct;
}