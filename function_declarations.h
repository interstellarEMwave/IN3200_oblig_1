#ifndef function_declarations
#define function_declarations

#include <stdlib.h>
#include <stdio.h>

typedef unsigned char u8;
typedef unsigned int u32;
typedef unsigned long long u64;



#define BIF(proposition, troublemaker) if(!(proposition)) { fprintf(stderr, "Problem with %s\n", troublemaker); break; }


void read_graph_from_file1(char* filename, int* N, double*** hyperlink_matrix);
void read_graph_from_file2(char* filename, int* N, int** row_ptr, int** col_idx, double** val);
void PageRank_iterations1(int N, double** hyperlink_matrix, double d, double epsilon, double* scores);
void PageRank_iterations2(int N, int* row_ptr, int* col_idx, double* val, double d, double epsilon, double* scores);
void top_n_webpages(int N, double* scores, int n);
u32 read_u32(u8* buffer, u64* cur);
void u32_swap(u32* a, u32* b);
void double_swap(double* a, double* b);
u32* quickSelect(double* arr, u32 beg, u32 end, int n, u32* ids);

#endif
