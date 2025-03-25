#include "function_declarations.h"


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

		u8 newlines = 0;
		while(newlines < 2) 
		{
			if(fileBuffer[cur] == '\n') ++newlines;
			++cur;
		}

		while((u8)(fileBuffer[cur] - '0') > 9) ++cur;
		*N = read_u32(fileBuffer, &cur);
		while((u8)(fileBuffer[cur] - '0') > 9) ++cur;
		u32 num_links = read_u32(fileBuffer, &cur);
		while((u8)(fileBuffer[cur] - '0') > 9) ++cur;
		
		BIF(links = (u32*)malloc(num_links*2*sizeof(u32)), "allocating links");
		BIF(outdegrees = (u32*)calloc(*N, sizeof(u32)), "allocating outdegrees");

		u32 linksSize = 0;
		u32 src = 0;
		u32 dst = 0;
		for(u32 i = 0; i < num_links; ++i)
		{
			src = read_u32(fileBuffer, &cur);
			while((u8)(fileBuffer[cur] - '0') > 9 && cur < fileSize) ++cur;
			dst = read_u32(fileBuffer, &cur);
			while((u8)(fileBuffer[cur] - '0') > 9 && cur < fileSize) ++cur;
			
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
