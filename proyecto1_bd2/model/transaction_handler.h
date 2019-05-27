#ifndef TRANSACTIONHANDLER_H
#define TRANSACTIONHANDLER_H

#include <vector>
#include "transaction.h"
#include "file_organization/randomfile.h"
using namespace std;

#include <iostream>
#include <thread>        
#include <mutex>



class TransactionHandler
{
	mutex mtx; //condition checking, writing-reading counter manager
	mutex print_mtx;  //for printing without interruption
	vector<Transaction> transactions;
	int writing; //number of transactions doing write instructions
	int reading; //number of transactions doing read instructions
public:
	TransactionHandler(){
		writing = 0;
		reading = 0;
	}

    int size()
    {
        return transactions.size();
    }

	void add(Transaction t)
	{
		transactions.push_back(t);
		return;
	}

    void run_all(RandomFile &randomFile)
	{
		thread t_threads[transactions.size()];
        for(unsigned int i = 0; i < transactions.size();i++)
		{
            t_threads[i] = thread(&TransactionHandler::run_transaction, this,i,ref(randomFile));
		}
        for(unsigned int i = 0; i < transactions.size();i++)
		{
			t_threads[i].join();			
		}
		cout << "transactions_finished"<<endl;
		return;
	}

    void run_transaction(int indx,RandomFile &randomFile)
	{
		for(int i = 0; i < transactions[indx].numInstructions();i++)
		{
            Request r = transactions[indx].getInstruction(i);
			
            if(r.getOperation() == "select")
                read_process(r,randomFile);
            else if(r.getOperation() == "insert")
                write_process(r,randomFile);
			
			print_mtx.lock();
            cout << "transaction "<<indx+1 << " finished instruction "<<i+1<<endl;
			print_mtx.unlock();	
		}

		print_mtx.lock();
        cout << "TRANSACTION "<<indx+1 << " FINISHED"<<endl;
		print_mtx.unlock();
		return;
	}

    void read_process(Request r,RandomFile &randomFile)
	{
		while(true)
		{
			mtx.lock();
			if(writing == 0) //if no transaction is writing, proceed
			{
				reading++;
				mtx.unlock();
				break;	
			}
			mtx.unlock();
		}

        if (r.getTable()=="releases")
        {
            randomFile.searchRelease(r.getVal(),r.getVarName(),r.getType());
        }
        else if (r.getTable() == "artists")
        {
            randomFile.searchArtist(r.getVal(),r.getVarName(),r.getType());
        }

		mtx.lock();
		reading--;
		mtx.unlock();
		return;
	}

    void write_process(Request r,RandomFile &randomFile)
	{
		while(true)
		{
			mtx.lock();
			if(reading == 0 && writing == 0) //if no transaction is doing anything, proceed
			{
				writing++;
				mtx.unlock();
				break;
			}
			mtx.unlock();
		}

        if (r.getTable()=="releases")
        {
            randomFile.insertRelease(r.getVal());
        }
        else if (r.getTable() == "artists")
        {
            randomFile.insertArtist(r.getVal());
        }

		mtx.lock();
		writing--;
		mtx.unlock();
		return;
	}

};

#endif
