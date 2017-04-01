#ifndef __DJST_H__
#define __DJST_H__
#include "Graph.h"

int* findPath_Djsttra(int** dist_matrix, int node_number, int startpoint);
int* obtainPath(int* sort_path, int startpoint, int stoppoint, int node_number);
int* inv_array(int** path_line_address);

#endif