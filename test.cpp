#include <fstream>
#include <string>
#include <iostream>
using namespace std;

long int countR = 0;
long int countA = 0;


int main()
{
	ifstream file;
	file.open("ReleasesTotal.dat", ios::in | ios::binary);
	
	if (file.is_open())
	{
		while(!file.eof())
		{			
			long r_id = 0;
			string r_name = "";
			size_t size_r_name = 0;
			int r_year = 0;
			string r_country = "";
			size_t size_r_country = 0;
			long a_id = 0;
			


			file.read((char *)&r_id,sizeof(long));
			file.read((char *)&size_r_name,sizeof(size_t));
			r_name.resize(size_r_name);
			file.read(&r_name[0],size_r_name);
			file.read((char *)&r_year,sizeof(int));
			file.read((char *)&size_r_country,sizeof(size_t));
			r_country.resize(size_r_country);
			file.read(&r_country[0],size_r_country);
			file.read((char *)&a_id,sizeof(long));
			countR++;

			cout << r_id << " " << r_name<<" " <<r_year << " " << r_country<<" "<< a_id << endl;
		}
	}

	file.close();

	file.open("ArtistTotal.dat", ios::in | ios::binary);
	if (file.is_open())
	{
		while(!file.eof())
		{			
			
			long a_id = 0;
			string a_name = "";
			size_t size_a_name = 0;
			
			file.read((char *)&a_id,sizeof(long));
			file.read((char *)&size_a_name,sizeof(size_t));
			a_name.resize(size_a_name);
			file.read(&a_name[0],size_a_name);
			countA++;
			cout << a_id << " " << a_name << endl;
		}
	}

	file.close();

	cout << countR << endl << countA << endl;
	return 0;
}