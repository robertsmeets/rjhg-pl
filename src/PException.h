

#ifndef PEXCEPTION_H_
#define PEXCEPTION_H_

#include <string>

using namespace std;

class PException {
	string reason;
public:
	PException(string);
	virtual ~PException();
	string ShowReason();
};

#endif /* PEXCEPTION_H_ */
