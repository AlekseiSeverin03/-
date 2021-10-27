#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "leslib.h"


//--------------------------------------------------------------------------------------------------------
//! Compare two strings
//!
//! @param [in] str1      1st string
//! @param [in] str2      2nd string
//! @param [in] len_str1  length 1st string
//! @param [in] len_str2  length 2nd string
//! @param [in] flag      a flag showing where the compare with lines will be 
//!                       started (from the beginning or from the end)
//!
//! @return return _1_  if 1st string better 2nd string; 
//!                _0_  if strings are equal;
//!               _-1_  if 1st string less 2nd string.
//!
//! @note  "flag" parameter allows you to choose where to start comparing strings.
//--------------------------------------------------------------------------------------------------------

int StrCompare (const char str1[], const char str2[], size_t len_str1, size_t len_str2, int flag)
{
	assert (str1 != NULL);
	assert (str2 != NULL);
	assert (len_str1 >= 0);
	assert (len_str2 >= 0);

	int i = 0, j = 0;

	if (flag != 1)
	{
		i = len_str1 - 1;
		j = len_str2 - 1;

		len_str1 = 0;
		len_str2 = 0;
	}
	
	while (1)
	{
		if (str1[i] != str2[j]) break;

		if (len_str1 == i || len_str2 == j) 
		{
			i += flag;
			j += flag;

			break;
		}

		i += flag;
		j += flag;
	}

	return (str1[i] - str2[j]);
}

//--------------------------------------------------------------------------------------------------------
//! Swap two char type values
//!
//! @param [in]  ptr1  pointer on 1st value
//! @param [in]  ptr2  pointer on 2nd value
//!
//! @return  void
//--------------------------------------------------------------------------------------------------------

void SwapChar (char *ptr1, char *ptr2)
{
	char tmp = *ptr1;
	*ptr1 = *ptr2;
	*ptr2 = tmp;

	return;
}

//--------------------------------------------------------------------------------------------------------
//! Swap two values any type
//!
//! @param [in]  ptr1  pointer on 1st value
//! @param [in]  ptr2  pointer on 2nd value
//! @param [in]  nbyte the number of bytes that the value occupies
//!
//! @return  void
//--------------------------------------------------------------------------------------------------------

void Swap (void *ptr1, void *ptr2, size_t nbyte)
{
	for (int i = 0; i < nbyte; i++)
		SwapChar ((char *) ptr1 + i, (char *) ptr2 + i);

	return;
}

//--------------------------------------------------------------------------------------------------------
//! Bubble sort
//!
//! @param [in]  Ptr_Strs  pointer on array of struct string
//! @param [in]  nstr      number of strings in array
//! @param [in]  flag      a flag showing where the sort with lines will be 
//!                        started (from the beginning or from the end)
//!
//! @return  void 
//!
//! @note  "flag" parameter allows you to choose where to start sotr strings.
//--------------------------------------------------------------------------------------------------------

void BubbleSort (struct string *Ptr_Strs, size_t nstr, int (*comp)(const char *, const char *, size_t, size_t, int), int flag)
{
	assert (Ptr_Strs != NULL);
	assert (nstr >= 0);

	for (int nPass = 0; nPass < nstr - 1; nPass++)
		for (int i = 0; i < nstr - 1; i++)
			if ((*comp)(Ptr_Strs[i].ptr_str, Ptr_Strs[i+1].ptr_str,
			            Ptr_Strs[i].len_str, Ptr_Strs[i+1].len_str, flag) > 0)
			{
				Swap (&Ptr_Strs[i], &Ptr_Strs[i+1], sizeof (Ptr_Strs[i]));
			}
	return;
}

//--------------------------------------------------------------------------------------------------------
//! Finds out the number of symbols in the file
//!
//! @param  [in]  stream  stream file
//!
//! @return  number of symbols in the file
//--------------------------------------------------------------------------------------------------------

int SizeFile (FILE *stream)
{
	fseek (stream, 0, SEEK_END);
	int size_file = ftell (stream);
	fseek (stream, 0, SEEK_SET);
	
	return size_file;
}

