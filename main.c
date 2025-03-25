#include "function_declarations.h"

int
main(int argc, char** argv)
{
	if(argc != 6) 
	{
		printf("YOYOYO,\n file1 file2 d epsilon n \nas arguments please!\n");
		return 0;
	}
	double d = atof(argv[3]);
	double e = atof(argv[4]);
	int n = atoi(argv[5]);

	printf("Parsing %s\n", argv[1]);
	int N = 0;
	double** hyperlink_matrix;
	double* scores1 = NULL;
	read_graph_from_file1(argv[1], &N, &hyperlink_matrix);
	if(!(scores1 = malloc(N*sizeof(double)))) 
	{
		free(scores1);
		printf("FUCK\n");
		return 0;
	}
	printf("PageRanking...\n");
	PageRank_iterations1(N, hyperlink_matrix, d, e, scores1);
	printf("Top %d webpages in %s:\n", n, argv[1]);
	top_n_webpages(N, scores1, n);

	

	printf("\n\nParsing %s\n", argv[2]);
	N = 0;
	int* row_ptr;
	int* col_idx;
	double* val;
	double* scores2 = NULL;
	read_graph_from_file2(argv[2], &N, &row_ptr, &col_idx, &val);
	if(!(scores2 = malloc(N*sizeof(double)))) 
	{
		free(scores2);
		printf("FUCK\n");
		return 0;
	}
	printf("PageRanking...\n");
	PageRank_iterations2(N, row_ptr, col_idx, val, d, e, scores2);
	printf("Top %d webpages in %s:\n", n, argv[2]);
	top_n_webpages(N, scores2, n);
		

	return 0;
}

