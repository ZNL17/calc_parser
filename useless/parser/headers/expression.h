#ifndef EXPRESSION_H
#define EXPRESSION_H
#include "lexer.h"
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
  Token value;
  Node *children[2];
};
typedef union {
  int integer;
  float decimal;
} Numeric;
typedef struct {
  Type type;
  Numeric numeric;
} Number;

Node *CNode();
void addExprToTree(Node *currNode, Node **headNode, Node **tailNode);
void addChildNode(Node *parent, Node *value, int direction);
int operatorCmp(Operator curr, Operator other);
int operatorTokenCmp(Token curr, Token other);
char getOperator(Node node);
int getPriority(Operator op);
Operator whichOperator(Token token);
void freeList(Node *node);
void freeNode(Node *node);
Number calc(Node *node);
Number calcOp(Number a, Number b, Node node);
Number specifyNumber(Token value);
String *fmtNumber(Number number);
Number add(Number a, Number b);
Number substract(Number a, Number b);
Number multiply(Number a, Number b);
Number divide(Number a, Number b);
int equal(Number a, Number b);
void printNumber(Number n);
void printNode(Node *node, char *name);
#endif
