#ifndef _TREE
#define _TREE
#include<stdio.h>
class CNode
{
public:
	int *key;
	void *data;
	CNode *left;
	CNode *right;
	CNode *parent;
public:
	static int depth;
	CNode();
	void define(CNode *pParent,int ks,int ds,const int *pkey,const void *pdata);	
	int compair(int ks,int *other);
	void list(void(*f)(void *,void *));
	void save(FILE *f,int ks,int ds);
	void clean();
	~CNode();
};
class CTree
{
	CNode *root;
	int keySize;
	int dataSize;
	CNode *addSub(CNode *node,const int *k,const void *d);
	CNode *findSub(CNode *pNode,const int *k);
public:
	CTree(int ks,int ds);
	~CTree();
	CNode *add(const int *k,const void *d);
	CNode *find(const int *k);
	bool remove(const int *k);
	void list(void(*f)(void *,void *));
	void depthList(void(*f)(void *,void *));
	void save(const char *name);
	void load(const char *name);
};
#endif
