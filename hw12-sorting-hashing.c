/*
 * hw12-sorting-hashing.c
 *
 *  Homework 12: Sorting & Hashing
 *  Department of Computer Science at Chungbuk National University
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h> //�������� �̱����� �������

#define MAX_ARRAY_SIZE			13	/* prime number */
#define MAX_HASH_TABLE_SIZE 	MAX_ARRAY_SIZE

/* �ʿ信 ���� �Լ� �߰� ���� */
int initialize(int **a); //�迭�� �ʱ�ȭ�ϴ� �Լ�(����������)
int freeArray(int *a); //�迭�� �Ҵ�� �޸𸮸� free ��Ű�� �Լ�
void printArray(int *a); //�迭�� ����ϴ� �Լ�

int selectionSort(int *a); //�迭�� ������ ������������ ���������ϴ� �Լ�
int insertionSort(int *a); //�迭�� ������ ������������ ���������ϴ� �Լ�
int bubbleSort(int *a); //�迭�� ������ ������������ ���������ϴ� �Լ�
int shellSort(int *a); //�迭�� ������ ������������ �� �����ϴ� �Լ�
/* recursive function���� ���� */
int quickSort(int *a, int n); //�迭�� ������ ������������ �� �����ϴ� �Լ�


/* hash code generator, key % MAX_HASH_TABLE_SIZE */
int hashCode(int key);

/* array a���� �� hash table�� �����. */
int hashing(int *a, int **ht);

/* hash table���� key�� ã�� hash table�� index return */
int search(int *ht, int key);


