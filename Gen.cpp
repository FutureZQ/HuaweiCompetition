#include "Gen.h"

//�����������죬p_ratoΪ������
void crossGen(vector<int> &ind1, vector<int> &ind2, double p_ratio)
{
	srand((unsigned)time(NULL));//�����������
	int length = ind1.size();
	int flag = 0;
	for (unsigned int i = 0; i < ind2.size(); i++)
	{
		if (ind2[i] != ind1[i])
			flag = 1;
	}
	if (flag == 0)//�鿴�Ƿ���ͬ����������
		return;

	flag = 0;
	while (flag == 0)
	{
		int index1 = rand() % length;
		int index2 = rand() % length;
		int min_index = index1 <= index2 ? index1 : index2;
		int max_index = index1 > index2 ? index1 : index2;
		for (int i = min_index; i <= max_index; i++)
		{
			int temp = ind1[i];
			ind1[i] = ind2[i];
			ind2[i] = temp;
			if (ind2[i] != ind1[i])
			{
				flag = 1;
			}
		}
	}
	

}
PathInfo* fitfunction(Graph *pG,vector<int> &serverList)
{
	return findServersAndPath(pG, serverList);
}

//��ʼ����Ⱥ
vector<vector<int>> generateNewIndividuals(Graph *pG,int number,double radio)
{

	srand((unsigned)time(NULL));//�����������
	int node_number = pG->mVexNumNet;
	vector<vector<int>> individuals(0,vector<int>(node_number));
	

	vector<int> individual(node_number);
	MeansStruct *ms = k_means2(pG, radio* pG->mVexNumConsume);//������������		
	for (int i = 0; i < ms->type_number; i++)
	{
		individual[i] = 0;
	}
	for (int i = 0; i < ms->type_number; i++)
	{
		int position = ms->k_center[i];
		if (position != -1)
		{
			individual[position] = 1;
		}
	}
	individuals.push_back(individual);
	deleteMeanStruct(&ms);

	///���������������
	for (int i = 1; i < number; i++)
	{
		vector<int> individual(node_number);
		for (int j = 0; j < pG->mVexNumNet; j++)
		{
			double r = (double)rand() / RAND_MAX;
			if (r <= radio*pG->mVexNumConsume/pG->mVexNumNet)
			{
				individual[j] = 1;
			}
			else
			{
				individual[j] = 0;
			}
		}
		individuals.push_back(individual);
	}

	return individuals;
}
//���ѡ��
int choiceParent(vector<int> cost)
{
	int max = 0;
	int max_index = 0;
	for (unsigned int i = 0; i < cost.size(); i++)
	{
		if (cost[i] >= max)
		{
			max = cost[i];
			max_index = i;
		}
	}
	return max_index;
}
//�Ŵ��㷨,number����Ϊż��
void genAlgorithm(Graph *pG,int individual_number,double consumer_ratio,double pm_ratio)
{

	int node_number = pG->mVexNumNet;
	vector<int> globel_op_individual(node_number);//��Ӣ��
	//��ʼ����Ⱥ
	vector<vector<int>> individual_list = generateNewIndividuals(pG, individual_number, consumer_ratio);//����individual_number��������Ϊһ����Ⱥ
	vector<int> individual_fit;

	int max_price = pG->serverPrice* pG->mVexNumConsume;
	int min_price = pG->serverPrice* pG->mVexNumConsume;

	int count = 5000;
	while (count-- >0)
	{
		int min_index = -1;
		individual_fit.clear();
		for (int i = 0; i < individual_number; i++) //������Ӧ��
		{
			PathInfo* path_info = fitfunction(pG, individual_list[i]);
			int temp_cost = max_price - path_info->total_price;
			if (temp_cost < 0) temp_cost = 0;
			individual_fit.push_back(temp_cost);
			if (min_price > path_info->total_price)
			{
				min_price = path_info->total_price;
				min_index = i;
				if (global_op_path_info != NULL)
				{
					deletePathInfo(&global_op_path_info);
				}
				global_op_path_info = path_info;
				adjustPathInfo(global_op_path_info);
			}
			else
			{
				deletePathInfo(&path_info);
			}
		}
		if(min_index!=-1) //����ʷ���Ž�����
			globel_op_individual.assign(individual_list[min_index].begin(), individual_list[min_index].end());//��¼���Ÿ���

		 ///////////////////////////////ѡȡ����//////////////////////////////////////////
		for (int i = 0; i < individual_number-1; i++)
		{
			int position= choiceParent(individual_fit);//��̭������
			individual_fit[position] = -1;
			individual_list.insert(individual_list.begin(),individual_list[position]);	
		
		}
		individual_list.insert(individual_list.begin(), vector<int>(node_number,0));//������Ž�
		individual_list[0].assign(globel_op_individual.begin(), globel_op_individual.end());
    for(int i=0;i<individual_number;i++)
		  individual_list.pop_back();//����֮ǰ�ĸ���

		/////////////////////////// �������򣨱��죩////////////////////////////////////////////////
		for (int i = 0; i < individual_number / 2; i++)//��������
		{
			crossGen(individual_list[2 * i], individual_list[2 * i + 1], pm_ratio);
		}
		if (clock() - startime >= 90 * CLOCKS_PER_SEC)
			return;
	}


}