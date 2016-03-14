#include "Statements.h"

void Statements::addStatement(Expression* s) {
   if (s!=NULL)
   {
      s->setTopLevel();
      statements.push_back(s);
   }
}

void Statements::print(int level) {
   for (int i = 0; i < level; i++) {
      printf("+");
   }
   printf("Statements\n" );
   for (Expression* a_statement:statements)
   {
      a_statement->print(level+1);
   }
}

vector<Expression*> Statements::getStatements()
{
   return statements;
}
