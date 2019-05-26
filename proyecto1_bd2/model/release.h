#ifndef RELEASE_H
#define RELEASE_H

#include <iostream>
#include <string>

using namespace std;

class Release
{
private:
	long id;
	string name;
	int year;
	string country;
	long a_id;
public:
	Release(){
		id=-1;
	}
	Release(long id,string name, int year, string country, long a_id)
	{
		this->id = id;
		this->name = name;
		this->year = year;
		this->country = country;
		this->a_id = a_id;
	}

	long getId(){return id;}
	string getName(){return name;}
	int getYear(){return year;}
	string getCountry(){return country;}
	long getArtistId(){return a_id;}

	void setId(long newId){id = newId;}
	void setName(string newName){name = newName;}
	void setYear(int newYear){year = newYear;}
	void setCountry(string newCountry){country = newCountry;}
	void setArtistId(long newId){a_id = newId;}

	void print()
	{
		if (id==-1)
		{
			cout << "===================" << endl;
			cout << "No releases found." << endl;
			cout << "===================" << endl;
			return;
		}
		cout << "===================" << endl;
		cout << "     -RELEASE-     "<< endl;
		cout << "===================" << endl;
		cout << "ID: " << id << endl;
		cout << "Name: "<<name << endl;
		cout << "Year: "<<year << endl;
		cout << "Country: "<<country << endl;
		cout << "Artist ID: "<<a_id << endl;
		return;
	}
};

#endif