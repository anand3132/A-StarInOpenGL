#include "astar.h"

stTile::stTile()															
{
	x=y		=0;
	width	=height=0;
	id		=ENONE;
	Gvalue	=NULL;
	Hvalue	=NULL;
	Fvalue	=NULL;
	loc		=-1;
	iParent	=NULL;
}

stTile::~stTile(){}
void stTile::InitST(int ax, int ay, int aWidth, int aHeight)						//STRUCTURE FUNCTION
{
	x=ax;	 y=ay;
	width	=aWidth;
	height	=aHeight;
}

AStarEngine::AStarEngine()
{
	nCols=nRows	=0;
	cell_width	=cell_height=0;
	pstmSource	=NULL;
	pstmDest	=NULL;

	pclmOpenList	= new clList();
	pclmClosedList	= new clList();
}

AStarEngine::~AStarEngine()
{
	delete[] pstmTileLst;															//deleting array...

	delete pclmOpenList;
	delete pclmClosedList;
}

void AStarEngine::InitGL(int Rows, int Cols, int aCellWidth, int aCellHeight)	
{
	nRows=Rows;
	nCols=Cols;
	cell_width=aCellWidth;
	cell_height=aCellHeight;

	pstmTileLst = new stTile[nRows*nCols];											//Cell array

	int aIterator=0;
	for(int i=0;i<nRows; i++)
	{
		for(int j=0;j<nCols;j++)
		{
			pstmTileLst[aIterator].InitST(START_X+j*cell_width, START_Y+i*cell_height, cell_width, cell_height);
			pstmTileLst[aIterator].loc=aIterator;
			aIterator++;
		}
	}
}

void AStarEngine::DrawMap()
{
	glPushMatrix();
		int aIterator=0;
		for(int i=0;i<nRows; i++)															//Loop to draw rows.....rectangles Up
		{
			for(int j=0;j<nCols;j++)														//Loop to draw columns..rectangles from left to right
			{
					glColor3f(0.f,0.0f,1.0f);												//blue lines...grid
					if(pstmTileLst[aIterator].id==EBLOCK)				
					{
						glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
						glColor3f(0.2f,0.5f,0.5f);											//block..Yellow
					}
					else if(pstmTileLst[aIterator].id==ESOURCE)		
					{
						
						glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);							//Red---source
						glColor3f(1.f,0.0f,0.0f);
					}
					else if(!pstmTileLst[aIterator].id==EBLOCK)								//clearing the block---green lines--grid
						glColor3f(0.f,0.0f,1.0f);
					else if(pstmTileLst[aIterator].id==EDESTINATION)
					{
						glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);							//green--destination
						glColor3f(0.f,1.0f,0.0f);
					}
					glBegin(GL_QUADS);
						glVertex2i(pstmTileLst[aIterator].x,									pstmTileLst[aIterator].y);
						glVertex2i(pstmTileLst[aIterator].x+pstmTileLst[aIterator].width,		pstmTileLst[aIterator].y);
						glVertex2i(pstmTileLst[aIterator].x+pstmTileLst[aIterator].width,		pstmTileLst[aIterator].y+pstmTileLst[aIterator].height);
						glVertex2i(pstmTileLst[aIterator].x,									pstmTileLst[aIterator].y+pstmTileLst[aIterator].height);
					glEnd();
					if (pstmTileLst[aIterator].id==EPATH)									//drawing the path
					{
						glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
						glColor3f(1.f,0.2f,0.5f);
						glBegin(GL_QUADS);
							glVertex2i(pstmTileLst[aIterator].x+2.5,									pstmTileLst[aIterator].y+2.5);
							glVertex2i(pstmTileLst[aIterator].x+pstmTileLst[aIterator].width-2.5,		pstmTileLst[aIterator].y+2.5);
							glVertex2i(pstmTileLst[aIterator].x+pstmTileLst[aIterator].width-2.5,		pstmTileLst[aIterator].y+pstmTileLst[aIterator].height-2.5);
							glVertex2i(pstmTileLst[aIterator].x+2.5,									pstmTileLst[aIterator].y+pstmTileLst[aIterator].height-2.5);
						glEnd();
					}
					//return back to green grid line
					if(pstmTileLst[aIterator].id==EBLOCK)							
						glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

					if(pstmTileLst[aIterator].id==EPATH)
						glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

					if(pstmTileLst[aIterator].id==ESOURCE || pstmTileLst[aIterator].id==EDESTINATION)
						glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

					aIterator++;
			}//for(int j=0;j<nCols;j++)	
		}//for(int i=0;i<nRows; i++)		
	glPopMatrix();
}

