/*
 * hw12-sorting-hashing.c
 *
 *  Homework 12: Sorting & Hashing
 *  Department of Computer Science at Chungbuk National University
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h> //랜덤값을 뽑기위한 헤더파일

#define MAX_ARRAY_SIZE			13	/* prime number */
#define MAX_HASH_TABLE_SIZE 	MAX_ARRAY_SIZE

/* 필요에 따라 함수 추가 가능 */
int initialize(int **a); //배열을 초기화하는 함수(랜덤값으로)
int freeArray(int *a); //배열에 할당된 메모리를 free 시키는 함수
void printArray(int *a); //배열을 출력하는 함수

int selectionSort(int *a); //배열의 값들을 오름차순으로 선택정렬하는 함수
int insertionSort(int *a); //배열의 값들을 오름차순으로 삽입정렬하는 함수
int bubbleSort(int *a); //배열의 값들을 오름차순으로 버블정렬하는 함수
int shellSort(int *a); //배열의 값들을 오름차순으로 셸 정렬하는 함수
/* recursive function으로 구현 */
int quickSort(int *a, int n); //배열의 값들을 오름차순으로 퀵 정렬하는 함수


/* hash code generator, key % MAX_HASH_TABLE_SIZE */
int hashCode(int key);

/* array a에대 한 hash table을 만든다. */
int hashing(int *a, int **ht);

/* hash table에서 key를 찾아 hash table의 index return */
int search(int *ht, int key);


int main()
{
	char command; //명령어 변수
	int *array = NULL; //정수 배열을 동적으로 할당받을 포인터
	int *hashtable = NULL;
	int key = -1; //key 값 -1로 초기화
	int index = -1; //index 값 -1로 초기화

	srand(time(NULL)); //시간을 seed로 줘서 매번 다른 값이 seed로 들어간다
	printf("[----- 김주훈 2020039027 -----]\n\n"); //이름 학번 출력
	do{
		printf("----------------------------------------------------------------\n");
		printf("                        Sorting & Hashing                       \n");
		printf("----------------------------------------------------------------\n");
		printf(" Initialize       = z           Quit             = q\n");
		printf(" Selection Sort   = s           Insertion Sort   = i\n");
		printf(" Bubble Sort      = b           Shell Sort       = l\n");
		printf(" Quick Sort       = k           Print Array      = p\n");
		printf(" Hashing          = h           Search(for Hash) = e\n");
		printf("----------------------------------------------------------------\n");

		printf("Command = ");
		scanf(" %c", &command);

		switch(command) {
		case 'z': case 'Z':
			initialize(&array);
			break;
		case 'q': case 'Q':
			freeArray(array);
			break;
		case 's': case 'S':
			selectionSort(array);
			break;
		case 'i': case 'I':
			insertionSort(array);
			break;
		case 'b': case 'B':
			bubbleSort(array);
			break;
		case 'l': case 'L':
			shellSort(array);
			break;
		case 'k': case 'K':
			printf("Quick Sort: \n");
			printf("----------------------------------------------------------------\n");
			printArray(array);
			quickSort(array, MAX_ARRAY_SIZE);
			printf("----------------------------------------------------------------\n");
			printArray(array);

			break;

		case 'h': case 'H':
			printf("Hashing: \n");
			printf("----------------------------------------------------------------\n");
			printArray(array);
			hashing(array, &hashtable);
			printArray(hashtable);
			break;

		case 'e': case 'E':
			printf("Your Key = ");
			scanf("%d", &key);
			printArray(hashtable);
			index = search(hashtable, key);
			printf("key = %d, index = %d,  hashtable[%d] = %d\n", key, index, index, hashtable[index]);
			break;

		case 'p': case 'P':
			printArray(array);
			break;
		default:
			printf("\n       >>>>>   Concentration!!   <<<<<     \n");
			break;
		}

	}while(command != 'q' && command != 'Q');

	return 1;
}

