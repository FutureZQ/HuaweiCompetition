#include "Graph.h"

int startime;//ʱ���¼
int stoptime;//ʱ��ֹͣ��¼;

int getCosumerConnectNode(int **consumerMatrix, int consumer_node)
{
	return consumerMatrix[consumer_node][1];
}
int getNodeConnectConsumer(int **consumerMatrix, int vlen_consumer, int net_node)
{
	for (int i = 0; i < vlen_consumer; i++)
	{
		if (consumerMatrix[i][1] == net_node)
			return i;
	}
	return -1;
}
/*
// ����ͼ�е��ڽӾ���position=0 Ϊ����1Ϊ����
*/
int **getOneValueAdMatrix(int*** ad_matrix, int vlen, int position)
{
	int **admatrix_one_value = create2DArray<int>(vlen, vlen);
	for (int i = 0; i < vlen; i++)
		for (int j = 0; j < vlen; j++)
			admatrix_one_value[i][j] = ad_matrix[i][j][position];
	return admatrix_one_value;
}

//�����ڽӾ���
int *** createAdMatrix(char* topo[],int **consumerMatrix)
{
	int elen;
	int vlen_node, vlen_consume;
	//����ͼ�����Ϣ
	sscanf(topo[0], "%d %d %d", &vlen_node, &elen, &vlen_consume);

	//��ʼ���ڽӾ���
	int ***ad_matrix = new int**[vlen_node];
	for (int i = 0; i < vlen_node; i++)
	{
		ad_matrix[i] = new int*[vlen_node];
		for (int j = 0; j < vlen_node; j++)
		{
			if (i == j)
			{
				int consumer = getNodeConnectConsumer(consumerMatrix, vlen_consume, i);
				if ( consumer  !=  -1)//��ʾ���������ѽڵ�
				{
					
					ad_matrix[i][j] = new int[2]{ consumerMatrix[consumer][2],0 }; //��¼���ѽڵ����
				}
				else
				{
					ad_matrix[i][j] = new int[2]{ INF,0 }; //�Լ����Լ�
				}
			}
			else
				ad_matrix[i][j] = new int[2]{ CUT,INF };
		}
	}

	//������Ϣ
	for (int i = 0; i < elen; i++)
	{
		int startpoint, stoppoint, band, cost;
		sscanf(topo[i + 4], "%d %d %d %d", &startpoint, &stoppoint, &band, &cost);//+4Ϊ��ʽ
		ad_matrix[startpoint][stoppoint][0] = band;
		ad_matrix[startpoint][stoppoint][1] = cost;
		ad_matrix[stoppoint][startpoint][0] = band;
		ad_matrix[stoppoint][startpoint][1] = cost;
	}
	return ad_matrix;

}
//ɾ���ڽӾ���
void deleteAdMatrix(int ****admatrix_address, int size)
{
	int ***ad_matrix = *admatrix_address;
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			delete[] ad_matrix[i][j];
		}
		delete[] ad_matrix[i];
	}
	delete ad_matrix;
}
int** createConsumerMatrix(char* topo[])
{
	int elen;
	int vlen_node, vlen_consume;
	//����ͼ�����Ϣ
	sscanf(topo[0], "%d %d %d", &vlen_node, &elen, &vlen_consume);

	int** consumerMatrix = new int*[vlen_consume];
	for (int i = 0; i < vlen_consume; i++)
	{
		consumerMatrix[i] = new int[3];
	}

	for (int i = 0; i < vlen_consume; i++)
	{
		int startpoint, stoppoint, band;
		sscanf(topo[i + 5+ elen], "%d %d %d", &startpoint, &stoppoint, &band);//+5Ϊ��ʽ
		consumerMatrix[i][0] = startpoint;
		consumerMatrix[i][1] = stoppoint;
		consumerMatrix[i][2] = band;
	}
	return consumerMatrix;
}

Graph* createGraph(char * topo[], int line_num)
{
	int elen;
	int vlen_node,vlen_consume;
	int server_price;

	//����ͼ�����Ϣ
	sscanf(topo[0],"%d %d %d",&vlen_node,&elen,&vlen_consume);	
	sscanf(topo[2], "%d", &server_price);


	//����ͼ
	Graph* pG=new Graph;
	pG->mEdgNum = elen;
	pG->mVexNumConsume = vlen_consume;
	pG->mVexNumNet = vlen_node;
	pG->serverPrice = server_price;
	pG->consumerMatrix = createConsumerMatrix(topo);
	pG->adMatrix = createAdMatrix(topo, pG->consumerMatrix);

	return pG;
}
void deleteGraph(Graph **graph_adress)
{
	Graph* graph = *graph_adress;
	deleteAdMatrix(&(graph->adMatrix), graph->mVexNumNet);
	delete2DArrary<int>(&graph->consumerMatrix, graph->mVexNumConsume);
	delete graph;

}
