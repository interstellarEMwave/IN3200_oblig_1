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


int
main(int argc, char** argv)
{
	int N = 0;
	double** hyperlink_matrix;
	read_graph_from_file1(argv[1], &N, &hyperlink_matrix); 
	
	//int N = 0;
	int* row_ptr;
	int* col_idx;
	double* val;
	read_graph_from_file2(argv[1], &N, &row_ptr, &col_idx, &val);
	return 0;
}


u32
read_u32(u8* buffer, u64* cur)
{
	u32 out = 0;
	while((u8)(buffer[*cur] - '0') <= 9)
	{
		out = 10*out + buffer[*cur] - '0';
		++*cur;
	}

	return out;
}


void 
read_graph_from_file1(char* filename, int* N, double*** hyperlink_matrix)
{
	FILE* file = NULL;
	u8* fileBuffer = NULL;
	u32* links = NULL;
	u32* outdegrees = NULL;
	*hyperlink_matrix = NULL;
	double* tempMatrix = NULL;

	do
	{
		BIF(file = fopen(filename, "rb"), "opening file");
		BIF(!fseek(file, 0, SEEK_END), "seeking to end of file");
		u64 fileSize = ftell(file);
		rewind(file);

		BIF(fileBuffer = (u8*)malloc(fileSize), "allocating filebuffer");
		BIF(fread(fileBuffer, 1, fileSize, file) == fileSize, "reading file");
		fclose(file);

		u64 cur = 0;

		while((u8)(fileBuffer[cur] - '0') > 9) ++cur;
		*N = read_u32(fileBuffer, &cur);
		while((u8)(fileBuffer[cur] - '0') > 9) ++cur;
		u32 num_links = read_u32(fileBuffer, &cur);
		while((u8)(fileBuffer[cur] - '0') > 9) ++cur;
		
		BIF(links = (u32*)malloc(num_links*2*sizeof(u32)), "allocating links");
		BIF(outdegrees = (u32*)calloc(*N, sizeof(u32)), "allocating outdegrees");
		
		u32 linksSize = 0;
		for(u32 i = 0; i < num_links; ++i)
		{
			u32 src = read_u32(fileBuffer, &cur);
			while((u8)(fileBuffer[cur] - '0') > 9) ++cur;
			u32 dst = read_u32(fileBuffer, &cur);
			while((u8)(fileBuffer[cur] - '0') > 9) ++cur;

			if(src != dst)
			{
				links[linksSize] = src;
				links[linksSize+1] = dst;
				linksSize += 2;	

				++outdegrees[src];
			}
		}
		
		BIF(tempMatrix = (double*)calloc((*N) * (*N), sizeof(double)), "allocating tempMatrix");
		BIF(*hyperlink_matrix = (double**)malloc(*N * sizeof(double*)), "allocating hyperlink matrix");	
		for(int i = 0; i < *N; ++i) 
		{
			(*hyperlink_matrix)[i] = tempMatrix + i * (*N);
		}
		
		for(u32 i = 0; i < linksSize; i += 2) (*hyperlink_matrix)[links[i+1]][links[i]] = outdegrees[links[i]] ? 1.0/outdegrees[links[i]] : 0;


		//Success return
		free(fileBuffer);
		free(links);
		free(outdegrees);
		return;
	}while(0);

	//Failure return
	if(file) fclose(file);
	free(fileBuffer);
	free(links);
	free(outdegrees);
	free(hyperlink_matrix);
	return;
}


void 
read_graph_from_file2(char* filename, int* N, int** row_ptr, int** col_idx, double** val)
{
	FILE* file = NULL;
	*row_ptr = NULL;
	*col_idx = NULL;
	*val = NULL;
	u32* indexes = NULL;
	u8* fileBuffer = NULL;
	u32* links = NULL;
	int* outdegrees = NULL;

	do
	{
		BIF(file = fopen(filename, "rb"), "opening file");
		BIF(!fseek(file, 0, SEEK_END), "seeking to end of file");
		u64 fileSize = ftell(file);
		rewind(file);

		BIF(fileBuffer = (u8*)malloc(fileSize), "allocating filebuffer");
		BIF(fread(fileBuffer, 1, fileSize, file) == fileSize, "reading file");
		fclose(file);
		
		u64 cur = 0;

		while((u8)(fileBuffer[cur] - '0') > 9) ++cur;
		*N = read_u32(fileBuffer, &cur);
		while((u8)(fileBuffer[cur] - '0') > 9) ++cur;
		u32 num_links = read_u32(fileBuffer, &cur);
		while((u8)(fileBuffer[cur] - '0') > 9) ++cur;
		
		BIF(links = (u32*)malloc(num_links*2*sizeof(u32)), "allocating links");
		BIF(*row_ptr = (int*)calloc(*N+1, sizeof(int)), "allocating (*row_ptr)");
		BIF(outdegrees = (int*)calloc(*N, sizeof(int)), "allocating outdegrees");
		
		u32 linksSize = 0;
		for(u32 i = 0; i < num_links; ++i)
		{
			u32 src = read_u32(fileBuffer, &cur);
			while((u8)(fileBuffer[cur] - '0') > 9) ++cur;
			u32 dst = read_u32(fileBuffer, &cur);
			while((u8)(fileBuffer[cur] - '0') > 9) ++cur;

			if(src != dst)
			{
				links[linksSize] = src;
				links[linksSize+1] = dst;
				linksSize += 2;	
				
				++outdegrees[src];
				++(*row_ptr)[dst+1];
			}
		}
		

		for(int i = 1; i < *N + 1; ++i) (*row_ptr)[i] += (*row_ptr)[i-1];


		BIF(*col_idx = (int*)malloc(linksSize/2 * sizeof(int)), "allocating *col_idx");
		BIF(*val = (double*)malloc(linksSize/2 * sizeof(double)), "allocating *val");

		BIF(indexes = (u32*)calloc(*N, sizeof(u32)), "allocating indexes");
		for(u32 i = 0; i < linksSize; i += 2)
		{
			u32 t_src = links[i];
			u32 t_dst = links[i+1];

			(*col_idx)[(*row_ptr)[t_dst] + indexes[t_dst]] = t_src;
			(*val)[(*row_ptr)[t_dst] + indexes[t_dst]] = (outdegrees[t_src]) ? 1.0/outdegrees[t_src] : 0;
			indexes[t_dst]++;
		}
	
		//Success return
		free(indexes);
		free(fileBuffer);
		free(links);
		free(outdegrees);
		return;
	} while(0);

	//Failure return
	if(file) fclose(file);
	free(*row_ptr);
	free(*col_idx);
	free(*val);
	free(indexes);
	free(fileBuffer);
	free(links);
	free(outdegrees);
	return;
}



void 
PageRank_iterations1(int N, double** hyperlink_matrix, double d, double epsilon, double* scores)
{
	double** X = NULL;
	
	do
	{
		BID(X = (double*)malloc(N*sizeof(double)), "allocate X");
		for(int i = 0; i < N; ++i) X[i] = 1.0/N;


		while(1)
		{

		}
		return;
	} while(0);

	//Fa
	
	return;
}