int initialize(int** a) //배열을 초기화 하는 함수(배열의 값들을 랜덤으로 초기화)
{
	int *temp = NULL;

	/* array가 NULL인 경우 메모리 할당 */
	if(*a == NULL) {
		temp = (int*)malloc(sizeof(int) * MAX_ARRAY_SIZE); //배열의 크기만큼 temp에 메모리 할당
		*a = temp;  /* 할당된 메모리의 주소를 복사 --> main에서 배열을 control 할수 있도록 함*/
	} else
		temp = *a;

	/* 랜덤값을 배열의 값으로 저장 */
	for(int i = 0; i < MAX_ARRAY_SIZE; i++)
		temp[i] = rand() % MAX_ARRAY_SIZE;

	return 0;
}

int freeArray(int *a) //배열을 free 시키는 함수
{
	if(a != NULL)
		free(a); //배열의 크기만큼 메모리를 할당받은 포인터를 free시킨다.
	return 0;
}

void printArray(int *a) //배열을 출력하는 함수
{
	if (a == NULL) { //배열이 생성되지 않았을 때
		printf("nothing to print.\n");
		return;
	}
	for(int i = 0; i < MAX_ARRAY_SIZE; i++) //배열의 처음부터 끝까지 출력
		printf("a[%02d] ", i);
	printf("\n");
	for(int i = 0; i < MAX_ARRAY_SIZE; i++)
		printf("%5d ", a[i]);
	printf("\n");
}


int selectionSort(int *a) //배열을 선택정렬하는 함수
{
	int min; //가장 작은 값을 저장할 변수
	int minindex; //가장 작은 값의 위치를 저장할 변수
	int i, j;

	printf("Selection Sort: \n");
	printf("----------------------------------------------------------------\n");

	printArray(a); //정렬되기 이전의 배열 출력

	for (i = 0; i < MAX_ARRAY_SIZE; i++)
	{
		minindex = i; //정렬되지 않은 첫 번째 값으로 초기화
		min = a[i]; //정렬되지 않은 첫 번째 위치로 초기화
		for(j = i+1; j < MAX_ARRAY_SIZE; j++) //배열의 크기만큼 반복
		{
			if (min > a[j]) //더 작은 값을 발견하면
			{
				min = a[j]; //min에 저장
				minindex = j; //위치 저장
			}
		}
		a[minindex] = a[i]; //min과 i의 값을 바꾼다
		a[i] = min;
	}

	printf("----------------------------------------------------------------\n");
	printArray(a); //정렬된 배열 출력
	return 0;
}

int insertionSort(int *a) //배열을 삽입정렬하는 함수
{
	int i, j, t;

	printf("Insertion Sort: \n");
	printf("----------------------------------------------------------------\n");

	printArray(a);

	for(i = 1; i < MAX_ARRAY_SIZE; i++)
	{
		t = a[i]; //정렬되지 않은 배열의 첫 번째 원소 저장
		j = i; //정렬되지 않은 배열의 첫 번째 원소 위치 저장
		while (a[j-1] > t && j > 0) //정렬된 원소가 더 크고, j가 0보다 크면
		{
			a[j] = a[j-1]; //뒤의 값을 앞의 값으로 바꾼다.
			j--; //첫 번째 원소가 될 때까지 반복
		}
		a[j] = t; //정렬된 배열의 맞는 위치에 t 저장
	}

	printf("----------------------------------------------------------------\n");
	printArray(a); //정렬된 후의 배열 출력

	return 0;
}

int bubbleSort(int *a) //배열을 버블정렬 하는 함수
{
	int i, j, t;

	printf("Bubble Sort: \n");
	printf("----------------------------------------------------------------\n");

	printArray(a);

	for(i = 0; i < MAX_ARRAY_SIZE; i++) //n^2 번 정도 반복
	{
		for (j = 0; j < MAX_ARRAY_SIZE; j++)
		{
			if (a[j-1] > a[j]) //앞의 값이 더 크면 바꾼다.
			{
				t = a[j-1];
				a[j-1] = a[j];
				a[j] = t;
			}
		}
	}

	printf("----------------------------------------------------------------\n");
	printArray(a);

	return 0;
}

