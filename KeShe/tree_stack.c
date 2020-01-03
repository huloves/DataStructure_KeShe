#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Item
{
	char ch;
}Item;

typedef struct bitree_node
{
	Item item;
	struct bitree_node *lchild, *rchild;
}bitree_node, *bitree;

typedef struct stack_node
{
	bitree_node *root;
	struct stack_node *next;
}stack_node, *stack;

typedef struct queue_node
{
    bitree_node *root;
    struct queue_node *next;
}queue_node, *queue_;

void push(stack *top, bitree *root);
void pop(stack *top, bitree *root);
int stack_empty(stack *top);
void top(stack *top, bitree *root);

void enter_queue(queue_ *q, bitree *root);
void delete_queue(queue_ *q, bitree *root);
int queue_empty(queue_ *q);

void create_tree(bitree *root);

void visit(Item item);
void pre_order(bitree *root);
void in_order(bitree *root);
void post_order(bitree *root);
void level_order(bitree *root);

char str[256];
int i = 0;

int main()
{
    bitree root = NULL;

	scanf("%s", str);
    create_tree(&root);
    pre_order(&root);
    printf("\n");
    in_order(&root);
    printf("\n");
    post_order(&root);
    printf("\n");
    level_order(&root);
    printf("\n");

	return 0;
}

void push(stack *top, bitree *root)
{
	stack_node *new_node;
	
	new_node = (stack_node*)malloc(sizeof(stack_node));
	if(new_node == NULL) {
		printf("malloc false.\n");
		exit(0);
	}
	
	new_node->root = *root;
	
	new_node->next = *top;
	*top = new_node;
}

void pop(stack *top, bitree *root)
{
	stack_node *temp;
	
	if(*top == NULL) {
		printf("the stack is empty.\n");
	}
	
	temp = *top;
	*top = temp->next;
	*root = temp->root;
	
    free(temp);
}

int stack_empty(stack *top)
{
	if(*top == NULL) {
		return 1;
	}
	else {
		return 0;
	}
}

void top(stack *top, bitree *root)
{
    *root = (*top)->root;
}

void create_tree(bitree *root)
{
	if(str[i] == '#') {
		*root = NULL;
        i++;
	}
	else {
		*root = (bitree_node*)malloc(sizeof(bitree_node));
		(*root)->item.ch = str[i];
		i++;
		create_tree(&(*root)->lchild);
		create_tree(&(*root)->rchild);
	}
}

void enter_queue(queue_ *q, bitree *root)
{
    queue_node *temp;
    if((*q) == NULL) {
        temp = (queue_node*)malloc(sizeof(queue_node));
        temp->next = NULL;
        temp->root = *root;
        (*q) = temp;
    }
    else {
        queue_node *current = *q;
        while(current->next != NULL) {
            current = current->next;
        }
        temp = (queue_node*)malloc(sizeof(queue_node));
        temp->next = NULL;
        temp->root = *root;
        current->next = temp;
    }
}

void delete_queue(queue_ *q, bitree *root)
{
    queue_node *temp = *q;

    *q = temp->next;
    *root = temp->root;

    free(temp);
}

int queue_empty(queue_ *q)
{
    if(*q == NULL) {
        return 1;
    }
    else {
        return 0;
    }
}

void visit(Item item)
{
    printf("%c", item.ch);
}

void pre_order(bitree *root)
{
    stack s = NULL;
    bitree p = *root;

    while(p != NULL || !stack_empty(&s)) {
        while( p != NULL ) {
            visit(p->item);
            push(&s, &p);
            p = p->lchild;
        }
        if(!stack_empty(&s)) {
            pop(&s, &p);
            p = p->rchild;
        }
    }
}

void in_order(bitree *root)
{
    stack s = NULL;
    bitree p = *root;

    while(p != NULL || !stack_empty(&s)) {
        while(p != NULL) {
            push(&s, &p);
            p = p->lchild;
        }
        if(!stack_empty(&s)) {
            pop(&s, &p);
            visit(p->item);
            p = p->rchild;
        }
    }
}

void post_order(bitree *root)
{
    stack s = NULL;
    bitree p = *root, q = NULL;

    while(p != NULL || !stack_empty(&s)) {
        while(p != NULL) {
            push(&s, &p);
            p = p->lchild;
        }
        if(!stack_empty(&s)) {
            top(&s, &p);
            //visit(p->item);
            if((p->rchild == NULL) || (p->rchild == q)) {
                pop(&s, &p);
                visit(p->item);
                q = p;
                p = NULL;
            }
            else {
                p = p->rchild;
            }
        }
    }
}

void level_order(bitree *root)
{
    queue_ q = NULL;
    bitree p;

    enter_queue(&q, root);
    while(!queue_empty(&q)) {
        delete_queue(&q, &p);
        visit(p->item);
        if(p->lchild != NULL) {
            enter_queue(&q, &p->lchild);
        }
        if(p->rchild != NULL) {
            enter_queue(&q, &p->rchild);
        }
    }
}

