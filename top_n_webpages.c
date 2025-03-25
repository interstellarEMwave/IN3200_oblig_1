#include "function_declarations.h"

void 
top_n_webpages(int N, double* scores, int n)
{
	u32* ids = NULL;
	do
	{
		BIF(ids = (u32*)malloc(N*sizeof(u32)), "allocating ids");
		for(u32 i = 0; i < N; ++i) ids[i] = i;

		quickSelect(scores, 0, N, n, ids);
	
		printf("id:\tscore\n");
		for(u32 i = 0; i < n; ++i) printf("%d:\t%f\n", ids[i], scores[ids[i]]);

		//Success return
		free(ids);
		return;

	} while(0);

	//Failure return
	free(ids);
	return;
}
