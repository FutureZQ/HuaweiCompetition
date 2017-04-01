#include "k_means.h"

void printMeanResult(int *k_center,int k_kind)
{
	int count = 0;
	for (int i = 0; i < k_kind; i++)
	{
	if (k_center[i] == -1)
	continue;
	count++;
	printf("��%d�࣬���ĵ�Ϊ:%d \n", count, k_center[i]);
	//for (int j = 1; j <= k_list[i][0]; j++)
	//{
	//printf("%d\t", k_list[i][j]);
	//}
	printf("\n\n");
	}
	
}

/// �ж��б����Ƿ����ָ��Ԫ��
int Contain(int *list, int length, int object)
{
	for (int i = 0; i < length; i++)
	{
		if (list[i] == object)
			return 1;
	}
	return 0;
}

//�ж������Ƿ������ѽڵ�
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

//���¾��������
void updateKCenter(int **dist, int **k_list, int* k_center, int k_kind)
{
	for (int i = 0; i < k_kind; i++)//����ÿһ�ൽ�������ڽڵ�Ķ����
	{
		int new_center = -1;//��ʼ��
		int mini_dis = INF;
		for (int j = 1; j <= k_list[i][0]; j++)
		{
			int sum_dist = 0;
			for (int k = 1; k <= k_list[i][0]; k++)
			{
				int startpoint = k_list[i][j];
				int stoppoint = k_list[i][k];
				sum_dist += dist[startpoint][stoppoint];//���㵽�������ڽڵ�ľ����
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
//������
void combine(int** k_list, int kind1, int kind2, int *k_center, int** dist)
{
	if (kind1 == kind2)
		return;
	int kind2_length = k_list[kind2][0];
	for (int i = 0; i < kind2_length; i++)//�ϲ���
	{
		int k1_number = k_list[kind1][0];
		int k2_number = k_list[kind2][0];
		k_list[kind1][k1_number + 1] = k_list[kind2][k2_number];
		k_list[kind1][0]++;
		k_list[kind2][0]--;
	}
	int new_center = k_center[kind1];//��ʼ��
	int mini_dis = INF;
	for (int i = 1; i <= k_list[kind1][0]; i++)
	{
		int sum_dist = 0;
		for (int j = 0; j <= k_list[kind1][0]; j++)
		{
			int startpoint = k_list[kind1][i];
			int stoppoint = k_list[kind1][j];
			sum_dist += dist[startpoint][stoppoint];//���㵽�������ڽڵ�ľ����
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
// Ѱ���������, �����������
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


//Ѱ�Ҽ������һ���������ĵľ���
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

//Mean++ ѡ��ʼֵ
int* obtainInitialCenter(int** dist, int k_kind, int node_number)
{
	srand((unsigned)time(NULL));
	int* k_center = new int[node_number];
	int* D = new int[node_number];
	int center_decided = 1;
	k_center[0] = abs(rand() % node_number);

	for (int k = 0; k < k_kind - 1; k++)
	{
		//����ÿ���㵽����������ĵľ���
		for (int i = 0; i < node_number; i++)
		{
			D[i] = findMinDis2center(dist, k_center, i, center_decided);
		}

		//���ݲ�ͬ�������ѡȡ

		int sum_dist = 0;
		for (int i = 0; i < node_number; i++)//�������ܳ���
		{
			sum_dist += findMinDis2center(dist, k_center, i, center_decided);
		}
		int rand_range = rand() % sum_dist;//����齱ֵ
		int new_node;//��¼�µĽڵ�
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
	int ** distance2center = create2DArray<int>(k_kind, vlen);//���ÿ���ڵ㵽ÿ�����ĵľ���	
	int **k_list = create2DArray<int>(k_kind, vlen + 1);//������ڵĽڵ�

	//srand((unsigned)time(NULL));
	//for (int i = 0; i < k_kind; i++) //�������k������
	//{
	//	int temp = rand() % vlen;
	//	if (Contain(k_center, i, temp))
	//	{
	//		i--; //��������һ��
	//	}
	//	else
	//	{
	//		k_center[i] = temp;
	//	}
	//}
	while (true)
	{
		int break_flag = 1;
		int* old_k_center = new int[k_kind];//���������
		for (int i = 0; i < k_kind; i++)
		{
			old_k_center[i] = k_center[i];
		}

		//����ÿһ�㵽�����������ĵľ���
		for (int i = 0; i < k_kind; i++)
		{
			for (int j = 0; j < vlen; j++)
			{
				if (k_center[i] == -1)//�������޽ڵ�
					distance2center[i][j] = INF;
				else
					distance2center[i][j] = dist[j][k_center[i]];
			}
		}

		//����ÿ���ڵ���������
		for (int i = 0; i < k_kind; i++)//��ʼ��
		{
			k_list[i][0] = 0;//k_list[i][0]��¼��i ���м����ڵ�
		}
		for (int i = 0; i < vlen; i++)//���ڵ㰴�������
		{
			int min_distance = INF;
			int min_index = 0;//��¼�������
			for (int j = 0; j < k_kind; j++)
			{
				if (distance2center[j][i] < min_distance)
				{
					min_distance = distance2center[j][i];
					min_index = j;//��������
				}
			}
			int number_per_class = k_list[min_index][0];//ÿһ�������нڵ���
			k_list[min_index][++number_per_class] = i;
			k_list[min_index][0]++;
		}

		//���¾�������
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
		if (break_flag == 1)//����
			break;
	}

	//�ϲ����������
	int T = 0;//�ϲ���ֵ	
	for (int i = 0; i < k_kind; i++)
	{
		for (int j = 0; j < k_kind; j++)
		{
			if (i != j && k_list[i][0] != 0 && k_list[j][0] != 0
				&& dist[k_center[i]][k_center[j]] <= T) //�ϲ�
			{
				combine(k_list, i, j, k_center, dist);
			}
		}
	}

	//�ϲ����������ѽڵ����
	for (int i = 0; i < k_kind; i++)
	{
		if (k_list[i][0] != 0 && !containConsumerNode(pG, k_list[i]))
		{
			int near_index = findNearClass(dist, k_center, k_kind, i);
			combine(k_list, i, near_index, k_center, dist);
			if (!containConsumerNode(pG, k_list[i]))//�ϲ������ɲ��������ѽڵ�
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


	//printMeanResult(k_center, k_kind);//��ӡ������
	return means_strucr;
}


//��ʤ�ķ�����
MeansStruct* k_means2(Graph *pG, int k_kind)
{
	MeansStruct* means_struct = new MeansStruct;
	int vlen = pG->mVexNumNet;
	int vlen_consumer = pG->mVexNumConsume;
	int **adlist = getOneValueAdMatrix(pG->adMatrix, vlen, 1);//�����ڽӾ���
	int **dist = floyd(adlist, vlen, 1);
	int *k_center = obtainInitialCenter(dist, k_kind, vlen);//new int[k_kind];
	int *list_netnode2consumers = new int[vlen];//�ڵ㵽ĳһ��ľ����
	int **k_list_consumer = create2DArray<int>(k_kind, vlen_consumer);//������ڵ����ѽڵ�

	//srand((unsigned)time(NULL));

	//for (int i = 0; i < k_kind; i++) //�������k������
	//{
	//	int temp = abs(rand() % vlen);
	//	if (Contain(k_center, i, temp))
	//	{
	//		i--; //��������һ��
	//	}
	//	else
	//	{
	//		k_center[i] = temp;
	//	}
	//}
	while (true)
	{
		int break_flag = 1;
		int* old_k_center = new int[k_kind];//���������
		for (int i = 0; i < k_kind; i++)
		{
			old_k_center[i] = k_center[i];
		}

		//�����ѽڵ����
		for (int i = 0; i < k_kind; i++)//����б�
		{
			k_list_consumer[i][0] = 0;//k_list[i][0]��¼��i ���м����ڵ�
		}
		for (int i = 0; i < vlen_consumer; i++)
		{
			int min_value = INF + 1;
			int min_index = -1;
			int consumer_node = getCosumerConnectNode(pG->consumerMatrix, i);
			for (int j = 0; j < k_kind; j++)
			{
				if (k_center[j] == -1)//����Ϊ��
					continue;

				int center_point = k_center[j];
				if (dist[consumer_node][center_point] <= min_value)
				{
					min_index = j;
					min_value = dist[consumer_node][center_point];
				}
			}
			int mini_kind_node_number = k_list_consumer[min_index][0];
			k_list_consumer[min_index][++mini_kind_node_number] = consumer_node;//�����
			k_list_consumer[min_index][0]++;
		}

		//���¼�����������
		for (int i = 0; i < k_kind; i++)//��i�������
		{
			int min_dist = INF + 1;
			int min_index = -1;
			for (int j = 0; j < vlen; j++)//����ÿ���ڵ㵽��i��ľ����
			{
				if (k_list_consumer[i][0] == 0)//��i��û�нڵ�
					break;
				list_netnode2consumers[j] = 0;
				for (int k = 1; k <= k_list_consumer[i][0]; k++)
				{
					//k_list_consumer[i][k]��ʾ��i��ĵ�k���ڵ�
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
		if (break_flag == 1)//����
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

	//printMeanResult(k_center, k_kind);//��ӡ������
	return means_struct;

}
//ɾ��MeansStruct
void deleteMeanStruct(MeansStruct **means_struct_adress)
{
	MeansStruct *means_struct = *means_struct_adress;
	delete2DArrary<int>(&means_struct->k_list, means_struct->type_number);
	delete[] means_struct->k_center;
	delete means_struct;
}