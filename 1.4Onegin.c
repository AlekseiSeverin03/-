#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

struct string
{
	char *ptr_str;
	int len_str;
};

void            BubbleSortBegin    (struct string *, size_t);
void            BubbleSortEnd      (struct string *, size_t);
void            SwapPointer        (const char **, const char **);
int             StrCompare0        (const char *,  const char *);
int             StrCompare         (const char *,  const char *, size_t, size_t);
struct string*  ReadFile           (const char *, size_t *, char **);
void            CopyFileInBuffer   (char *, int ,  FILE *);
int             NumberStrInBuffer  (char *);
void            CopyPtrStrInArray  (struct string *, char *);
void            PrintStrs          (struct string *, size_t);
void            Swap               (int *, int *);
void            SwapStructString   (struct string *, struct string *);
void            RemoveEmptyLines   (char **);



int main()
{
	char name_file[20];
	size_t nstr = 0;
	char *ptr_buffer;

	printf ("\nEnter NAME file: ");
	scanf ("%s", name_file);

	struct string *Index = ReadFile (name_file, &nstr, &ptr_buffer);

	printf ("Your file:\n");
	PrintStrs (Index, nstr);

	BubbleSortBegin (Index, nstr);

	printf ("SortBegin file:\n");
	PrintStrs (Index, nstr);	

	BubbleSortEnd (Index, nstr);

	printf ("SortEnd file:\n");
//	PrintStrs (Index, nstr);	

	free (ptr_buffer);


	return 0;	
}



int StrCompare0 (const char str1[], const char str2[])
{
	assert (str1 != NULL);
	assert (str2 != NULL);

	int i = 0;
	
	while (1)
	{
		if (str1[i] != str2[i]) break;
		if (str1[i] == '\0')    break;
		i++;
	}

	return (str1[i] - str2[i]);
}

int StrCompareBegin (const char str1[], const char str2[], size_t len_str1, size_t len_str2)
{
	assert (str1 != NULL);
	assert (str2 != NULL);

	assert (len_str1 >= 0);
	assert (len_str2 >= 0);


	int i = 0;
	
	while (1)
	{
		if (str1[i] != str2[i]) break;
		if (len_str1 == i && len_str2 == i) return 0;
		if (len_str1 == i) return -1;
		if (len_str2 == i) return 1;
		i++;
	}

	return (str1[i] - str2[i]);
}

int StrCompareEnd (const char str1[], const char str2[], size_t len_str1, size_t len_str2)
{
	assert (str1 != NULL);
	assert (str2 != NULL);

	assert (len_str1 >= 0);
	assert (len_str2 >= 0);


	int i = len_str1 - 1;
	int j = len_str2 - 1;
	
	while (1)
	{
		if (str1[i] != str2[j]) break;
		if (i == 0 && j == 0) return 0;
		if (i == 0) return -1;
		if (j == 0) return 1;
		i--;
		j--;
	}

	return (str1[i] - str2[j]);
}


void SwapStructString (struct string *one, struct string *two)
{
	assert (one != NULL);
	assert (two != NULL);


	struct string tmp = *one;
	*one = *two;
	*two = tmp;

	return;
}

void SwapPointer (const char **str1, const char **str2)
{
	assert (str1 != NULL);
	assert (str2 != NULL);
	assert (*str1 != NULL);
	assert (*str2 != NULL);


	const char *temp = *str1;
	*str1 = *str2;
	*str2 = temp;
}

void Swap (int *x, int *y)
{
	int tmp = *x;
	*x = *y;
	*y = tmp;
	
	return;
}

void BubbleSortBegin (struct string *Ptr_Strs, size_t N)
{
	assert (Ptr_Strs != NULL);
	assert (N >= 0);

	for (int nPass = 0; nPass < N - 1; nPass++)
		for (int i = 0; i < N - 1; i++)
			if (StrCompareBegin (Ptr_Strs[i].ptr_str, Ptr_Strs[i+1].ptr_str,
			    Ptr_Strs[i].len_str, Ptr_Strs[i+1].len_str) > 0)
			{
				SwapStructString (&Ptr_Strs[i], &Ptr_Strs[i+1]);
			}
	return;
}