int main()
{
	char command; //��ɾ� ����
	int *array = NULL; //���� �迭�� �������� �Ҵ���� ������
	int *hashtable = NULL;
	int key = -1; //key �� -1�� �ʱ�ȭ
	int index = -1; //index �� -1�� �ʱ�ȭ

	srand(time(NULL)); //�ð��� seed�� �༭ �Ź� �ٸ� ���� seed�� ����
	printf("[----- ������ 2020039027 -----]\n\n"); //�̸� �й� ���
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

int initialize(int** a) //�迭�� �ʱ�ȭ �ϴ� �Լ�(�迭�� ������ �������� �ʱ�ȭ)
{
	int *temp = NULL;

	/* array�� NULL�� ��� �޸� �Ҵ� */
	if(*a == NULL) {
		temp = (int*)malloc(sizeof(int) * MAX_ARRAY_SIZE); //�迭�� ũ�⸸ŭ temp�� �޸� �Ҵ�
		*a = temp;  /* �Ҵ�� �޸��� �ּҸ� ���� --> main���� �迭�� control �Ҽ� �ֵ��� ��*/
	} else
		temp = *a;

	/* �������� �迭�� ������ ���� */
	for(int i = 0; i < MAX_ARRAY_SIZE; i++)
		temp[i] = rand() % MAX_ARRAY_SIZE;

	return 0;
}

int freeArray(int *a) //�迭�� free ��Ű�� �Լ�
{
	if(a != NULL)
		free(a); //�迭�� ũ�⸸ŭ �޸𸮸� �Ҵ���� �����͸� free��Ų��.
	return 0;
}

void printArray(int *a) //�迭�� ����ϴ� �Լ�
{
	if (a == NULL) { //�迭�� �������� �ʾ��� ��
		printf("nothing to print.\n");
		return;
	}
	for(int i = 0; i < MAX_ARRAY_SIZE; i++) //�迭�� ó������ ������ ���
		printf("a[%02d] ", i);
	printf("\n");
	for(int i = 0; i < MAX_ARRAY_SIZE; i++)
		printf("%5d ", a[i]);
	printf("\n");
}


int selectionSort(int *a) //�迭�� ���������ϴ� �Լ�
{
	int min; //���� ���� ���� ������ ����
	int minindex; //���� ���� ���� ��ġ�� ������ ����
	int i, j;

	printf("Selection Sort: \n");
	printf("----------------------------------------------------------------\n");

	printArray(a); //���ĵǱ� ������ �迭 ���

	for (i = 0; i < MAX_ARRAY_SIZE; i++)
	{
		minindex = i; //���ĵ��� ���� ù ��° ������ �ʱ�ȭ
		min = a[i]; //���ĵ��� ���� ù ��° ��ġ�� �ʱ�ȭ
		for(j = i+1; j < MAX_ARRAY_SIZE; j++) //�迭�� ũ�⸸ŭ �ݺ�
		{
			if (min > a[j]) //�� ���� ���� �߰��ϸ�
			{
				min = a[j]; //min�� ����
				minindex = j; //��ġ ����
			}
		}
		a[minindex] = a[i]; //min�� i�� ���� �ٲ۴�
		a[i] = min;
	}

	printf("----------------------------------------------------------------\n");
	printArray(a); //���ĵ� �迭 ���
	return 0;
}

int insertionSort(int *a) //�迭�� ���������ϴ� �Լ�
{
	int i, j, t;

	printf("Insertion Sort: \n");
	printf("----------------------------------------------------------------\n");

	printArray(a);

	for(i = 1; i < MAX_ARRAY_SIZE; i++)
	{
		t = a[i]; //���ĵ��� ���� �迭�� ù ��° ���� ����
		j = i; //���ĵ��� ���� �迭�� ù ��° ���� ��ġ ����
		while (a[j-1] > t && j > 0) //���ĵ� ���Ұ� �� ũ��, j�� 0���� ũ��
		{
			a[j] = a[j-1]; //���� ���� ���� ������ �ٲ۴�.
			j--; //ù ��° ���Ұ� �� ������ �ݺ�
		}
		a[j] = t; //���ĵ� �迭�� �´� ��ġ�� t ����
	}

	printf("----------------------------------------------------------------\n");
	printArray(a); //���ĵ� ���� �迭 ���

	return 0;
}

int bubbleSort(int *a) //�迭�� �������� �ϴ� �Լ�
{
	int i, j, t;

	printf("Bubble Sort: \n");
	printf("----------------------------------------------------------------\n");

	printArray(a);

	for(i = 0; i < MAX_ARRAY_SIZE; i++) //n^2 �� ���� �ݺ�
	{
		for (j = 0; j < MAX_ARRAY_SIZE; j++)
		{
			if (a[j-1] > a[j]) //���� ���� �� ũ�� �ٲ۴�.
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

int shellSort(int *a) //�迭�� �� ���� �ϴ� �Լ�
{
	int i, j, k, h, v;

	printf("Shell Sort: \n");
	printf("----------------------------------------------------------------\n");

	printArray(a);

	for (h = MAX_ARRAY_SIZE/2; h > 0; h /= 2) //h�� 1���� �۾��� ������ �ݺ�
	{
		for (i = 0; i < h; i++)
		{
			for(j = i + h; j < MAX_ARRAY_SIZE; j += h)
			{
				v = a[j];
				k = j;
				while (k > h-1 && a[k-h] > v)
				{
					a[k] = a[k-h]; //h��ŭ �������ִ� ���� ���������Ѵ�.
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

int quickSort(int *a, int n) //�迭�� �� ���� �ϴ� �Լ�(�����)
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
			while(a[++i] < v); //v���� ū ���� ���� ������ i�� ����
			while(a[--j] > v); //v���� ���� ���� ���� ������ j�� ����

			if (i >= j) break; //i�� j�� ����(cross)�ϸ� break
			t = a[i];
			a[i] = a[j];
			a[j] = t;
			//i�� ��ġ�� ���� j�� ��ġ�� ���� �ٲ۴�.
		}
		t = a[i];
		a[i] = a[n-1];
		a[n-1] = t;
		//i�� ��ġ�� ���� n-1�� ��ġ�� ���� �ٲ۴�.(�Ǻ��� ��ġ�� ���� i�� ��ġ�� ���� �ٲ۴�.)

		quickSort(a, i); //�Ǻ��� �������� ����Լ� ����
		quickSort(a+i+1, n-i-1); //�Ǻ��� ���������� ����Լ� ����
	}


	return 0;
}

int hashCode(int key) {
   return key % MAX_HASH_TABLE_SIZE; //���� �Լ��� ����� ��ⷯ ������ �ؼ� return
}

int hashing(int *a, int **ht) //�迭�� �����͸� �ؽ� ���̺� �����ϴ� �Լ�
{
	int *hashtable = NULL;

	/* hash table�� NULL�� ��� �޸� �Ҵ� */
	if(*ht == NULL) {
		hashtable = (int*)malloc(sizeof(int) * MAX_ARRAY_SIZE);
		*ht = hashtable;  /* �Ҵ�� �޸��� �ּҸ� ���� --> main���� �迭�� control �Ҽ� �ֵ��� ��*/
	} else {
		hashtable = *ht;	/* hash table�� NULL�� �ƴѰ��, table ��Ȱ��, reset to -1 */
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
		key = a[i]; //key �� ����
		hashcode = hashCode(key); //�ؽ��ڵ� ����
		
		printf("key = %d, hashcode = %d, hashtable[%d]=%d\n", key, hashcode, hashcode, hashtable[hashcode]);
		
		if (hashtable[hashcode] == -1)
		{
			hashtable[hashcode] = key; //����Ȱ� ������ key�� ����
		} else 	{ //������

			index = hashcode;

			while(hashtable[index] != -1)
			{
				index = (++index) % MAX_HASH_TABLE_SIZE; //������� ã�Ƽ� �Ҵ��Ѵ�
				/*
				printf("index = %d\n", index);
				*/
			}
			hashtable[index] = key; //������� key ����
		}
	}

	return 0;
}

int search(int *ht, int key) //�ؽ� ���̺��� key ���� ã�� �Լ�
{
	int index = hashCode(key); //hashCode�Լ��� ���� key ���� �ؽ� ���̺� �°� ����

	if(ht[index] == key) //key ���� �´� ���� �ؽ� ���̺��� ã���� index ���� 
		return index;

	while(ht[++index] != key) //�浹�� �Ͼ�� ����̸� 
	{
		index = index % MAX_HASH_TABLE_SIZE; //������� ã�Ҵ� �˰����� ã�´�
	}
	return index; //index ����
}



