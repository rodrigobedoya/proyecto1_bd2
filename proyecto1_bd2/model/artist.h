#ifndef ARTIST_H
#define ARTIST_H

#include <iostream>
#include <string>

using namespace std;

class Artist
{
private:
	long id;
	string name;
public:
	Artist(){
		id = -1;
	}
	Artist(long id, string name)
	{
		this->id = id;
		this->name = name;
	}

	long getId(){return id;}
	string getName(){return name;}

	void setId(long newId){id = newId;}
	void setName(string newName){name = newName;}

	void print()
	{

		if (id==-1)
		{
			cout << "===================" << endl;
			cout << "No artists found." << endl;
			cout << "===================" << endl;
			return;
		}
		cout << "===================" << endl;
		cout << "     -ARTIST-      "<< endl;
		cout << "===================" << endl;
		
		cout << "ID: " << id << endl;
		cout << "Name: "<<name << endl;
		return;
	}
};

#endif