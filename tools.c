#include "function_declarations.h"


u32
read_u32(u8* buffer, u64* cur)
{
	u32 out = 0;
	while((u8)(buffer[*cur] - (u8)'0') <= (u8)9)
	{
		out = 10*out + buffer[*cur] - '0';
		++*cur;
	}

	return out;
}


void u32_swap(u32* a, u32* b) { u32 t = *a; *a = *b; *b = t; }
void double_swap(double* a, double* b) { double t = *a; *a = *b; *b = t; }



u32*
quickSelect(double* arr, u32 beg, u32 end, int n, u32* ids)
{
	if(end < beg + 2) return ids;

	double piv = arr[ids[beg]];
	u32 l = beg + 1;
	u32 r = end - 1;
	while(l < r)
	{
		if(arr[ids[l]] > piv) ++l;
		else
		{
			u32_swap(ids+l, ids+r);
			--r;
		}
	}
	if(!(arr[ids[l]] > piv)) --l;
	
	u32_swap(ids+beg, ids+l);
	
	quickSelect(arr, beg, l, n, ids);
	if(l >= n) return ids;
	quickSelect(arr, l+1, end, n, ids);

	return ids;
}

