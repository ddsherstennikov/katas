// tree_is_balanced.cpp
// Determine if the tree is balanced
//==================================

#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>
#include <set>
#include <iostream>
using namespace std;

typedef int      key_t;   // data type of the keys
typedef int      value_t; // data type of the value associated with the key

int max(int a, int b) { return a > b ? a : b; }
int abs(int a) {return a >= 0 ? a : -a; }

//data type of tree nodes
class node_t
{
public:
  key_t key;
	node_t *l, *r, *p;

	node_t(key_t k) : key(k), p(NULL), l(NULL), r(NULL) {}
	node_t() : key(0), p(NULL), l(NULL), r(NULL) {}

};

class tree
{
private:
	node_t*	merge(node_t* root);
	node_t* maximum(node_t* root);
	node_t* minimum(node_t* root);
	void removeChildrenAndRoot(node_t* root);

public:
	node_t* root;

	tree(node_t* n) : root(n) {}
	~tree();

	node_t*	search(key_t key, node_t* root);
	void insert(node_t *n, node_t* root);
	void remove(node_t *n);
	int is_balanced(node_t* n);

};

void tree::removeChildrenAndRoot(node_t* root)
{
	//delete all on the left and right of root
	if((!root->l)&&(!root->r))
		delete root;
	if((root->l)&&(!root->r))
		removeChildrenAndRoot(root->l);
	if((!root->l)&&(root->r))
		removeChildrenAndRoot(root->r);
	if((root->l)&&(root->r))
	{
		removeChildrenAndRoot(root->l);
		removeChildrenAndRoot(root->r);
	}
	
	return;
}

tree::~tree()
{
	removeChildrenAndRoot(root);
}

void tree::insert(node_t* n, node_t* rt)
{
	if((rt->l) &&  (n->key < rt->key))
	{ insert(n, rt->l); return; }
	if((rt->r) && (rt->key < n->key))
	{ insert(n, rt->r); return; }
	if(n->key < rt->key)
	{ rt->l = n; n->p = rt; return; }
	if(rt->key < n->key)
	{ rt->r = n; n->p = rt; return; }

	//if root->key == n->key, do nothing
}

node_t*	tree::search(key_t key, node_t* root)
{
if(root->l && key < root->key)
return search(key,root->l);
if(root->r && root->key < key)
return search(key, root->r);
if(root->key == key)
return root;
return NULL;
}

node_t*	tree::merge(node_t* root)
{
	node_t* newroot;

	if(root->r)
	{
		//find new root
		newroot = minimum(root->r);

		//remove it from tree: it`s right branch min => has no l or r neighbours
		if(newroot->p->key < newroot->key)
			newroot->p->r = NULL;
		if(newroot->key < newroot->p->key)
			newroot->p->l = NULL;

		//establish r & l connections
		newroot->l = root->l;
		newroot->r = root->r;
		if(newroot->l) newroot->l->p = newroot;
		if(newroot->r) newroot->r->p = newroot;

		return newroot;
	}

	else if(root->l)
	{
		//find new root
		newroot = root->l;

		//remove it from tree: it`s left branch max => is root->l
		root->l = NULL;

		//l and r connections are ok
		//p-conn. is set in remove method
		return newroot;
	}

	else return NULL;
}

void tree::remove(node_t* n)
{
	//if null, do nothing
	if(!n)
		return;

	//else merge and delete
	node_t *p = n->p;
	if(p->l == n)
	{p->l = merge(n); if(p->l) p->l->p = p;}
	else 
	{p->r = merge(n); if(p->r) p->r->p = p;}
	delete (n);
	return;
}

node_t* tree::maximum(node_t *root)
{
	if(root->r)
		return maximum(root->r);
	else
		return root;
}

node_t* tree::minimum(node_t *root)
{
	if(root->l)
		return minimum(root->l);
	else
		return root;
}

int tree::is_balanced(node_t* n)
{
	if(!(n->l) && !(n->r))
		return 1;
	else if(!(n->l) && (n->r))
	{
		int r = is_balanced(n->r);
		if ( r == 1 ) return (r + 1);
		else return 0;
	}
	else if((n->l) && !(n->r))
	{
		int l = is_balanced(n->l);
		if ( l == 1 ) return (l + 1);
		else return 0;
	}
	else
	{
		int l = is_balanced(n->l);
		int r = is_balanced(n->r);
		
		if ( ( abs(r - l) <= 1 ) && ( r ) && ( l ) ) return(1 + max(l, r));
		else return 0;
	}
}


int main(int argc, char* argv[])
{
	//1) create tree
	int a;
	cin >> a;
	if(a == 0) {	cout << "NO";	return -1; }
	node_t *n = new node_t(a);
	tree T(n);

	//2) fill tree
	cin >> a;
	while(a != 0)
	{
		node_t* n = new node_t(a);
		T.insert(n, T.root);
		cin >> a;
	}

	//3) check tree
	if( T.is_balanced(T.root) )
		cout << "YES";
	else
		cout << "NO";

	return 0;
}

