#include "Literal.h"

#include <iostream>

using namespace std;

void Literal::print(int level)
{

	{for (unsigned int i = 0; i < level; i++) {
		printf("+");
	}
	printf("Literal\n" );
	}

}