void AStarEngine::Mouse_click(int x, int y, int flag)
{
	int aIterator=0;
	for(int i=0;i<nRows; i++)
	{
		for(int j=0;j<nCols;j++) 
		{
			int cx=x/CELL_WIDTH;
			int cy=(maxy-y)/CELL_HEIGHT;
			if(i==cy && j==cx )															//key config with mouse
			{
				if((flag&MK_SHIFT) && !(flag&MK_CONTROL) )								//Block //shift only		
				{	pstmTileLst[aIterator].id=EBLOCK;		}
									
				else if((flag&MK_CONTROL) && !(flag&MK_SHIFT) && (flag&MK_LBUTTON))		//source--Red//control only
				{
					if(pstmSource)														//is red already drawn ? 
					{	pstmSource->id=ENONE;		}	

					pstmSource=&pstmTileLst[aIterator];
					pstmSource->id=ESOURCE;
					pstmSource->loc=aIterator;
					
                     
				}
				
				else if((flag&MK_CONTROL) && (flag&MK_SHIFT) && (flag&MK_LBUTTON))		//destination--Blue
				{
					if(pstmDest)
					{	pstmDest->id=ENONE;		}
					pstmDest=&pstmTileLst[aIterator];


					pstmDest->id=EDESTINATION;
					pstmDest->loc=aIterator;
				}
				else if(!(flag&MK_SHIFT) && !(flag&MK_CONTROL) && (flag&MK_LBUTTON))
				{
 					pstmTileLst[aIterator].id=ENONE;
				}
			}
			
			aIterator++;
		}
	}
}

//------------------------------------------------------------------------------------------------------------------------------------------------------
//																	Astar Algorithm
//------------------------------------------------------------------------------------------------------------------------------------------------------
BOOL AStarEngine::AddToopenList(stTile* aTile)		 
{
	pclmOpenList->AddANode(aTile);								//sent the tile to store in the pclmopenList(open link list)..tail
	return TRUE;												//if not found add the tile
}

BOOL AStarEngine::AddToClosedList(stTile* aTile)				//path to be followed
{
	pclmClosedList->AddANode(aTile);							//sent the tile to store in the pclmClosedList(closed link list)..tail
	return TRUE;												//if not found add the tile
}
BOOL AStarEngine::DropFromOpenList(stTile* aTile)				
{
	stNode* aNodeToDel=pclmOpenList->Find(aTile);
	pclmOpenList->Remove(aNodeToDel);
	return TRUE;
}

BOOL AStarEngine::FindInOpenList(stTile* aTile)
{
	stNode* tempIterator=pclmOpenList->Head;
	while(tempIterator)
	{
		if(aTile==tempIterator->Data)
			return TRUE;
		tempIterator=tempIterator->Next;
	}

	return FALSE;
}
BOOL AStarEngine::FindInClosedList(stTile* aTile)
{
	stNode* tempIterator=pclmClosedList->Head;			

	while(tempIterator)
	{
		if(aTile==tempIterator->Data)		
			return TRUE;
		tempIterator=tempIterator->Next;
	}

	return FALSE;
}

void AStarEngine::Reset()												//This function is to reset the whole arrays Fvalue,Gvalue,Hvalue iParant and Id
{
	pclmOpenList->Reset();
	pclmClosedList->Reset();

	for(int i=0;i<MAX_COL*MAX_ROW;i++)
	{
		pstmTileLst[i].Fvalue=0;
		pstmTileLst[i].Gvalue=0;
		pstmTileLst[i].Hvalue=0;
		pstmTileLst[i].iParent=NULL;
		if(pstmTileLst[i].id==EPATH)
			pstmTileLst[i].id=ENONE;
	}
}

