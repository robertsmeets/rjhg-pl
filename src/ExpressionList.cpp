#include "ExpressionList.h"


using namespace std;

ExpressionList::ExpressionList()
{
}

void ExpressionList::addExpression(Expression* e)
{
   expressions.push_back(e);
}

void ExpressionList::print(int level)
{
   for (unsigned int i = 0; i < level; i++) { printf("+"); }
   printf("ExpressionList\n" );
   for (auto expression:expressions)
   {
       expression->print(level+1);
   } 
}

vector<Expression*> ExpressionList::getExpressions()
{
   return expressions;
}

