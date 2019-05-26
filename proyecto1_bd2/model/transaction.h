#ifndef TRANSACTION_H
#define TRANSACTION_H

#include "request.h"
#include <vector>

using namespace std;

class Transaction
{
private:
	vector<Request> instructions;
public:
    Request getInstruction(int i)
	{
        return (instructions[i]);
	}
	int numInstructions()
	{
		return instructions.size();
	}
	void addRequest(Request r)
	{
		instructions.push_back(r);
	}
};

#endif 
