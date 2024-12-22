#pragma once
#include "cocos2d.h"
#include "AstarItem.h"
using namespace cocos2d;
class Astar
{
private:
	int curCol, curRow, aimCol, aimRow;
	int AimX, AimY, AimW, AimH; 
	int* map;
	int mapWidth;//地图宽
	int mapHeight;//地图的高
    CCArray *open;
    CCArray *close;
    CCArray *path;
public:
	Astar(void);
	~Astar(void);
	int getG(int col,int row,int id);
	int getH(int col,int row);
	void fromopentoclose();
	void removefromopen();
	void getpath();
	void starseach(int fid);
	void resetSort(int last);
	bool checkclose(int col,int row);
	void addtoopen(int col,int row,int id);
	bool checkmap(int col,int row);
	bool checkOpen(int col,int row,int id);
    CCArray *findPath(int curX, int curY, int aimX, int aimY, int* passmap,int map_width,int map_height);

	void  setMapObstruct(int *mapObstruct);
};
