#include <stdio.h>

struct string
{
	char *ptr_str;
	int len_str;
};

int             SizeFile           (FILE *);
void            CopyFileInBuffer   (char *,   size_t,  FILE *);
int             NumberStrInBuffer  (char *);
void            CopyPtrStrInArray  (struct string *,  const char *,  size_t);
struct string*  ReadFile           (const char *,  size_t *,  char **);
int             StrCompare         (const char *,  const char *,  size_t,  size_t,  const int);
void            Swap               (void *, void *, size_t);
void            SwapChar           (char *, char *);
void            BubbleSort         (struct string *,  size_t, int (*comp)(const char *, const char *, size_t, size_t, int),  int);
void            PrintStrs          (const struct string *,  size_t,  const char *);
void            PrintSource        (const char *,  size_t,  const char *);