int shellSort(int *a) //배열을 셸 정렬 하는 함수
{
	int i, j, k, h, v;

	printf("Shell Sort: \n");
	printf("----------------------------------------------------------------\n");

	printArray(a);

	for (h = MAX_ARRAY_SIZE/2; h > 0; h /= 2) //h를 1보다 작아질 때까지 반복
	{
		for (i = 0; i < h; i++)
		{
			for(j = i + h; j < MAX_ARRAY_SIZE; j += h)
			{
				v = a[j];
				k = j;
				while (k > h-1 && a[k-h] > v)
				{
					a[k] = a[k-h]; //h만큼 떨어져있는 값을 선택정렬한다.
					k -= h;
				}
				a[k] = v; 
			}
		}
	}
	printf("----------------------------------------------------------------\n");
	printArray(a);

	return 0;
}

int quickSort(int *a, int n) //배열을 퀵 정렬 하는 함수(재귀적)
{
	int v, t;
	int i, j;

	if (n > 1)
	{
		v = a[n-1];
		i = -1;
		j = n - 1;

		while(1)
		{
			while(a[++i] < v); //v보다 큰 값을 가진 곳에서 i가 멈춤
			while(a[--j] > v); //v보다 작은 값을 가진 곳에서 j가 멈춤

			if (i >= j) break; //i와 j가 교차(cross)하면 break
			t = a[i];
			a[i] = a[j];
			a[j] = t;
			//i에 위치한 값과 j의 위치한 값을 바꾼다.
		}
		t = a[i];
		a[i] = a[n-1];
		a[n-1] = t;
		//i에 위치한 값과 n-1에 위치한 값을 바꾼다.(피봇의 위치의 값과 i의 위치의 값을 바꾼다.)

		quickSort(a, i); //피봇의 왼쪽으로 재귀함수 시작
		quickSort(a+i+1, n-i-1); //피봇의 오른쪽으로 재귀함수 시작
	}


	return 0;
}

int hashCode(int key) {
   return key % MAX_HASH_TABLE_SIZE; //제산 함수를 사용해 모듈러 연산을 해서 return
}

int hashing(int *a, int **ht) //배열의 데이터를 해시 테이블에 저장하는 함수
{
	int *hashtable = NULL;

	/* hash table이 NULL인 경우 메모리 할당 */
	if(*ht == NULL) {
		hashtable = (int*)malloc(sizeof(int) * MAX_ARRAY_SIZE);
		*ht = hashtable;  /* 할당된 메모리의 주소를 복사 --> main에서 배열을 control 할수 있도록 함*/
	} else {
		hashtable = *ht;	/* hash table이 NULL이 아닌경우, table 재활용, reset to -1 */
	}

	for(int i = 0; i < MAX_HASH_TABLE_SIZE; i++)
		hashtable[i] = -1;

	/*
	for(int i = 0; i < MAX_HASH_TABLE_SIZE; i++)
		printf("hashtable[%d] = %d\n", i, hashtable[i]);
	*/

	int key = -1;
	int hashcode = -1;
	int index = -1;
	for (int i = 0; i < MAX_ARRAY_SIZE; i++)
	{
		key = a[i]; //key 값 저장
		hashcode = hashCode(key); //해시코드 저장
		
		printf("key = %d, hashcode = %d, hashtable[%d]=%d\n", key, hashcode, hashcode, hashtable[hashcode]);
		
		if (hashtable[hashcode] == -1)
		{
			hashtable[hashcode] = key; //저장된게 없으면 key값 저장
		} else 	{ //있으면

			index = hashcode;

			while(hashtable[index] != -1)
			{
				index = (++index) % MAX_HASH_TABLE_SIZE; //빈공간을 찾아서 할당한다
				/*
				printf("index = %d\n", index);
				*/
			}
			hashtable[index] = key; //빈공간에 key 저장
		}
	}

	return 0;
}

int search(int *ht, int key) //해시 테이블에서 key 값을 찾는 함수
{
	int index = hashCode(key); //hashCode함수를 통해 key 값을 해시 테이블에 맞게 변경

	if(ht[index] == key) //key 값에 맞는 것을 해시 테이블에서 찾으면 index 리턴 
		return index;

	while(ht[++index] != key) //충돌이 일어났을 경우이면 
	{
		index = index % MAX_HASH_TABLE_SIZE; //빈공간을 찾았던 알고리즘대로 찾는다
	}
	return index; //index 리턴
}



