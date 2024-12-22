#include "Astar.h"
#include "Astaritem.h"

Astar::Astar(void)
{
	path = new CCArray();
	open = new CCArray();
	close = new CCArray();
}

Astar::~Astar(void)
{
  
	 //open->removeAllObjects();
 

	 //close->removeAllObjects();
  

     // path->removeAllObjects();
	  
	CC_SAFE_RELEASE(open);
    CC_SAFE_RELEASE(close);
    CC_SAFE_RELEASE(path);

	delete []map;
}
CCArray *Astar::findPath(int curX, int curY, int aimX, int aimY, int* passmap,int map_width,int map_height)
{
    //参数以及记录路径数组初始化
	curCol = curX;
    curRow = curY;
	aimCol = aimX;
	aimRow = aimY;
	map = passmap;
	mapWidth = map_width;
	mapHeight = map_height;

	path->removeAllObjects();

	
    AstarItem * temp = new AstarItem();
	open->addObject(temp);
	AstarItem * temp1 = new AstarItem();
	temp1->setpos(curCol,curRow);
	temp1->setg(0);
	int ag = getH(curCol,curRow);
	temp1->seth(ag);
	temp1->setfid(0);
	temp1->setf(ag);
	open->addObject(temp1);
	

    //遍历寻找路径
	while(open->count() > 1){
	   fromopentoclose();//open和close列表管理
	   int fatherid = close->count() - 1;
	   if(abs(aimCol -((AstarItem *)close->objectAtIndex(fatherid))->getcol()) ==0 && abs(aimRow - ((AstarItem *)close->objectAtIndex(fatherid))->getrow()) == 0){
		   getpath();
		   break;
	   }else{
           //搜索
	       starseach(fatherid);
	   }
	}

	open->removeAllObjects();
	close->removeAllObjects();

    //获得路径
	if(path->count() == 0){
	   CCLog("NO PATH");
	   return NULL;
	}else{
		if(((AstarItem *)path->lastObject())->getcol() != aimCol || ((AstarItem *)path->lastObject())->getrow() != aimRow){
		   AstarItem * temp = new AstarItem();
	       temp->setpos(aimCol,aimRow);
		   path->addObject(temp);
		   delete temp;
		}
		return path;
	}
}

