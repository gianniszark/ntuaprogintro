/*
 * ask1.cpp
 *
 *  Created on: 19 ��� 2016
 *      Author: Giannis
 */


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>

FILE *f, *g;

/*data structures*/
struct AuthorRecord {
	char textTitle[100];
	long Download;
	struct AuthorRecord *next;
};
typedef struct AuthorRecord *AuthorRecordType, AuthorRecord;


typedef struct {
	char firstName[40];
	char lastName[40];
	int idNumber;
	long sum, mo;  //fields for sum and average value
	AuthorRecordType text;
} AuthorType;


struct MemberNodeStruct {
	AuthorType *anAuthor;
	struct MemberNodeStruct *next;
};
typedef struct MemberNodeStruct MemberNode, *MemberNodeType;


AuthorRecordType NewText(AuthorRecordType tlist) {
	AuthorRecordType p, curr, prev;
	int flag;
	//book input
	p = (AuthorRecordType) malloc(sizeof(AuthorRecord));
	fscanf(f, "%[^\n]%*c", p->textTitle);
	fscanf(f, "%ld\n", &(p->Download));

	//case of null list
	if (tlist == NULL) {
		tlist = p;
		p->next = NULL;
		return (tlist);
	}
	//sorting

	//list with at least 1 elem
	curr = tlist;
	prev = NULL;
	while (curr != NULL) {
		flag = strcmp(p->textTitle, curr->textTitle);
		//same book
		if (flag == 0) {
			curr->Download = curr->Download + p->Download;
			free(p);
			return (tlist);
		}
		//found pos
		if (flag == -1) {
			if (prev == NULL) {
				p->next = tlist;
				tlist = p;
			} else {
				prev->next = p;
				p->next = curr;
			}
			return (tlist);
		}
		//didnt find pos
		if (flag == 1 && curr->next == NULL) {
			curr->next = p;
			p->next = NULL;
			return (tlist);
		}
		prev = curr;
		curr = curr->next;
	}
	//this should never happen
	return (tlist);

}

MemberNodeType NewAuthor(MemberNodeType list) {
	int i, n, flag;
	MemberNodeType p, curr, prev;

	//input author data
	p = (MemberNodeType)malloc(sizeof(MemberNode));
	p->anAuthor = (AuthorType*) malloc(sizeof(AuthorType));
	fscanf(f, "%s %s %d\n%d\n", p->anAuthor->firstName, p->anAuthor->lastName,
			&(p->anAuthor->idNumber), &n);

	//input book data
	p->anAuthor->text = NULL;
	for (i = 1; i <= n; i++)
		p->anAuthor->text = NewText(p->anAuthor->text);

	//place author in correct pos

	//case of null list
	if (list == NULL) {
		list = p;
		p->next = NULL;
		return (list);
	}
	//case of list with at least 1 elem
	curr = list;
	prev = NULL;
	while (curr != NULL) {
		flag = strcmp(p->anAuthor->lastName, curr->anAuthor->lastName);
		//same author
		if (flag == 0)
			if (strcmp(p->anAuthor->firstName, curr->anAuthor->firstName) == -1)
				flag = -1;
		//found pos
		if (flag == -1) {
			//is first in list
			if (prev == NULL) {
				p->next = list;
				list = p;
			}
			//is in the rest of the list
			else {
				prev->next = p;
				p->next = curr;
			}
			return (list);
		}
		//didnt find pos
		if (flag == 1 && curr->next == NULL) {
			curr->next = p;
			p->next = NULL;
			return (list);
		}
		prev = curr;
		curr = curr->next;
	}
	return (list);
}

void DownSum(MemberNodeType list) {  //calculate sum and average value
	long count;
	MemberNodeType p;
	AuthorRecordType t;

	for (p = list; p != NULL; p = p->next) {
		if (p->anAuthor->text == NULL) {
			p->anAuthor->sum = 0;
			p->anAuthor->mo = 0;
		} else {
			p->anAuthor->sum = 0;
			count = 0;
			for (t = p->anAuthor->text; t != NULL; t = t->next) {
				count++;
				p->anAuthor->sum += t->Download;
			}
			p->anAuthor->mo = p->anAuthor->sum / count;
		}
	}
}

MemberNodeType popular(MemberNodeType *list) { //create list for popular authors
	MemberNodeType prev, curr, p, plist;

	plist = NULL;
	/*check if current author is popular*/
	if ((*list)->anAuthor->sum > 10000) {
		plist = (*list);
		(*list) = (*list)->next;
		plist->next = NULL;
		p = plist;
	}
	while ((*list) != NULL) {
		if ((*list)->anAuthor->sum > 10000) {
			p->next = (*list);
			(*list) = (*list)->next;
			p = p->next;
			p->next = NULL;
		} else
			break;
	}

	curr = (*list);
	prev = NULL;
	while (curr != NULL) {
		if (curr->anAuthor->sum > 10000) {
			prev->next = curr->next;
			if (plist == NULL) {
				plist = curr;
				p = curr;
			} else {
				p->next = curr;
				p = p->next;
			}
			p->next = NULL;
			curr = prev;
		}
		prev = curr;
		curr = curr->next;
	}
	return (plist);
}

int length(MemberNodeType plist) { //list length
	MemberNodeType p;
	int count;
	count = 0;
	if (plist != NULL)
		for (p = plist; p != NULL; p = p->next)
			count++;
	return count;
}

int main() {
	int i, n, PopularCount;
	MemberNodeType MembersList, p, PopularList;
	AuthorRecordType t;

    //open files
	f = fopen("input.txt", "r");
	g = fopen("output.txt","w");
	fscanf(f, "%d\n", &n);
	//create memberlist
	MembersList = NULL;
	for (i = 1; i <= n; i++)
		MembersList = NewAuthor(MembersList);

	DownSum(MembersList); //calculate sum and average value

	//print authors and books
	for (p = MembersList; p != NULL; p = p->next) {
		for (t = p->anAuthor->text; t != NULL; t = t->next) {
			fprintf(g, "%s %s %s %ld\n", p->anAuthor->lastName,
					p->anAuthor->firstName, t->textTitle, t->Download);
		}
		fprintf(g, "%s %s %ld %ld\n\n", p->anAuthor->lastName,
				p->anAuthor->firstName, p->anAuthor->sum, p->anAuthor->mo);
	}

	PopularList = popular(&MembersList); //create Popularlist with popular authors
	PopularCount = length(PopularList); //number of popular authors

	//print popular authors
	fprintf(g,"Popular authors:\n");
	fprintf(g, "Total number of popular authors is %d\n", PopularCount);
	for (p = PopularList; p != NULL; p = p->next)
		fprintf(g, "%s %s %d\n", p->anAuthor->lastName, p->anAuthor->firstName,
				p->anAuthor->idNumber);
	fprintf(g, "END\n");



	//close the files
	fclose(f);
	fclose(g);
	return 0;
}