//--------------------------------------------------------------------------------------------------------
//! Writes the contents of the file to the buffer
//!
//! @param [in]  name_file       name file
//! @parem [out] nstr            number of lines written
//! @param [out] ptr_ptr_buffer  pointer on pointer on buffer
//!
//! @return  pointer on struct string
//--------------------------------------------------------------------------------------------------------

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
		
	int size_buf = SizeFile (stream) + 2;

	*ptr_ptr_buffer = (char *) calloc (size_buf, sizeof (char));
	
	if (*ptr_ptr_buffer == NULL)
	{
		printf ("FAILED TO ALLOCATE MEMORY FOR THE BUFFER\n");
		exit (1);
	}

	CopyFileInBuffer (*ptr_ptr_buffer, size_buf, stream);
	fclose (stream);

	int size_array = NumberStrInBuffer (*ptr_ptr_buffer); 

	struct string *Ptr_Strs;
	Ptr_Strs = (struct string *) calloc (size_array, sizeof (struct string));

	if (Ptr_Strs == NULL)
	{
		printf ("FAILED TO ALLOCATE MEMORY FOR THE ARRAY STRUCTS FOR STRING\n");
		exit (1);
	}

	CopyPtrStrInArray (Ptr_Strs, *ptr_ptr_buffer + 1, size_buf - 1); 

	*nstr = size_array;	

	return Ptr_Strs;
}

//--------------------------------------------------------------------------------------------------------
//! Copies the contents of the file to the buffer
//!
//! @param [in]  buffer     pointer on buffer
//! @param [in]  size_file  number symbols in file
//! @param [in]  stream     the stream from which the buffer will be written
//!
//! @return void
//--------------------------------------------------------------------------------------------------------

void CopyFileInBuffer (char buffer[], size_t size_file,  FILE *stream)
{
	assert (buffer != NULL);
	assert (stream != NULL);		

	buffer[0] = '\0';
	fread (buffer + 1, sizeof (char), size_file, stream);
	
	return;
}

int NumberStrInBuffer (char *str)
{
	assert (str != NULL);

	int count = 0;
	char symbol = 0;
	int i = 0;

	while ((symbol = *(str + i + 1)) != '\0')
	{
		if (symbol == '\n')
		{
			*(str + i + 1) = '\0';
			
			if (*(str + i) != '\0')
				count++;
		}

		i++;
	}

	

	return (count + 1);
}

//--------------------------------------------------------------------------------------------------------
//! Copy pointers not empty lines and length's lines in array structs
//!
//! @param [in]  Index        struct: array string and them length
//! @param [in]  str          pointer on begin buffer
//! @param [in]  size_buffer  size buffer
//!
//! @return void
//--------------------------------------------------------------------------------------------------------

void CopyPtrStrInArray (struct string *Index, const char *str, size_t size_buffer)
{
	assert (str != NULL);
	assert (size_buffer >= 0);

	int number_str = 0;
	const char *ptr = str;  
	int length = 0;

	for (int i = 0; i < size_buffer; i++)
	{
		char symbol = 0;
		
		if ((symbol = *(str + i)) == '\0')
			if (*(str + i - 1) == '\0') 
			{
				ptr = str + i + 1;   
			}
			else
			{
				Index[number_str].ptr_str = (char *) ptr;
				Index[number_str].len_str = length;  

				ptr = str + i + 1;
				number_str++;
				length = 0;
			}
		else
		{
			length++;
		}
	}	
	
	return;
}

//--------------------------------------------------------------------------------------------------------
//! Print not empty lines
//!
//! @param [in] Index      struct: array strings and them length
//! @param [in] nstr       number not empty string in buffer
//! @param [in] name_file  filename where the sort file will be write
//!
//! @return  void
//--------------------------------------------------------------------------------------------------------

void PrintStrs (const struct string *Index, size_t nstr, const char *name_file)
{
	assert (Index != NULL);
	assert (nstr >= 0);

	FILE *stream = fopen (name_file, "w");
	if (stream == NULL)
	{
		printf ("Problem with opening file %s\n", name_file);
		exit (2);
	}

	for (int i = 0; i < nstr; i++)
		{
			fprintf (stream, "%s\n", (Index + i)->ptr_str);
		}

	fclose (stream);

	return;
}

//--------------------------------------------------------------------------------------------------------
//! Print content source file
//!
//! @param [in] str        pointer on buffer where write content source file
//! @param [in] size_file  size buffer
//! @param [in] name_file  filename where the original file will be overwritten
//!
//! @return  void
//!
//! @note  function exit program if where were problem with opening file
//--------------------------------------------------------------------------------------------------------

void PrintSource (const char *str, size_t size_file, const char *name_file)
{
	assert (str != NULL);

	FILE *stream = fopen (name_file, "w");
	if (stream == NULL)
	{
		printf ("Problem with opening file %s\n", name_file);
		exit (2);
	}

	char symbol = 0; 
	int i = 0;
	int len_buf = 0;

	while (str[len_buf] == '\0')
	{
		fprintf (stream, "\n");
		len_buf++;
	}

	while (i < size_file)
	{
		while ((symbol = str[len_buf]) != '\0')
		{
			fprintf (stream, "%c", symbol);

			len_buf++;
		}

		while (str[len_buf] == '\0')
		{
			fprintf (stream, "\n");
				
			len_buf++;
		}
		
		i++;
	}

	return;
}