void  Astar::setMapObstruct(int *mapObstruct){
	//int  *map = mapObstruct;

	/* for(int i=0;i<13;i++){
	   for(int j=0;j<14;j++){
		   CCLog(" %d",map[i*14+j]);
		   //cout<<" "<<mapObstruct[i*mapWidthTileNum+j];
		  // printf(" %d",mapObstruct[i*mapWidthTileNum+j]);
	   }
	   CCLog("LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL");
   } */
}
void Astar::starseach(int fid)
{
	int col = ((AstarItem *)close->objectAtIndex(fid))->getcol();
	int row = ((AstarItem *)close->objectAtIndex(fid))->getrow();
    //搜索目前点的上下左右四个方向
	int mycol = col;
	int myrow = row - 1;
	if(myrow >= 0 && checkmap(mycol,myrow)){
		if(checkOpen(mycol,myrow,fid) && checkclose(mycol,myrow)){
		   addtoopen(mycol,myrow,fid);
		}
	}
	mycol = col - 1;
	myrow = row;
	if(mycol >= 0 && checkmap(mycol,myrow)){
		if(checkOpen(mycol,myrow,fid) && checkclose(mycol,myrow)){
		   addtoopen(mycol,myrow,fid);
		}
	}
	mycol = col;
	myrow = row + 1;
	if(myrow <=mapWidth && checkmap(mycol,myrow)){
		if(checkOpen(mycol,myrow,fid) && checkclose(mycol,myrow)){
		   addtoopen(mycol,myrow,fid);
		}
	}
	mycol = col + 1;
	myrow = row;
	if(mycol <= mapHeight && checkmap(mycol,myrow)){
		if(checkOpen(mycol,myrow,fid) && checkclose(mycol,myrow)){
		   addtoopen(mycol,myrow,fid);
		}
	}
}
void Astar::addtoopen(int col,int row,int id)
{
    //向open列表中加入点
    AstarItem * temp = new AstarItem();
	temp->setpos(col,row);
	temp->setfid(id);
	int g = getG(col, row, id);
	int h = getH(col, row);
	temp->setg(g);
	temp->seth(h);
	temp->setf(g + h);
	open->addObject(temp);
	resetSort(open->count() - 1);

	//CCLog("addtoopen--------col:%d,row:%d,f:%d",col,row,temp->getf());
}
bool Astar::checkclose(int col,int row)
{
    //检查close列表
	for(int i = close->count() - 1;i >= 0;i --){
		//CCLog("checkclose--------col:%d,row:%d",((AstarItem *)close->objectAtIndex(i))->getcol(),((AstarItem *)close->objectAtIndex(i))->getrow());
        if(((AstarItem *)close->objectAtIndex(i))->getcol() == col && ((AstarItem *)close->objectAtIndex(i))->getrow() == row){
	       
           return false; 
		}
	}
	return true;
}
bool Astar::checkOpen(int col,int row,int id)
{
    //检查open列表中是否有更小的步长
	for(int i = open->count() - 1;i >= 0;i --){
		if(((AstarItem *)open->objectAtIndex(i))->getcol() == col && ((AstarItem *)open->objectAtIndex(i))->getrow() == row){
		    int tempG = getG(col,row,id);
			if(tempG < ((AstarItem *)open->objectAtIndex(i))->getg()){
			   ((AstarItem *)open->objectAtIndex(i))->setg(tempG);
			   ((AstarItem *)open->objectAtIndex(i))->setfid(id);
			   ((AstarItem *)open->objectAtIndex(i))->setf(tempG + ((AstarItem *)open->objectAtIndex(i))->geth());
			   resetSort(i);
			}
			return false;
		}
	}
	return true;
}
bool Astar::checkmap(int col,int row)
{
    //检查地图中是否有碰撞
   if(abs(aimCol - col) > 0 || abs(aimRow - row) > 0){
	   if(map[row*mapWidth+col]==1){
		   // CCLog("false--------col:%d,row:%d",col,row);
		    return false;
	   }
   }
  // CCLog("true--------col:%d,row:%d",col,row);
   return true;
}
void Astar::getpath()
{
    //从整个close数组中找出路径
	/*if(path->count() == 0){
		path->addObject(close->objectAtIndex(close->count() - 1));
	}else{
	    path->insertObject(close->objectAtIndex(close->count() - 1),path->count() - 1);
	}
	while(true){
		if(((AstarItem *)path->objectAtIndex(0))->getg() == 0){
		   break;
		}
		path->insertObject(close->objectAtIndex(((AstarItem *)path->objectAtIndex(0))->getfid()),0);
	}
	curCol = aimCol;
	curRow = aimRow;*/
	path->addObject(close->lastObject());
    while (true)
    {
        if (((AstarItem *)path->objectAtIndex(0))->getg() == 0)
        {  //达到终点时,结束循环
            break;
        }
        path->insertObject(close->objectAtIndex(((AstarItem *)path->objectAtIndex(0))->getfid()),0);
    }

    curCol = aimCol;
    curRow = aimRow;
}
void Astar::fromopentoclose()
{
    //把open列表中的点放到close列表中
	AstarItem * temp = (AstarItem *)open->objectAtIndex(1);
	close->addObject(temp);
	removefromopen();

	//CCLog("fromopentoclose--------col:%d,row:%d,f:%d",temp->getcol(),temp->getrow(),temp->getf());
}
void Astar::removefromopen()
{
	int last = open->count() - 1;
	open->replaceObjectAtIndex(1,open->lastObject(),true);
	open->removeLastObject();
	last = open->count() - 1;
    //堆排序
	int head = 1;
    while ( (head * 2) <= last )
    {
        int child1 = head * 2;
        int child2 = child1 + 1;
        int childMin(0);

        //找出 childMin
        if (child2 <= last)
        {
            AstarItem *child1AstarItem = (AstarItem *)open->objectAtIndex(child1);
            AstarItem *child2AstarItem = (AstarItem *)open->objectAtIndex(child2);
            childMin = child1AstarItem->getf() < child2AstarItem->getf() ? child1 : child2;
        }
        else
        {
            childMin = child1;
        }
        //head > childMin就交换
        AstarItem *headAstarItem = (AstarItem *)open->objectAtIndex(head);
        AstarItem *childMinAstarItem = (AstarItem *)open->objectAtIndex(childMin);
        if (headAstarItem->getf() <= childMinAstarItem->getf())
        {
            break;
        }
        open->exchangeObject(headAstarItem, childMinAstarItem);
        head = childMin;

    }
}
void Astar::resetSort(int last)
{
    //根据步长排序，堆排序
	/*while(last > 1){
	   int half = last/2;
	   if(((AstarItem *)open->objectAtIndex(half))->getf() <= ((AstarItem *)open->objectAtIndex(last))->getf())
		   break;
	   AstarItem * temp = (AstarItem *)open->objectAtIndex(last);
	   open->replaceObjectAtIndex(last,open->objectAtIndex(half),false);
	   open->replaceObjectAtIndex(half,temp,true);
	   temp->release();
	}*/

	/*for(int i = 0;i<open->count();i++){

		 for(int j=i+1;j<open->count();j++) 
         { 
			 if(((AstarItem *)open->objectAtIndex(j))->getf()<((AstarItem *)open->objectAtIndex(i))->getf()) 
              { 
                AstarItem * temp  = (AstarItem *)open->objectAtIndex(i); 
				open->replaceObjectAtIndex(i,open->objectAtIndex(j),false);
                //pData[i] = pData[j]; 
				  open->replaceObjectAtIndex(j,temp,true);
               // pData[j] = iTemp;
				 temp->release();
              } 

		 }
	}*/

	while (last > 1)
    {
        int half = last / 2;
        AstarItem *lastAstarItem = (AstarItem *)open->objectAtIndex(last);
        AstarItem *halfAstarItem = (AstarItem *)open->objectAtIndex(half);
        if (halfAstarItem->getf() <= lastAstarItem->getf())
			break;
        open->exchangeObject(halfAstarItem, lastAstarItem);
        last = half;
    }

}
int Astar::getG(int col,int row,int id)
{
    //获得该点的g函数值
	int fx = ((AstarItem *)close->objectAtIndex(id))->getcol();
	int fy = ((AstarItem *)close->objectAtIndex(id))->getrow();
	int fg = ((AstarItem *)close->objectAtIndex(id))->getg();
	if(fx - col != 0 && fy - row != 0){
	   return fg + 14;
	}else{
	   return fg + 10;
	}
}
int Astar::getH(int col,int row)
{
    //获得该点的h值
	return abs(aimCol - col) * 10 + abs(aimRow - row) * 10;
}