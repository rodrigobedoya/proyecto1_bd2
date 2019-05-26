#ifndef REQUEST_H
#define REQUEST_H
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <iostream>
using namespace std;

class Request
{
private:

	string operation;
	string table;
	string type;
	string varName;
	vector<string> val;

public:
	Request(){}

	string getOperation(){return operation;}
	string getTable(){return table;}
	string getType(){return type;}
	string getVarName(){return varName;}
	vector<string> getVal(){return val;}

	string getVal(int i){return val[i];}

	void print()
	{
		cout << "operation: "<<operation<<endl;
		cout << "table: " << table << endl;
		cout << "type: " << type << endl;
		cout << "var_name: " << varName << endl;
		cout << "val: ";
        for(unsigned int i = 0; i < val.size();i++)
		{
			cout << val[i] <<  " "; 
		}
		cout << endl;
	}

	int error()
	{
		cout << "ERROR: Unknown command"<<endl;
		return 1;
	}
	int processQuery(string query)
	{
		val.clear();
		stringstream sstream;
		sstream << query;
		string word;
		
		if (sstream.rdbuf()->in_avail() != 0)
		{		
			sstream >> word;
			transform(word.begin(), word.end(), word.begin(), ::tolower);
			if(word == "select")
			{
				operation = "select";
				return processSelect(sstream);
			}
			else if(word == "insert")
			{
				operation = "insert";
                return processInsert(sstream);
			}
			else
			{
				return error();
			}
			return 0;
		}
		else
		{
			cout << "ERROR: no query"<<endl;
			return 1;
		}
	}

	int processSelect(stringstream &sstream)
	{
		string word;
		if(sstream.rdbuf()->in_avail() != 0)
		{
			sstream >> word;
			if(word == "*")
			{
				sstream >> word;
				transform(word.begin(), word.end(), word.begin(), ::tolower);
				if(word == "from")
				{
					sstream >> word;
					if (word == "releases")
					{
						table = "releases";
						return processSelectConstraint(sstream);
					}
					else if (word == "artists")
					{
						table="artists";
						return processSelectConstraint(sstream);
					}
					else
						return error();
				}
				else
					return error();
			}
			else
				return error();
		}
		else
			return error();
		
	}

	int processSelectConstraint(stringstream &sstream)
	{
		string word;
		if(sstream.rdbuf()->in_avail() != 0)
		{
			sstream >> word;
			transform(word.begin(), word.end(), word.begin(), ::tolower);
			if(word == "where")
			{
				if(sstream.rdbuf()->in_avail() != 0)
				{
					sstream >> word;
					varName = word;
					if(sstream.rdbuf()->in_avail() != 0)
					{
						sstream >> word;
						transform(word.begin(), word.end(), word.begin(), ::tolower);
                        if(word == "==" || word == "=")
							type = "equal";
						
						else if(word == "!=")
							type = "notequal";
						
						else if(word == ">")
							type = "greater";	
						
						else if(word == ">=")
							type = "greaterequal";
							
						
						else if(word == "<")
							type = "smaller";
							
						else if(word == "<=")
							type = "smallerequal";
							
						else if (word == "not")
						{
							sstream >> word;
							if (word == "between")
							{
								type = "notbetween";
							}
							return processSelectConstraintBetween(sstream);
						}
						else if(word == "between")
						{
							type = "between";
							return processSelectConstraintBetween(sstream);
						}
						else
						{
							return error();
						}
						return processSelectConstraintSingle(sstream);
					}					
					else
						return error();
				}
				else
					return error();		
			}
			else
				return error();
			
		}
		else
		{
			return 1;
		}
	}

	int processSelectConstraintSingle(stringstream &sstream)
	{
		string word;
		sstream >> word;
        val.push_back(word);
        return 0;
	}

	int processSelectConstraintBetween(stringstream &sstream)
	{
		string word;
		sstream >> word;
		val.push_back(word);
		sstream >> word; // "AND"
		sstream >> word;
		val.push_back(word);
        return 0;
	}

    int processInsert(stringstream &sstream)
    {
        string word;
        if(sstream.rdbuf()->in_avail() != 0)
        {
            sstream >> word;
            transform(word.begin(), word.end(), word.begin(), ::tolower);
            if(word == "into")
            {
                sstream >> word;
                if (word == "releases")
                {
                    table = "releases";
                    return processInsertData(sstream);
                }
                else if (word == "artists")
                {
                    table="artists";
                    return processInsertData(sstream);
                }
                else
                    return error();
            }
            else
                return error();
        }
        else
            return error();

    }

    int processInsertData(stringstream &sstream)
    {
        string word;
        sstream >> word;
        transform(word.begin(), word.end(), word.begin(), ::tolower);
        if(word == "values")
        {
            string data;
            while(sstream>>data)
            {
                val.push_back(data);
            }
            return 0;
        }
        else
            return error();
    }
};

#endif
