#ifndef COMPOSITEMETHODCALL_H_
#define COMPOSITEMETHODCALL_H_ 

#include "Statement.h"
#include "Expression.h"
#include "SingleMethodCall.h"

     class CompositeMethodCall : public Statement,public Expression {
public:
   void addSingleMethodCall(SingleMethodCall*);

};
#endif
