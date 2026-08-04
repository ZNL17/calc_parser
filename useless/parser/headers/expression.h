#ifndef EXPRESSION_H
#define EXPRESSION_H
#define ADD 43
#define SUBSTRACT 45
#define MULTPLY 42
#define DIVIDE 47
#define LEFT 0
#define RIGHT 1
typedef enum {
  Add = ADD,
  Substract = SUBSTRACT,
  Multply = MULTPLY,
  Divide = DIVIDE
} Operator;
typedef struct _Node Node;
struct _Node {
  Node *parent;
  int value;
  Operator op;
  Node *children[2];
};

Node *CNode();
void addExprToTree(Node *currNode, Node **headNode, Node **tailNode);
void addChildNode(Node *parent, int value, int direction);
int operatorCmp(Operator curr, Operator other);
char getOperator(Operator op);
int getPriority(Operator op);
void freeList(Node *node);
void freeNode(Node *node);
int calc(Node *node);
int calcOp(int a, int b, Operator op);
void printNode(Node *node, char *name);
#endif
