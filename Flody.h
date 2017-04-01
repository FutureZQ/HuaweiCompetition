#ifndef __FLODY_H__
#define __FLODY_H__

#include<stdlib.h>
#include<cmath>
#include "Graph.h"

int** floyd(int** adMatrix,int vlen,int mode=0);
int* find_path(int **path, int startpoint, int stoppoint, int vlen);
void printPath(Graph* pG, int * path_line);




#endif