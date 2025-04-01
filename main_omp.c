#include "function_declarations.h"
#include <omp.h>


int
main(int argc, char** argv)
{
	//omp_set_num_threads(3);



	if(argc != 5) 
	{
		printf("YOYOYO,\n filename d epsilon n \nas arguments please!\n");
		return 0;
	}
	double d = atof(argv[2]);
	double e = atof(argv[3]);
	int n = atoi(argv[4]);

	printf("Parsing %s\n", argv[1]);
	int N = 0;
	int* row_ptr;
	int* col_idx;
	double* val;
	double* scores2 = NULL;
	read_graph_from_file2(argv[1], &N, &row_ptr, &col_idx, &val);
	
	if(!(scores2 = malloc(N*sizeof(double)))) 
	{
		free(scores2);
		printf("FUCK\n");
		return 0;
	}
	printf("PageRanking...\n");
	PageRank_iterations2(N, row_ptr, col_idx, val, d, e, scores2);
	printf("Top %d webpages in %s:\n", n, argv[1]);
	top_n_webpages(N, scores2, n);
		
	return 0;
}

