#include<stdio.h>
#include<string.h>
#include "tree.h"
int CNode::depth;
CNode::CNode()
{
	key=0;
	data=0;
	left=0;
	right=0;
	parent=0;
};
void CNode::define(CNode *pParent,int ks,int ds,const int *pkey,const void *pdata)
{
	if(!key)key=new int[ks];
	if(!data)data=operator new (ds);
	for(int n=0;n<ks;n++)
		key[n]=pkey[n];
	memcpy(data,pdata,ds);
	parent=pParent;
};
int CNode::compair(int ks,int *other)
{
	for(int n=0;n<ks;n++)
	{
		if(key[n]>other[n])return -1;
		if(key[n]<other[n])return 1;
	}
	return 0;
};
void CNode::list(void(*f)(void *,void *))
{
	depth++;
	if(left)left->list(f);
	depth--;
	printf("depth=%d ",depth);
	f(key,data);
	depth++;
	if(right)right->list(f);
	depth--;
}

void CNode::save(FILE *f,int ks,int ds)
{
	fwrite(key,ks,sizeof(int),f);
	fwrite(data,ds,1,f);
	if(left)left->save(f,ks,ds);
	if(right)right->save(f,ks,ds);
}
void CNode::clean()
{
	if(key)delete key;
	if(data)operator delete (data);
	key=0;
	data=0;
	left=0;
	right=0;
}
CNode::~CNode()
{
	if(key)delete key;
	if(data)operator delete (data);
	if(left)delete left;
	if(right)delete right;
}
///////////////////////////////////////////////////
CTree::CTree(int ks,int ds)
{
	root=0;
	keySize=ks;
	dataSize=ds;
}
CNode *CTree::addSub(CNode *pNode,const int *k,const void *d)
{
	int comp;
	comp=pNode->compair(keySize,(int *)k);
	if(comp==0)
	{
		pNode->define(pNode->parent,keySize,dataSize,k,d);
		return pNode;
	}
	if(comp==-1)
	{
		if(pNode->left==0)
		{
			pNode->left=new CNode;
			pNode->left->define(pNode,keySize,dataSize,k,d);
			return pNode->left;
		}
		return addSub(pNode->left,k,d);
	}
	if(comp==+1)
	{
		if(pNode->right==0)
		{
			pNode->right=new CNode;
			pNode->right->define(pNode,keySize,dataSize,k,d);
			return pNode->right;
		}
		return addSub(pNode->right,k,d);
	}
	return NULL;
};
CNode *CTree::findSub(CNode *pNode,const int *k)
{
	int comp;
	comp=pNode->compair(keySize,(int *)k);
	if((comp==-1) && (pNode->left))
			return findSub(pNode->left,k);

	if((comp==+1) && (pNode->right))
			return findSub(pNode->right,k);

	if(comp==0)
			return pNode;
	return 0;
};


////////// Public /////////////////////////////////
CNode *CTree::add(const int *k,const void *d)
{
	if(root==0)
	{
		root=new CNode;
		root->define(0,keySize,dataSize,k,d);
		return root;
	};
	return addSub(root,k,d);
};

CNode *CTree::find(const int k[])
{
	if(root)return findSub(root,k);
	return 0;
};

bool CTree::remove(const int *k)
{
	CNode *node=0;
	if(root)node=findSub(root,k);

	if(node==0)
		return false;

	if(root==node)
	{
		if(node->left)
		{
			root=node->left;
			node->left->parent=root;
			CNode *p=root;
			while(p->right)p=p->right;
			p->right=node->right;
			if(node->right)
				node->right->parent=p;
		}else
		{
			root=node->right;
			if(node->right)
				node->right->parent=root;
		}
		node->clean();
		delete node;
		return true;
	}
	if(node==node->parent->left)
	{
		if(node->left)
		{
			node->parent->left=node->left;
			node->left->parent=node->parent;
			CNode *p=node->left;
			while(p->right)p=p->right;
			p->right=node->right;
			if(node->right)
				node->right->parent=p;
		}else
		{
			node->parent->left=node->right;
			if(node->right)
				node->right->parent=node->parent;
		}
		node->clean();
		delete node;
		return true;
	}else
	if(node==node->parent->right)
	{
		if(node->left)
		{
			node->parent->right=node->left;
			node->left->parent=node->parent;
			CNode *p=node->left;
			while(p->right)p=p->right;
			p->right=node->right;
			if(node->right)
				node->right->parent=p;
		}else
		{
			node->parent->right=node->right;
			if(node->right)
				node->right->parent=node->parent;
		}
		node->clean();
		delete node;
		return true;
	}
	// should never reach this point
	return false;
}

void CTree::list(void(*f)(void *,void *))
{
	if(root)
	{
		root->depth=0;
		root->list(f);
	}
};
void CTree::save(const char *name)
{
	FILE *f;
	f=fopen(name,"wb");
	fwrite(&keySize,sizeof(int),1,f);
	fwrite(&dataSize,sizeof(int),1,f);
	if(root)root->save(f,keySize,dataSize);
	fclose(f);
};
void CTree::load(const char *name)
{

	FILE *f;
	f=fopen(name,"rb");
	if(!f)return;
	if(root)delete root;
	root=0;
	fread(&keySize,sizeof(int),1,f);
	fread(&dataSize,sizeof(int),1,f);

	int *key=new int[keySize];
	void *data=operator new (dataSize);

	do{
		if(!fread(key,sizeof(int),keySize,f))
			break;
		if(!fread(data,dataSize,1,f))
			break;
		add(key,data);
	}while(true);
	delete key;
	operator delete(data);
	fclose(f);
}
CTree::~CTree()
{
	if(root)delete root;
}

