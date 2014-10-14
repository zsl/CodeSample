// biTreeToDoubleLink.cpp : main project file.
#include <cassert>
#include <iostream>

using namespace std;

typedef struct tagNode{
	int data;
	tagNode *left, *right;
} Node;

void SetNodeData(Node *n, int data, Node *left = NULL, Node *right = NULL)
{
	assert(n != NULL);

	n->data = data;
	n->left = left;
	n->right = right;
}

Node *Insert(Node *&n, int data)
{
	if (NULL == n){
		n = new Node;
		SetNodeData(n, data);
		return n;
	}
	else if (n->data > data){
		return Insert(n->left, data);
	}
	else{
		return Insert(n->right, data);
	}
}

Node *MostLeft(Node *n)
{
	assert(n != NULL);

	if ( NULL == n->left )
		return n;
	else 
		return MostLeft(n->left);
}

Node *MostRight(Node *n)
{
	assert(n != NULL);

	if ( NULL == n->right)
		return n;
	else
		return MostRight(n->right);
}

void TreeToDoubleLink(Node *head)
{
	if ( NULL == head )
		return;
	
	TreeToDoubleLink(head->left);
	TreeToDoubleLink(head->right);

	Node *nodeLeft_rightest = head->left == NULL ? NULL : MostRight(head->left);
	Node *nodeRight_leftest = head->right == NULL ? NULL : MostLeft(head->right);

	head->left = nodeLeft_rightest;
	if ( nodeLeft_rightest)
		nodeLeft_rightest->right = head;

	head->right = nodeRight_leftest;
	if ( nodeRight_leftest )
		nodeRight_leftest->left = head;
}

int main()
{
	Node *head = NULL;
	Insert(head, 10);
	Insert(head, 6);
	Insert(head, 4);
	Insert(head, 8);
	Insert(head, 14);
	Insert(head, 12);
	Insert(head, 16);

	Node *headDoubleLink = MostLeft(head);
	TreeToDoubleLink(head);

	for (Node *n = headDoubleLink; n != NULL; n = n->right){
		cout << n->data << " ";
	}

	return 0;
}
