#include "function_declarations.h"

void 
PageRank_iterations1(int N, double** hyperlink_matrix, double d, double epsilon, double* scores)
{
	double* X[] = {NULL, NULL};
	u32* dangling = NULL;

	do
	{
		BIF(X[0] = (double*)malloc(N*sizeof(double)), "allocate X0");
		BIF(X[1] = (double*)malloc(N*sizeof(double)), "allocate X1");
		for(u32 i = 0; i < N; ++i) X[0][i] = 1.0/N;
		

		BIF(dangling = (u32*)calloc(N, sizeof(u32)), "allocate dangling");
		for(u32 i = 0; i < N; ++i) for(u32 j = 0; j < N; j++) dangling[j] += (u32)N*hyperlink_matrix[i][j];
		
		u32 danglingSize = 0;
		for(u32 i = 0; i < N; ++i) 
		{
			if(!dangling[i])
			{
				dangling[danglingSize] = i;
				++danglingSize;
			}
		}

		double W = (double)danglingSize/N;
		double C = (double)(1-d)/N;

		u8 last = 0;
		u8 cur = 1;
		while(1)
		{
			for(u32 i = 0; i < N; ++i)
			{
				X[cur][i] = 0;
				for(u32 j = 0; j < N; ++j) X[cur][i] += hyperlink_matrix[i][j] * X[last][j];
				X[cur][i] = d*(X[cur][i] + W/N) + C;
			}
	
			double W_new = 0;
			for(u32 i = 0; i < danglingSize; ++i) W_new += X[cur][dangling[i]];

			double stopCrit = 0;
			for(u32 i = 0; i < N; ++i) if(X[cur][i] - X[last][i] > stopCrit) stopCrit = X[cur][i] - X[last][i];
			if(stopCrit < epsilon) break;
			else
			{
				last ^= cur;
				cur ^= last;
				last ^= cur;
			}
		}

		for(u32 i = 0; i < N; ++i) scores[i] = X[cur][i];

		//Success return
		free(X[0]);
		free(X[1]);
		free(dangling);
		return;
	
	} while(0);

	//Failure return
	
	free(X[0]);
	free(X[1]);
	free(dangling);
	return;
}


