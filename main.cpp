#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include"tree.h"

#define KEY_SIZE 2

class CPoint
{
public:
	int x;
	int y;
	int dir;
	void print();
};
void CPoint::print()
{
	printf("point[%d,%d]-%d\n",x,y,dir);
}

void display(void *k,void *d)
{
	int *key=(int *)k;
	CPoint *p=(CPoint *)d;
	printf("key[");
	for(int n=0;n<KEY_SIZE;n++)
		printf (" %d ",key[n]);
	printf("] ");
	p->print();
}
CTree tree(KEY_SIZE,sizeof(CPoint));

int key[2];
int main(int argc,char *argv[])
{
	srand(time(NULL));
	CPoint data;
	printf("create\n");
	for(int n=0;n<10;n++)
	{
		key[0]=rand()%100;
		key[1]=rand()%100;
		data.x=		rand()%10;
		data.y=		rand()%10;
		data.dir=	rand()%4;
		tree.add(key,(void *)&data);
	}
	printf("Start\n-----\n");
	tree.list(display);
	for(;;)
	{
		key[0]=rand()%100;
		key[1]=rand()%100;
		if(tree.remove(key))
		{
			printf("remove key[%d,%d]\n",key[0],key[1]);
			char c;
			c=getc(stdin);
			if(c=='x')break;
			tree.list(display);
		}
	}
	printf("End\n");
	return 0;
}