void AStarEngine::Clear()
{	
	for(int i=0;i<MAX_COL*MAX_ROW;i++)
	{
		pstmTileLst[i].id=ENONE;
	}
}
void AStarEngine::FindPath(stTile* aSourceTile, stTile* aDestTile, int& nPath)
{
	stTile* TempSource=aSourceTile;
	stTile* TempDest=aDestTile;
	stTile* aLessF=NULL;

	int Adjnode[8];
	int sRow=NULL;
	int sCol=NULL;

	int dRow=NULL;
	int dCol=NULL;

	Reset();

	AddToopenList(aSourceTile);

	if(TempSource!=NULL && TempDest!=NULL)
	{
		dRow=TempDest->loc/MAX_COL;																//storing the Particular ROW of the destination
		dCol=TempDest->loc%MAX_COL;																//storing the Particular COL of the destination

		while(TempSource->loc!=aDestTile->loc)													//loop until a path is found
		{
			//find the lowest F
			if(pclmOpenList->Head==NULL)														
			{
				MessageBox(NULL,"Can't Find a Source, Destination or a Path.","ERROR",MB_OK|MB_ICONEXCLAMATION);
				break;
			}

			stNode* aTempNode=pclmOpenList->Head;
				aLessF=aTempNode->Data;											

			while(aTempNode!=NULL)
			{
				if((aTempNode->Data->Fvalue < aLessF->Fvalue))									//FindLowest Fvalue
					aLessF=aTempNode->Data;
				aTempNode=aTempNode->Next;
			}

			TempSource=aLessF;																	//set the lowest F tile to current tile
			sRow =(TempSource->loc/MAX_COL);													//storing the Particular ROW of the Source
			sCol =(TempSource->loc%MAX_COL);													//storing the Particular COL of the Source
			
			DropFromOpenList(TempSource);
			AddToClosedList(TempSource);														//add it to closed list

			//find adjacent tiles
			Adjnode[0]=-1;
			if((sRow-1)>=0 && (sRow-1)<MAX_ROW && (sCol-1)>=0 && (sCol-1)<MAX_COL)
				Adjnode[0]=((MAX_COL*(sRow-1)) + sCol-1);										//aDownLeft			+14

			Adjnode[1]=-1;
			if((sRow-1)>=0 && (sRow-1)<MAX_ROW && (sCol)>=0 && (sCol)<MAX_COL)
				Adjnode[1]=((MAX_COL*(sRow-1)) +   sCol);										//aDown	

			Adjnode[2]=-1;
			if((sRow-1)>=0 && (sRow-1)<MAX_ROW && (sCol+1)>=0 && (sCol+1)<MAX_COL)
				Adjnode[2]=((MAX_COL*(sRow-1)) +   sCol+1);										//aDownRight		+14

			Adjnode[3]=-1;
			if((sRow)>=0 && (sRow)<MAX_ROW && (sCol-1)>=0 && (sCol-1)<MAX_COL)
				Adjnode[3]=((MAX_COL*(sRow))   + sCol-1);										//aLeftSide

			Adjnode[4]=-1;
			if((sRow)>=0 && (sRow)<MAX_ROW && (sCol+1)>=0 && (sCol+1)<MAX_COL)
				Adjnode[4]=((MAX_COL*(sRow))   + sCol+1);										//aRightSide

			Adjnode[5]=-1;
			if((sRow+1)>=0 && (sRow+1)<MAX_ROW && (sCol-1)>=0 && (sCol-1)<MAX_COL)
				Adjnode[5]=((MAX_COL*(sRow+1)) + sCol-1);										//aTopLeft			+14

			Adjnode[6]=-1;
			if((sRow+1)>=0 && (sRow+1)<MAX_ROW && (sCol)>=0 && (sCol)<MAX_COL)
				Adjnode[6]=((MAX_COL*(sRow+1)) +   sCol);										//aTop

			Adjnode[7]=-1;
			if((sRow+1)>=0 && (sRow+1)<MAX_ROW && (sCol+1)>=0 && (sCol+1)<MAX_COL)
				Adjnode[7]=((MAX_COL*(sRow+1)) + sCol+1);										//aTopRight			+14

			for(int i=0;i<8;i++)																//if the tile is not inside the window set it to -1
			{
				if(Adjnode[i]<0)																//is selected node is -ve
				{	Adjnode[i]=-1;						}
				else if(Adjnode[i]>MAX_ROW*MAX_COL)												//is selected higher than the maximum limit
				{	Adjnode[i]=-1;						}
				else if(pstmTileLst[Adjnode[i]].id==EBLOCK)										//is selected tile is block
				{	Adjnode[i]=-1;						}
			}//for(int i=0;i<8;i++)

			for(int x=0;x<8;x++)
			{
				if(Adjnode[x]!=-1)
				{	
					int adjRow =(pstmTileLst[Adjnode[x]].loc/MAX_COL);																
					int adjCol =(pstmTileLst[Adjnode[x]].loc%MAX_COL);
					int gcost=0;

					if((Adjnode[x]/MAX_COL)==sRow ||(Adjnode[x]%MAX_COL)==sCol)														//Giving the G value as 10 if the tile is horizontal or vertical  
						gcost=10;
					else																											//else if in digonal 14 is set
						gcost=14;
//---------------------------------------------------------------------------------------------------------------------------------------------------
					if(!FindInClosedList(&pstmTileLst[Adjnode[x]]))
					{
						if(!FindInOpenList(&pstmTileLst[Adjnode[x]]))
						{
							pstmTileLst[Adjnode[x]].iParent=TempSource;
							pstmTileLst[Adjnode[x]].Gvalue=TempSource->Gvalue+gcost;												//GValue
							pstmTileLst[Adjnode[x]].Hvalue=(abs(adjRow-dRow)+abs(adjCol-dCol))*10;									//Hvalue
							pstmTileLst[Adjnode[x]].Fvalue=pstmTileLst[Adjnode[x]].Gvalue+pstmTileLst[Adjnode[x]].Hvalue;			//Fvalue
							AddToopenList(&pstmTileLst[Adjnode[x]]);																//add to open list//selected node
						}
						else
						{
							int tRow =(pstmTileLst[Adjnode[x]].loc/MAX_COL);														//
							int tCol =(pstmTileLst[Adjnode[x]].loc%MAX_COL);
							int yy=abs(sRow-tRow);																					//gettin
							int xx=abs(sCol-tCol);
							int addedGCost=0;
							int tempGCost=0;
							if(xx==1 && yy==1)
								addedGCost=14;
							else
								addedGCost=10;

							tempGCost=TempSource->Gvalue+addedGCost;
							if(tempGCost < pstmTileLst[Adjnode[x]].Gvalue)
							{
								pstmTileLst[Adjnode[x]].iParent=TempSource;
								pstmTileLst[Adjnode[x]].Gvalue=tempGCost;
								pstmTileLst[Adjnode[x]].Fvalue=pstmTileLst[Adjnode[x]].Gvalue+pstmTileLst[Adjnode[x]].Hvalue;
							}
						}
					}
				}//if(Adjnode[x]!=-1)
			}//for(int x=0;x<8;x++)

		}//while(TempSource->loc!=aDestTile->loc)
//---------------------------------------------------------------------------------------------------------------------------------------------------
		//traverse through the path
		stTile* tempD=TempDest->iParent;
		while(true && tempD)
		{
			tempD->id=EPATH;
			tempD=tempD->iParent;
			if(tempD==aSourceTile) break;
		}
		
	}
	else
	MessageBox(NULL,"Can't Find A Source Or Destination .","ERROR",MB_OK|MB_ICONEXCLAMATION);
}

//---------------------------------------------------------------------------------------------------------------------------------------------------
//														Instructions
//--------------------------------------------------------------------------------------------------------------------------------------------------
