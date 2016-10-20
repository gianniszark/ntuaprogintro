#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>

struct pos_node {  //node with coordinates
	int x, y;
	struct pos_node *next;
};
typedef struct pos_node pos_node;

struct file_node { //node with filename
	char filename[30];
	struct file_node *next;
	pos_node *coordinates;
};
typedef struct file_node file_node;

struct word_node { //node for a word
	char *word;
	struct word_node *left, *right;
	file_node *location;
};
typedef struct word_node word_node, *word_nodeptr;

FILE *f, *cur; //f for input files, cur for the current file
char FileName[30];
word_nodeptr HashTable[1031];
int Table256[20];

int HashFunction(char *word) { /*calculate Hash of word*/
	int n, i, sum;

	n = strlen(word);
	sum = 0;
	for (i = 0; i <= n - 1; i++)
		sum = sum + (((Table256[i] % 1031) * (word[i] % 1031))) % 1031;
	sum = sum % 1031;
	return sum;
}

int ischar(char c) { //checks if character c is a letter of the alphabet
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
		return 1;
	return 0;
}

void InsertFile(file_node *head, int x, int y) { //word has already been added once
	file_node *p, *k;
	pos_node *last, *newNode;

	for (p = head; p->next != NULL; p = p->next)
		;
	if (strcmp(p->filename, FileName) == 0) { //word has already been added again in this file

		for (last = p->coordinates; last->next != NULL; last = last->next)
			;
		newNode = (pos_node*) malloc(sizeof(pos_node));
		newNode->x = x;
		newNode->y = y;
		newNode->next = NULL;
		last->next = newNode;
	} else {                               // word hasn't been added from this file yet
		k = (file_node*)malloc(sizeof(file_node));
		strcpy(k->filename, FileName);
		k->next = NULL;
		p->next=k;
		newNode = (pos_node*)malloc(sizeof(pos_node));
		newNode->x = x;
		newNode->y = y;
		newNode->next = NULL;
		k->coordinates=newNode;

	}
}

void InsertWord(word_nodeptr *head, char *mold, int x, int y) { //sorts word in tree (collision manage)
	int k;
	word_nodeptr p;
	if ((*head) == NULL) { //word does not exist in tree
		p = (word_nodeptr)malloc(sizeof(word_node));
		p->word = (char *)malloc(sizeof(strlen(mold) + 1));
		strcpy(p->word,mold);
		(*head) = p;
		p->left = p->right = NULL;
		p->location = (file_node*) malloc(sizeof(file_node));
		strcpy(p->location->filename, FileName);
		p->location->coordinates = (pos_node*) malloc(sizeof(pos_node));
		p->location->coordinates->x = x;
		p->location->coordinates->y = y;
		p->location->next = NULL;
		p->location->coordinates->next = NULL;

	} else {

		k = strcmp((*head)->word, mold);
		if (k == 0) { //word exists in tree
			InsertFile((*head)->location, x, y);
		}
		if (k > 0) //word smaller than head word
			return InsertWord(&(*head)->left, mold, x, y);
		if (k < 0) //word bigger than head word
			return InsertWord(&(*head)->right, mold, x, y);
	}
}

void NewWord(char *mold, int x, int y) { //process new word
	word_node *p;
	int Hash;

	Hash = HashFunction(mold);

	if (HashTable[Hash] == NULL) { // no other word in this cell
		p = (word_node*) malloc(sizeof(word_node));
		p->word = (char*) malloc(sizeof(strlen(mold) + 1));
		strcpy(p->word, mold);
		HashTable[Hash] = p;
		p->left = p->right = NULL;
		p->location = (file_node*) malloc(sizeof(file_node));
		p->location->coordinates = (pos_node*) malloc(sizeof(pos_node));
		p->location->coordinates->x = x;
		p->location->coordinates->y = y;
		p->location->next = NULL;
		strcpy(p->location->filename, FileName);
		p->location->coordinates->next = NULL;
	} else                        //there is a word in this cell
		InsertWord(&HashTable[Hash], mold, x, y);
}

void TextInput() {
	char mold[21], c;
	int x, y, i, x0, y0;

	x = y = 1;
	while (!feof(cur)) {
		c = fgetc(cur);
		if (ischar(c)) {
			i = 0;
			x0 = x;
			y0 = y;
			for (; ischar(c) == 1; c = fgetc(cur)) {
				mold[i] = c;
				i++;
			}
			mold[i] = '\0';
			NewWord(mold, x0, y0);
			y = y + i;
		}
		if (c == '\n') {
			y = 1;
			x++;
		} else if (c == ' ')
			y++;

	}
}

int create256(int i) {
	int res;
	if (i == 0)
		return (1);
	else {
		res = (256 * (create256(i - 1))) % 1031;
		return (res);
	}
}

word_nodeptr find(word_nodeptr head, char *word) {
	int k;

	if (head == NULL)
		return NULL;
	else {
		k = strcmp(head->word, word);
		if (k == 0)
			return head;
		else if (k == 1)
			return find(head->left, word);
		else
			return find(head->right, word);
	}
}

void test() { //test for searching a word
	char word[20];
	int Hash;
	word_node *p;
	file_node *q;
	pos_node *k;

	scanf("%s", word);
	Hash = HashFunction(word);
	if (HashTable[Hash] == NULL)
		printf("NOT FOUND");
	else {
		p = find(HashTable[Hash], word);
		if (p == NULL)
			printf("NOT FOUND");
		else {
			for (q = p->location; q != NULL; q = q->next)
				for (k = q->coordinates; k != NULL; k = k->next)
					printf("%s (%d,%d)\n", q->filename, k->x, k->y);
		}
	}
}

int main() {
	int i;


	for (i = 0; i <= 19; i++)
		Table256[i] = create256(i);
	for (i = 0; i <= 1030; i++)
		HashTable[i] = NULL;
	f = fopen("input.txt", "r");

	while (fscanf(f, "%s\n", FileName) != EOF) {

		cur = fopen(FileName, "r");
		TextInput();
		fclose(cur);
	}
	fclose(f);



	test();
	system("pause");

	return 0;
}


