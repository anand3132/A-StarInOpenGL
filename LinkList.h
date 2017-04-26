#ifndef LINKLIST_H 
#define LINKLIST_H

#include <stdio.h>
#include "astar.h"

//Forward declaration
struct stTile;

struct stNode
{
	stTile* Data;
	stNode* Next;												//point to next structure address

	stNode()
	{
		Next=NULL;
		Data=0;
	}

	~stNode()
	{
		//if(Next) delete Next;
		Data=NULL;
	}
};
//typedef stList* pstList;


class clList
{
public:
	clList();
	~clList();

	void AddANode(stTile* aTile);
	//void DeleteANode(stNode*);
	//stNode* Previous(stNode*);
	stNode* Find(stTile* aTile);
	BOOL Remove(stNode* aNode);
	void Reset();
	//stNode* FindLast();

	//bool FindItem(int,int);
	//void PrintList();
	//stNode* Previous(long);

	stNode* Head;
	stNode* Tail;
	stNode* CurrentPtr;			//initializing 3 structures head tail currentPtr 
};
#endif