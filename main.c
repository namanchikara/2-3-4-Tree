#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct node {
	int rno;

	char name[80];
	char add[80];
	char city[80];
	char email[80];

	struct node *parent;
	struct node *bro;
	struct node *lkido;
	struct node *rkido;
};
struct node *root = NULL;

void makeLeftChild(struct node *papa, struct node *kido)
{
	papa->lkido = kido;
	if (kido)
		kido->parent = papa;
}

void makeRightChild(struct node *papa, struct node *kido)
{
	papa->rkido = kido;
	if (kido)
		kido->parent = papa;
}

int onRightSide(struct node *node)
{

	struct node *parent = node->parent;
	if (parent && parent->rkido)
	{
		parent = parent->rkido;
		while (parent != NULL)
		{
			if (parent == node)
				return 1;
			parent = parent->bro;
		}
		return 0;
	}
	return 0;
}

int onLeftSide(struct node *node)
{
	struct node *parent = node->parent;
	if (parent && parent->lkido)
	{
		parent = parent->lkido;
		while (parent != NULL)
		{
			if (parent == node)
				return 1;
			parent = parent->bro;
		}
		return 0;
	}
	return 0;
}

void fixit(struct node *s, struct node *m, struct node *l);
int keys(struct node *node)
{
	int count = 0;
	struct node *temp = node;
	if (temp)
	{
		struct node *parent = temp->parent;
		if (parent)
		{
			if (onRightSide(node))
			{
				temp = parent->rkido;
			}
			else
			{
				temp = parent->lkido;
			}
		}
		else
			temp = root;

		while (temp != NULL)
		{
			temp = temp->bro;
			count += 1;
		}

		return count;
	}

	return 0;
}

int subchilds(struct node **node)
{
	int count = 0;
	struct node *temp = *node;

	while (temp != NULL)
	{
		if (temp->rkido)
			count += 1;
		if (temp->lkido)
			count += 1;
		temp = temp->bro;
	}
	return count;
}

struct node *getParent(struct node *node)
{
	struct node *parent = node->parent;
	if (parent)
	{
		struct node *grandparent = parent->parent;
		if (grandparent)
		{
			if (grandparent->rno > parent->rno)
			{
				return grandparent->lkido;
			}

			else
				return grandparent->rkido;
		}
		return root;
	}
	else return NULL;
}

struct node *getNextChild(struct node *node, int data)
{

	if (data < node->rno)
	{
		if (node->lkido)
			return node->lkido;

		return node;
	}

	if (data > node->rno && node->bro)
		return node->bro;

	if (data > node->rno && node->rkido)
		return node->rkido;

	return node;

}

struct node *getFirstNode(struct node *node)
{
	if (onLeftSide(node))
		return node->parent->lkido;

	if (onRightSide(node))
		return node->parent->rkido;

	return root;
}

void split(struct node *node)
{
	node = getFirstNode(node);

	int count = 0;
	struct node
		*first = node,
		*second = first->bro,
		*third = second->bro;

	makeRightChild(first, second->lkido);
	second->lkido = NULL;
	makeLeftChild(third, second->rkido);
	second->rkido = NULL;

	first->bro = NULL;

	if (first->parent)
		fixit(first, second, third);


	else {
		second->bro = first->parent;
		root = second;
		first->parent = second;
		third->parent = second;
		makeLeftChild(second, first);
		makeRightChild(second, third);
	}
}


void fixit(struct node *s, struct node *m, struct node *l)
{
	struct node *parent = getParent(m);
	if (keys(parent) >= 3)
		split(parent);

	if (keys(parent) == 2)
	{
		/*

		case:
				p - q			m - p - q
				/		 =>	  /    /
			(s)(m)(l)		 s    l

		*/
		if (m->rno < parent->rno) {

			if (parent == root)
				root = m;


			makeLeftChild(m, s);
			m->rkido = NULL;
			s->bro = NULL;

			makeLeftChild(parent, l);
			m->bro = parent;

			struct node *grandnode = parent->parent;

			m->parent = grandnode;

			if (grandnode)
			{
				if (grandnode->lkido == parent)
				{
					grandnode->lkido = m;
				}

				if (grandnode->rkido == parent)
				{
					grandnode->rkido = m;
				}
			}
		}

		else if (parent->rno < m->rno && m->rno < parent->bro->rno)
		{
			m->bro = parent->bro;
			m->parent = parent->parent;
			parent->bro = m;
			parent->rkido = NULL;
			makeLeftChild(m, s);
			makeRightChild(m, l);

		}

		else if (parent->rno < m->rno && m->rno > parent->bro->rno) {

			makeLeftChild(parent->bro, parent->rkido);
			parent->rkido = NULL;
			parent = parent->bro;
			parent->bro = m;
			makeRightChild(parent, s);
			makeRightChild(m, l);
			m->lkido = NULL;
			m->parent = parent->parent;
			m->bro = NULL;

		}
	}

	if (keys(parent) == 1)
	{
		if (m->rno < parent->rno) {

			if (parent == root)
				root = m;

			makeLeftChild(m, s);
			makeRightChild(m, l);
			m->rkido = l;
			l->parent = m;
			parent->lkido = NULL;
			s->bro = NULL;

			m->bro = parent;

			struct node *grandnode = parent->parent;

			m->parent = grandnode;

			if (grandnode)
			{
				if (grandnode->lkido == parent)
				{
					grandnode->lkido = m;
				}

				if (grandnode->rkido == parent)
				{
					grandnode->rkido = m;
				}
			}
		}

		if (m->rno > parent->rno)
		{
			makeRightChild(parent, s);
			m->lkido = NULL;
			parent->bro = m;
			makeRightChild(m, l);
			m->parent = parent->parent;
			m->bro = NULL;
		}

	}
}

