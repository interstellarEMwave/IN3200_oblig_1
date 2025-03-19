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
		
		printf("%d %d\n", *N, num_links);
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
		
		for(u32 i = 0; i < linksSize; i += 2) (*hyperlink_matrix)[links[i+1]][links[i]] = 1.0/outdegrees[links[i]];

		printf("matrix:\n");
		for(int i = 0; i < *N; ++i)
		{
			for(int j = 0; j < *N; ++j) printf("%f ", (*hyperlink_matrix)[i][j]);
			printf("\n");
		}


		//Success return
		free(fileBuffer);
		return;
	}while(0);

	//Failure return
	if(file) fclose(file);
	free(fileBuffer);
	return;
}
