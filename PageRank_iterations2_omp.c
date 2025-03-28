#include "function_declarations.h"
#include <omp.h>

void 
PageRank_iterations2(int N, int* row_ptr, int* col_idx, double* val, double d, double epsilon, double* scores)
{
	double* X[] = {NULL, NULL};
	u32* dangling = NULL;
	u32* t_dangling = NULL;

	do
	{
		BIF(X[0] = (double*)malloc(N*sizeof(double)), "allocate X0");
		BIF(X[1] = (double*)malloc(N*sizeof(double)), "allocate X1");
		BIF(dangling = (u32*)calloc(N, sizeof(u32)), "allocate dangling");
	
		#pragma omp parallel
		{
			#pragma omp master
			{
				BIF(t_dangling = (u32*)calloc(N*omp_get_num_threads()*sizeof(u32)), "allocate t_dangling"); 
			}

			#pragma omp for
			for(u32 i = 0; i < N; ++i) X[0][i] = 1.0/N;
			
	
			u32 t_num = omp_get_thread_num();
			u32 t_index = N*t_num;
			#pragma omp for
			for(u32 i = 0; i < row_ptr[N]; ++i) ++dangling[t_index + col_idx[i]];

			
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
					for(u32 j = 0; j < row_ptr[i+1]-row_ptr[i]; ++j) X[cur][i] += val[row_ptr[i] + j] * X[last][col_idx[row_ptr[i] + j]];
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
		}


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
	free(t_dangling);
	return;
}


