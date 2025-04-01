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

		double W = 0;
		double C = (double)(1-d)/N;
		u8 last = 0;
		u8 cur = 1;
		u32 danglingSize = 0;
		double stopCrit = 0;

		u8 error = 0;
		u8 done = 0;
	
		#pragma omp parallel
		{
			#pragma omp single
			{
				t_dangling = (u32*)calloc(N*omp_get_num_threads(), sizeof(u32));
				if(!t_dangling) error = 1;
			}
	
			if(!error)
			{
				//pragma omp single
				#pragma omp for
				for(u32 i = 0; i < N; ++i) X[0][i] = 1.0/N;

				u32 num_t = omp_get_num_threads();	
				u32 t_num = omp_get_thread_num();
				u32 t_index = N*t_num;
				/*	
				//pragma omp single
				#pragma omp for
				for(u32 i = 0; i < row_ptr[N]; ++i) ++t_dangling[t_index + col_idx[i]];

				//pragma omp single
				#pragma omp for
				for(u32 i = 0; i < N; ++i) for(u32 j = 0; j < num_t; ++j) dangling[i] += t_dangling[N*t_num+i];
				*/
				#pragma omp single
				{
					for(u32 i = 0; i < row_ptr[N]; ++i) ++dangling[col_idx[i]];
				}


				#pragma omp single
				{
					for(u32 i = 0; i < N; ++i)
					{
						if(!dangling[i])
						{
							dangling[danglingSize] = i;
							++danglingSize;
						}
					}
					W = (double)danglingSize/(N*N);
				}

				while(!done)
				{
					//schedule(guided)
					#pragma omp for 
					//pragma omp single
					for(u32 i = 0; i < N; ++i)
					{
						X[cur][i] = 0;
						for(u32 j = 0; j < row_ptr[i+1]-row_ptr[i]; ++j) X[cur][i] += val[row_ptr[i] + j] * X[last][col_idx[row_ptr[i] + j]];
						X[cur][i] = d*(X[cur][i] + W) + C;
					}
					
					#pragma omp single
					{
						W = 0;
					}
					
					#pragma omp for reduction(+:W)
					//pragma omp single
					for(u32 i = 0; i < danglingSize; ++i) W += X[cur][dangling[i]];

					#pragma omp for reduction(max:stopCrit)
					//pragma omp single
					for(u32 i = 0; i < N; ++i) if(fabs(X[cur][i] - X[last][i]) > stopCrit) stopCrit = fabs(X[cur][i] - X[last][i]);
					
					//printf("\t%d %f\n", t_num, stopCrit);
					#pragma omp single
					{
						//printf("%f\n", stopCrit);
						if(stopCrit < epsilon) done = 1;
						else
						{
							//for(int i = 0; i < N; ++i) printf("%f ", X[last][i]);
							//printf("\n");
							last ^= cur;
							cur ^= last;
							last ^= cur;

							W = W/N;
						}

						stopCrit = 0;
					}
				}
				
				#pragma omp for
				for(u32 i = 0; i < N; ++i) scores[i] = X[cur][i];
			}

		}


		//Success return
		free(X[0]);
		free(X[1]);
		free(dangling);
		free(t_dangling);
		return;

	} while(0);

	//Failure return
	free(X[0]);
	free(X[1]);
	free(dangling);
	free(t_dangling);
	return;
}


