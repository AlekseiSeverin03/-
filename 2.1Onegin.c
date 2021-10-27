#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "leslib.h"

const int MAX_LEN_NAME_FILE = 1000;

enum FLAG
{
	BEGIN = 1,
	END  = -1,
};

int main()
{
	char name_file[MAX_LEN_NAME_FILE];

	printf ("\nEnter NAME file: ");
	scanf ("%s", name_file);

	size_t nstr = 0;
	char *ptr_buffer;

	struct string *Index = ReadFile (name_file, &nstr, &ptr_buffer);

	BubbleSort (Index, nstr, StrCompare, BEGIN);
	PrintStrs (Index, nstr, "SortBegin.txt");	

	BubbleSort (Index, nstr, StrCompare,  END);
	PrintStrs (Index, nstr, "SortEnd.txt");

	PrintSource (ptr_buffer + 1, nstr, "SourceFile.txt");

	free (ptr_buffer);


	return 0;	
}

