#ifndef ASTAR_H_
#define ASTAR_H_

#define START_X 0									
#define START_Y 0
#define MAX_COL 64
#define MAX_ROW 48
#define CELL_HEIGHT 20
#define CELL_WIDTH 20


#include <Windows.h>
#include <gl//GL.h>
#include "LinkList.h"

enum TILEID											//Switches for tile drawing
{
	ENONE,
	EBLOCK,
	ESOURCE,
	EPATH,
	EDESTINATION
};

struct stTile										//structure for the co-ordinates of the tiles
{
	stTile();
	~stTile();

	int x,y;																							
	int width,height;
	int loc;
	int Gvalue;
	int Hvalue;
	int Fvalue;
	TILEID id;
	stTile* iParent;

	void InitST(int ax, int ay, int aWidth, int aHeight);
};

//forward declaration
class clList;

class AStarEngine
{
public:
	
	int maxy;
	AStarEngine();
	~AStarEngine();

	//Draw functions
	void InitGL(int Rows, int Cols, int aCellWidth, int aCellHeight);		
	void Mouse_click(int x, int y, int flag=0);								//Mouse functions
	void DrawMap();															//Draw functions
		
	// Astar algorithm 
	void FindPath(stTile* aSourceTile, stTile* aDestTile, int& nPath);		//Main Path ::algorithm

	BOOL AddToopenList(stTile* aTile);										//used to add tile in the open list
	BOOL AddToClosedList(stTile* aTile);
	BOOL DropFromOpenList(stTile* aTile);
	BOOL FindInOpenList(stTile* aTile);
	BOOL FindInClosedList(stTile* aTile);
	void Clear();
	void Instruction();														//Instructions
	void Reset();


	int nRows,nCols;
	int cell_width, cell_height;
	
	stTile* pstmTileLst;													//structure Pointer to tile array
	stTile*	pstmSource;														//structure pointer to Source Tile
	stTile*	pstmDest;														//structure Pointer to Destination Tile

	//Astar algorithm LinkList 
	clList* pclmOpenList;													//Class pointer to Open list
	clList* pclmClosedList;													//Class pointer to Closed list
	int F,G,H;
	int sLoc;
};

#endif