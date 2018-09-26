/*
  ******************************************************************************
  * @file		split.c
  * @author		Molodcov Vladislav
  * @institute		MIPT
  * @departament	DREC
  * @group		717
  * @version		V1.1
  * @date		19-September-2018
  * @brief		SPLIT-functions
  ******************************************************************************
*/

/* Includes ----------------------------------------------------------------------*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<assert.h>

/* Defgroup for sizes ------------------------------------------------------------*/

#define MAX_SIZE_TEXT		10000
#define MAX_SIZE_DELIMITERS	10
#define MAX_NUM_WORDS		100
#define MAX_SIZE_WORD		100

/* Structure for SPLIT -----------------------------------------------------------*/

typedef struct
{
	int max_size_text;
	int max_size_delimiters;
	int max_num_words;
	int max_size_word;
	int count;

	char * text;
	char * delimiters;
	char ** words;
} SPLIT_STRUCT;

/* Declarations of functions for memory ------------------------------------------*/

char ** Allocate_memory_array(int array_size, int word_size);
char * Allocate_memory(int num_symbols);
void Free_memory_array(char ** array, int array_size);

/* Declarations of SPLIT-functions -----------------------------------------------*/

int SPLIT_Init(SPLIT_STRUCT * split);
int SPLIT_Scan(SPLIT_STRUCT * split);
int SPLIT_Split(SPLIT_STRUCT * split);
int SPLIT_Print(SPLIT_STRUCT * split);
int SPLIT_Free(SPLIT_STRUCT * split);

/* Main function -----------------------------------------------------------------*/

/* FIXIT: замечание в основном к стилю именования переменных и ф-й: вы собрали всевозможные стили вместе.
   Лучше что-то такое:
   SplitInfo info;
   info.maxSizeText = MAX_SIZE_TEXT;
   ...
   
   Еще assert работает только в режиме debug. Т.е. если вы соберете программу с ключом -O2 (режим release), и потом
   вы не сможете на каком-то этапе, например, память выделить, то потом получится seg fault, при попытке обращения к этой памяти.
   Плюс компилятор в режиме release может быть умнее программиста и просто не вычислять выражения внутри assert'ов :)
*/


int main()
{
	SPLIT_STRUCT split;
	split.max_size_text = 		MAX_SIZE_TEXT;
	split.max_size_delimiters = 	MAX_SIZE_DELIMITERS;
	split.max_num_words = 		MAX_NUM_WORDS;
	split.max_size_word = 		MAX_SIZE_WORD;

	assert(!SPLIT_Init(&split));
	assert(!SPLIT_Scan(&split));
	assert(!SPLIT_Split(&split));
	assert(!SPLIT_Print(&split));
	assert(!SPLIT_Free(&split));

	return 0;
}

/* Prototypes of functions for memory---------------------------------------------*/

char * Allocate_memory(int num_symbols)
{
	char * ret = (char *)malloc((num_symbols + 1) * sizeof(char));

	return ret;
}

char ** Allocate_memory_array(int array_size, int word_size)
{
	char ** ret = (char **)malloc(array_size * sizeof(char *));

	int i;

	for(i = 0; i < array_size; i++)
		ret[i] = Allocate_memory(word_size);

	return ret;
}

void Free_memory_array(char ** array, int array_size)
{
	int i;

	for(i = 0; i < array_size; i++)
		free(array[i]);

	free(array);
}

/* Prototypes of SPLIT-functions -------------------------------------------------*/

int SPLIT_Init(SPLIT_STRUCT * split)
{
	split->text =		Allocate_memory(split->max_size_text);
	split->delimiters =	Allocate_memory(split->max_size_delimiters);
	split->words =		Allocate_memory_array(split->max_num_words, split->max_size_word);
	split->count =		0;

	if(!split->text || !split->delimiters || !split->words)
		return 1;

	int i;

	for(i = 0; i < split->max_num_words; i++)
	{
		if(!split->words[i])
			return 1;
	}

	return 0;
}

int SPLIT_Scan(SPLIT_STRUCT * split)
{
	printf("Print text:\n");
	fgets(split->text, split->max_size_text, stdin);

	printf("Print delimiters:\n");
	fgets(split->delimiters, split->max_size_delimiters, stdin);

	return 0;
}

int SPLIT_Split(SPLIT_STRUCT * split)
{
	char * fragment = Allocate_memory(MAX_SIZE_WORD);

	int i = 0;

	fragment = strtok(split->text, split->delimiters);
	while(fragment)
	{
		if(!split->words[i])
			return 1;

		strcpy(split->words[i], fragment);
		fragment = strtok(NULL, split->delimiters);
		i++;
	}

	split->count = i;

	free(fragment);

	return 0;
}

int SPLIT_Print(SPLIT_STRUCT * split)
{
	if(!split->words)
		return 1;

	int i;
	int n = split->count;
	for(i = 0; i < n; i++)
	{
		if(!split->words[i])
			return 1;

		printf("%s\n", split->words[i]);
	}

	return 0;
}

int SPLIT_Free(SPLIT_STRUCT * split)
{
	free(split->text);
	free(split->delimiters);
	Free_memory_array(split->words, split->max_num_words);

	return 0;
}
