#include "LinkList.h"
#include <iostream>

using namespace std;

clList::clList()
{
	Head = NULL;												//allocating space for head
	Tail=NULL;
	CurrentPtr = Head;
}

/*stNode* clList::FindLast()
{
	stNode* aTemp=Head;
	while(true)
	{
		if(aTemp->Next==NULL) break;
		aTemp=aTemp->Next;
	}

	return aTemp;
}*/

void clList::AddANode(stTile* aTile)
{
	if(!Head)											//given tile structure is stored in tail->next->data
	{
		Head = new stNode();
		Tail=Head;
	}
	else
	{
		Tail->Next = new stNode();
		Tail=Tail->Next;
	}

	Tail->Data=aTile;
}

stNode* clList::Find(stTile* aTile)
{
	stNode* temp=Head;

	while(temp)
	{
		if(temp->Data==aTile) return temp;

		temp=temp->Next;
	}

	return NULL;
}

void clList::Reset()
{
	stNode* atemp=Head;
	
	while(Head)
	{
		stNode* t=Head->Next;
		delete Head;
		Head=t;
	}
	Tail=Head=NULL;
}

BOOL clList::Remove(stNode* aNode)
{
	stNode* temp;

	if(aNode==Head && aNode==Tail)
	{
		delete Head;
		Head=Tail=NULL;
	}
	else if(aNode==Head)											//if node is head
	{
		temp=Head;
		Head=Head->Next;
		delete temp;
	}
	else if(aNode==Tail)
	{
		if(Head==Tail)
		{
			delete Tail;
			Tail=Head=NULL;
		}
		else
		{
			temp=Head;
			while(temp && temp->Next!=aNode)
			{
				temp=temp->Next;
			}

			if(temp)
			{
				stNode* del=temp->Next;
				temp->Next=NULL;
				Tail=temp;
				delete del;
			}
		}
	}
	else
	{
		temp=Head;
		while(temp && temp->Next!=aNode)
		{
			temp=temp->Next;
		}

		if(temp)
		{
			stNode* del=temp->Next;
			temp->Next=del->Next;
			delete del;
		}
	}

	return TRUE;
}

//void clList::DeleteANode(stNode* corpse) 
//{
//	stNode* temp;
//
//	if(corpse == Head) 
//	{
//		temp=Head;
//		Head=Head->Next;
//		delete temp;
//	}
//	else if(corpse == Tail) 
//	{
//		temp=Previous(corpse);
//		temp->Next=NULL;
//		delete corpse;
//		Tail=temp;
//	}
//	else 
//	{
//		temp=Previous(corpse);
//		temp->Next=corpse->Next;
//		delete corpse;
//	}
//
//	CurrentPtr=Head; 
//}
//
//
clList::~clList()
{
	stNode* temp = Head;
	CurrentPtr = Head;

	while(CurrentPtr != NULL)
	{
		CurrentPtr = CurrentPtr->Next;
		delete temp;
		temp=CurrentPtr;
	}
}



//stNode* clList::Previous(stNode* index)
//{
//	stNode* temp=Head;
//
//	if(index==Head)				{	return Head;		}		
//	while(temp->Next != index)	{	temp=temp->Next;	}
//
//	return temp;
//}
