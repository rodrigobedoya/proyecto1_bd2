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

	//print request parameters
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

	//general purpose error function
	int error()
	{
		cout << "ERROR: Unknown command"<<endl;
		return 1;
	}
	
	//parse query string
	int processQuery(string query)
	{
		val.clear();
		stringstream sstream;
		sstream << query;
		string word;
		
		if (sstream.rdbuf()->in_avail() != 0) //if query is not empty string
		{		
			sstream >> word;
			transform(word.begin(), word.end(), word.begin(), ::tolower);
			if(word == "select")
			{
				operation = "select";
				return processSelect(sstream); //parse as "select" request
			}
			else if(word == "insert")
			{
				operation = "insert"; 
                return processInsert(sstream); //parse as "insert" request
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

	
	//select request parsing
	int processSelect(stringstream &sstream)
	{
		string word;
		if(sstream.rdbuf()->in_avail() != 0)
		{
			sstream >> word;
			if(word == "*") //accept only "*", no specific attr
			{
				sstream >> word;
				transform(word.begin(), word.end(), word.begin(), ::tolower);
				if(word == "from")
				{
					sstream >> word;
					//determine table
					if (word == "releases")
					{
						table = "releases";
						return processSelectConstraint(sstream); //parse specific request constraint 
					}
					else if (word == "artists")
					{
						table="artists";
						return processSelectConstraint(sstream); //parse specific request constraint
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

	//parse specific request constraint
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
							//get two attributes
							return processSelectConstraintBetween(sstream);						}
						else if(word == "between")
						{
							type = "between";
							//get two attributes
							return processSelectConstraintBetween(sstream);
						}
						else
						{
							return error();
						}
						//get one attribute
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

	//get one attribute
	int processSelectConstraintSingle(stringstream &sstream)
	{
		string word;
		sstream >> word;
        val.push_back(word);
        return 0;
	}
	
	//get two attributes
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

	//parse "insert" request
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
		    //determine table
                if (word == "releases")
                {
                    table = "releases";
                    return processInsertData(sstream); //get values
                }
                else if (word == "artists")
                {
                    table="artists";
                    return processInsertData(sstream); //get values
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
	
	//get values
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
