// Floyd.cpp: 主项目文件。
#include<stdio.h>
#include "Graph.h"
#include "Flody.h"
#include "findserver.h"
#include<fstream>

#define MAX_LINE_LEN 10000

char * worstResult(char * topo[],int line_num)
{
	char* result = new char[MAX_LINE_LEN] {0};
	for (int i = line_num - 1; i > 0; i--)
	{
		if (strncmp(topo[i], "0", 1) == 0)
		{
			char buffer[30];
			int consumer, server, band;
			sprintf(buffer, "%d", line_num - i);
			strcat(result, buffer);
			strcat(result, "\n\n");			
			for (int j = i; j < line_num; j++)
			{
				sscanf(topo[j], "%d %d %d", &consumer, &server, &band);
				sprintf(buffer, "%d %d %d\n", server, consumer, band);
				strcat(result, buffer);
			}
			return result;
		}
	}
	return NULL;
}
int read_file(char ** const buff, const unsigned int spec, const char * const filename)
{
	FILE *fp = fopen(filename, "r");
	if (fp == NULL)
	{
		cout << "Fail to open file " << filename << ", " << strerror(errno) << endl;
		return 0;
	}
	cout << "Open file " << filename << " OK\n";

	char line[MAX_LINE_LEN + 2];
	unsigned int cnt = 0;
	while ((cnt < spec) && !feof(fp))
	{
		line[0] = 0;
		if (fgets(line, MAX_LINE_LEN + 2, fp) == NULL)  continue;
		if (line[0] == 0)   continue;
		buff[cnt] = (char *)malloc(MAX_LINE_LEN + 2);
		strncpy(buff[cnt], line, MAX_LINE_LEN + 2 - 1);
		buff[cnt][MAX_LINE_LEN + 1] = 0;
		cnt++;
	}
	fclose(fp);
	cout << "There are " << cnt << " lines in file " << filename << endl;

	return cnt;
}


void printAllPath(Graph* pG,PathInfo* path_info)
{
	for ( unsigned int i = 0; i < path_info->path_lines.size(); i++)
	{		
		printPath(pG, path_info->path_lines[i]);
	}
}

//将路径信息转换成竞赛需要的格式
char *ouput_result(Graph* pG, PathInfo *path_inf)
{
	string output;
	vector<int*> all_path = path_inf->path_lines;
	vector<int> all_path_band = path_inf->path_band;
	output += to_string(all_path.size());
	output += "\n\n";
	for (unsigned int i = 0; i < all_path.size(); i++)
	{
		int *path = path_inf->path_lines[i];//路径指针，指向某条路径

		for (int j = 1; j < path[0]; j++)//输出至倒数第二个点
			output += to_string(path[j]) + " ";
		//最后替换成消费节点
		if (path[1] != path[path[0]])//如果不是直连需要把最后的节点加上
		{
			output += to_string(path[path[0]]) + " ";
		}
		output += to_string(
			getNodeConnectConsumer(pG->consumerMatrix, pG->mVexNumConsume, path[path[0]]))
			+ " ";//加入对应的消费节点

		int band = all_path_band[i];
		output += to_string(band) + "\n";
	}
	int str_size = output.length();
	char* topo_file = new char[str_size + 1];
	output.copy(topo_file, str_size, 0);
	topo_file[str_size] = '\0';
	return topo_file;
}
void write2file(char *topo_file,char* outputfilename)
{
	ofstream fout(outputfilename);
	fout << topo_file;
	fout.close();
	fout.clear();
}
void Solution(char* filename, char* outputfilename)
{
	int looptime = 10000; //迭代次数

	startime = clock();//时间开始点	
	char *topo[MAX_LINE_LEN];
	int line_num;
	char *topo_file = filename;
	line_num = read_file(topo, MAX_LINE_LEN, topo_file);

	///////////////////////正式开始/////////////////////////////////////


	Graph* pG = createGraph(topo, line_num);//建立图

	global_op_path_info = NULL;//声明最优路径指针

	while (looptime-- > 0)
	{
		PathInfo* path_info;
		path_info = findServersAndPath(pG);
		adjustPathInfo(path_info);
		if (global_op_path_info == NULL)
		{
			global_op_path_info = path_info;
		}
		else if (global_op_path_info->total_price > path_info->total_price)
		{
			deletePathInfo(&global_op_path_info);
			global_op_path_info = path_info;
		}
		else
		{
			deletePathInfo(&path_info);
		}
		if (clock() - startime > 90 * CLOCKS_PER_SEC)//超时
			break;
	}

	char* topo_output_file = ouput_result(pG, global_op_path_info);//将结果转换成可输出的格式
	write2file(topo_output_file, outputfilename);//将结果写入目标文件

	//打印结果
	//printAllPath(pG, global_op_path_info);
	printf("\nTotal Price: %d/%d\n", global_op_path_info->total_price,pG->mVexNumConsume*pG->serverPrice);
	printf("Node number: %d\n", global_op_path_info->node_number);

	//释放空间
	deletePathInfo(&global_op_path_info);
	delete[] topo_output_file;

	//输出时间
	stoptime = clock();
	cout << "Time:" << 1.0*(stoptime - startime) / 1000 << "\n";
	//system("pause");


} 

int main()
{
	Solution("case.txt", "caseresult.txt");	
	Solution("初级\\case0.txt", "result1.txt");
	Solution("中级\\case0.txt", "result2.txt");
	Solution("高级\\case0.txt", "result3.txt");
	system("pause");

}
