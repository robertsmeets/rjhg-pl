#include "SingleMethodCall.h"

SingleMethodCall::SingleMethodCall(string,ExpressionList*){};

void SingleMethodCall::print(int level)
{for (unsigned int i = 0; i < level; i++) {
	 						printf("+");
	 					}
	 printf("SingleMethodCall\n" ); 			}