struct node *insertt(int data)
{
	struct node *temp = root;

	do
	{

		for (; keys(temp) >= 3; temp = getNextChild(temp, data))
		{
			split(temp);
			temp = temp->parent;
		}

		if (temp)
		{
			if (temp->lkido || temp->rkido)
			{
				temp = getNextChild(temp, data);
			}

			else {

				struct node *node = (struct node*) malloc(sizeof(struct node));
				node->rno = data;
				node->bro = NULL;
				node->lkido = NULL;
				node->rkido = NULL;

				if (data < temp->rno)
				{
					if (temp == root)
						root = node;
					node->bro = temp;

					struct node *grandnode = temp->parent;
					node->parent = temp->parent;

					if (onLeftSide(temp))
						temp->parent->lkido = node;

					else if (onRightSide(temp))
						temp->parent->rkido = node;
				}

				else if (data > temp->rno)
				{
					int key = keys(temp);
					while (key--)
						if (temp->bro && data > temp->bro->rno)
						{
							temp = temp->bro;
						}

					node->bro = temp->bro;
					temp->bro = node;
					node->parent = temp->parent;

				}
				return node;
			}
		}

		else if (root == NULL)
		{
			struct node *node = (struct node*) malloc(sizeof(struct node));
			node->rno = data;
			root = node;
			node->rkido = NULL;
			node->lkido = NULL;
			node->parent = NULL;
			node->bro = NULL;
			return node;
		}

	} while (1);

}

void traverse(struct node *node)
{
	struct node *temp = node;

	while (temp != NULL)
	{
		if (temp->parent)
			printf("%d is parent ", temp->parent->rno);
		if (temp->bro)
			printf("%d is brother ", temp->bro->rno);
		temp = temp->bro;
	}

	temp = node;
	while (temp != NULL)
	{
		if (temp->lkido)
			traverse(temp->lkido);
		if (temp->rkido)
			traverse(temp->rkido);
		temp = temp->bro;
	}
	printf("\n");
}

void whoosechild(struct node *temp)
{
	while (1) {

		printf("\nNode is %d", temp->rno);

		if (temp)
		{
			if (temp->parent)
				printf("\nParent is: %d", temp->parent->rno);
			if (temp->rkido)
				printf("\nRight child is: %d", temp->rkido->rno);
			if (temp->lkido)
				printf("\nLeft child is: %d", temp->lkido->rno);
			if (temp->bro)
			{
				printf("\nBro is: %d", temp->bro->rno);
				if (temp->bro->bro)
					printf("\nBro's bro is: %d", temp->bro->bro->rno);
			}
		}

		printf("\n1.backup to parent\n2.left child\n3.right child\n4.Bro\n5insert\n6.Exit");

		int ch;
		scanf("%d", &ch);
		if (ch == 1)
			temp = temp->parent;
		if (ch == 2)
			temp = temp->lkido;
		if (ch == 3)
			temp = temp->rkido;
		if (ch == 5)
		{
			int temp;
			scanf("%d", &temp);
			insertt(temp);
		}
		if (ch == 4)
			temp = temp->bro;
		if (ch == 6)
			break;
	}
}

void traverset(struct node *node)
{
  struct node *temp = node;
  while(temp!=NULL)
  {
    if(temp->lkido)
      traverset(temp->lkido);
    
    printf("\n%d: \n%s %s %s %s",temp->rno,temp->name,temp->city,temp->add,temp->email);

    if(temp->rkido)
      traverset(temp->rkido);

    temp = temp->bro;
  }
}

struct node *search(int rnum, struct node *node)
{
  struct node *temp = node;


  while(temp!=NULL)
  {
    
    if(temp->rno == rnum)
      return temp;

    if(temp->lkido)
      search(rnum, temp->lkido);

    if(temp->rkido)
      search(rnum, temp->rkido);
    
    temp = temp->bro;
  }
  return NULL;

}

void insertNode()
{
	char num[80];
	int rnum = 0;
	printf("\n");
	scanf("%s", num);
	{
		if (strlen(num) == 6)
		{
			if (num[0] == '0')
			{
				printf("\nInvalid Student Number : First Digit is Zero");
				return;
			}

			for (int i = 0; i < 6; i++)
			{
				if (num[i]<'0' || num[i]>'9')
				{
					printf("\nInvalid Student Number : Number must contain all numbers");
					return;
				}

				else {
					int temp = num[i] - 48;
					rnum *= 10;
					rnum += temp;
			  }
			}

      if(search(rnum,root))
      {
        printf("\n%d Already exists, Try again\n",rnum);
        return;
      }

			struct node *node = insertt(rnum);
      char c[5],name[80], add[80], city[80], email[80];

      fgets(c, 80, stdin);
			fgets(node->name , 80, stdin);
			fgets(node->add  , 80, stdin);
      fgets(node->city , 80, stdin);
      fgets(node->email, 80, stdin);
      printf(".\n");
      
		}

		else {

			printf("\nInvalid Student Number : Length is not equal to 6");
			return;
		}
	}
}

int main(void) {

	while (1)
	{
		char choice[2];
    char name[80];  

    fgets(choice, 2, stdin);
    if(choice[0]=='i')
      insertNode();

    if(choice[0]=='t')
      whoosechild(root);

    if(choice[0]=='l')
      traverset(root);
      
    if(choice[0]=='n')
    {
      int key;
      scanf("%d",&key);
      struct node *status = search(key, root);
      if(status)
      {
        printf("\n%d:\n%s %s %s %s",status->rno, status->name, status->add, status->city, status->email);
      }
      else{
        printf("\n%d not found in record",key);
      }

    }
  }

	getchar();

}