void BubbleSortEnd (struct string *Ptr_Strs, size_t N)
{
	assert (Ptr_Strs != NULL);
	assert (N >= 0);

	for (int nPass = 0; nPass < N - 1; nPass++)
		for (int i = 0; i < N - 1; i++)
			if (StrCompareEnd (Ptr_Strs[i].ptr_str, Ptr_Strs[i+1].ptr_str,
			    Ptr_Strs[i].len_str, Ptr_Strs[i+1].len_str) > 0)
			{
				SwapStructString (&Ptr_Strs[i], &Ptr_Strs[i+1]);
			}
	return;
}


struct string* ReadFile (const char *name_file, size_t *nstr, char **ptr_ptr_buffer)
{
	assert (name_file != NULL);
	assert (nstr != NULL);
	assert (ptr_ptr_buffer != NULL);


	FILE *stream = fopen (name_file, "r");
	if (stream == NULL) 
	{
		printf ("\nFAILED TO OPEN THE FILE \"%s\"\n", name_file);
		exit (1);
	}

	fseek (stream, 0, SEEK_END);
		
	int size_file = ftell (stream);

	fseek (stream, 0, SEEK_SET);	

	*ptr_ptr_buffer = calloc (size_file, sizeof (char));
	
	if (*ptr_ptr_buffer == NULL)
	{
		printf ("FAILED TO ALLOCATE MEMORY FOR THE BUFFER\n");
		exit (1);
	}

	CopyFileInBuffer (*ptr_ptr_buffer, size_file, stream);
	fclose (stream);


	RemoveEmptyLines (ptr_ptr_buffer);


	int size_array = NumberStrInBuffer (*ptr_ptr_buffer);

	struct string *Ptr_Strs;
	Ptr_Strs = calloc (size_array, sizeof (struct string));

	if (Ptr_Strs == NULL)
	{
		printf ("FAILED TO ALLOCATE MEMORY FOR THE ARRAY STRUCTS FOR STRING\n");
		exit (1);
	}

	CopyPtrStrInArray (Ptr_Strs, *ptr_ptr_buffer); 

	*nstr = size_array;	

	return Ptr_Strs;
}

void CopyFileInBuffer (char buffer[], int size_file,  FILE *stream)
{
	assert (buffer != NULL);
	assert (stream != NULL);		


	int i = 0;
	for (i = 0; i < size_file; i++)
		buffer[i] = fgetc (stream);

	return;
}

int NumberStrInBuffer (char *str)
{
	assert (str != NULL);


	int count = 0;
	char symbol = 0;
	int i = 0;

	while ((symbol = *(str + i)) != '\0')
	{
		if (symbol == '\n')	count++;
		i++;
	}

	return (count + 1);
}

void CopyPtrStrInArray (struct string *index, char *str)
{
	assert (str != NULL);


	int number_str = 0;
	char *ptr = str;
	int length = 0;

	for (int i = 0; i < strlen (str) + 1; i++)
	{
		char symbol = 0;
		
		if ((symbol = *(str + i)) == '\n' || symbol == '\0')
		{
			index[number_str].ptr_str = ptr;
			ptr = str + i + 1;
			index[number_str].len_str = length;
			length = 0;
			number_str++;
		}
		else
		{
			length++;
		}
	}	
	
	return;
}

void PrintStrs (struct string *Index, size_t nstr)
{
	assert (Index != NULL);
	assert (nstr >= 0);


	for (int i = 0; i < nstr; i++)
		{
			for (int nchar = 0; nchar < (Index + i)->len_str; nchar++)
				printf ("%c", ((Index + i)->ptr_str)[nchar]);
			printf ("\n");
		}
	printf ("\n");

	return;
}

void RemoveEmptyLines (char **str)
{
	assert (*str != NULL);


	int pos_rd = 0;

	while ((*str)[pos_rd] == '\n' || (*str)[pos_rd] == '\v')  
	{
		pos_rd++;
	}

	int pos_wt = 0;
	
	(*str)[pos_wt] = (*str)[pos_rd];    // if in start of file were empty lines - remove them
	
	for (; (*str)[pos_rd] != '\0';)
	{
		if ((*str)[pos_rd + 1] == '\v')
		{
			(*str)[pos_rd + 1] = '\n';
		}

		if ((*str)[pos_rd] == '\n' && (*str)[pos_rd + 1] == '\n') {}
		else
		{
			pos_wt++;	
		}

		pos_rd++;
		(*str)[pos_wt] = (*str)[pos_rd];
	}
	
	if ((*str)[pos_wt -1] == '\n')
	{
		(*str)[pos_wt - 1] = '\0';     // if in end of file was enmpty line - remove its
	}

	*str = realloc (*str, pos_wt + 1);

	return;
}
