#include <iostream>
#include <python3.5m/Python.h>
#include <fstream>
#include <string>
#include <vector>
#include <signal.h>

using namespace std;
int flag = 0;
int dataCorr = 0;
int indx;

void readIndex()
{
	ifstream file;
	file.open("index.txt");
	file >> indx;
	file.close();
}

void increaseIndex()
{
	indx++;
	ofstream file;
	file.open("index.txt");
	file << to_string(indx);
	file.close();
}

void readFlag()
{
	ifstream file;
	file.open("flag.txt");
	file >> flag;
}

void my_handler(int s){
	if(dataCorr)
       printf("NAH MANO, GG, LA KAGASTE, CORROMPISTE LA DATA\n");
    exit(1); 
}


int main()
{	
	struct sigaction sigIntHandler;

   sigIntHandler.sa_handler = my_handler;
   sigemptyset(&sigIntHandler.sa_mask);
   sigIntHandler.sa_flags = 0;
   sigaction(SIGINT, &sigIntHandler, NULL);

	while(true)
	{
		readIndex();
		flag = 0;
		string cmd = "python3 script.py " + to_string(indx);
		system(cmd.c_str());
		readFlag();

		
		if(flag == 1)
		{
			cout << "Writing Data: Stopping running program now will most definetly end in data corruption\n";
			ifstream file;
			file.open("output.txt");
			string line;
			vector<string> lines;   // {<r_id>,<r_name>,<r_year>,<r_country>,<a_id>,<a_name>}
			while(getline(file,line))
			{
				lines.push_back(line);
			}	
			file.close();

			dataCorr = 1;
			ofstream outfile;
			outfile.open("releases.dat", ios::out | ios::app | ios::binary);	

			long r_id = stol(lines[0]);
			string r_name = lines[1];
			size_t size_r_name = r_name.size();
			int r_year = stoi(lines[2]);
			string r_country = lines[3];
			size_t size_r_country = r_country.size();
			long a_id = stol(lines[4]);
			string a_name = lines[5];
			size_t size_a_name = a_name.size();

			outfile.write((const char *)&r_id,sizeof(long));
			outfile.write((const char *)&size_r_name,sizeof(size_t));
			outfile.write(&r_name[0],size_r_name);
			outfile.write((const char *)&r_year,sizeof(int));
			outfile.write((const char *)&size_r_country,sizeof(size_t));
			outfile.write(&r_country[0],size_r_country);
			outfile.write((const char *)&a_id,sizeof(long));
			outfile.close();

			outfile.open("artists.dat", ios::out | ios::app | ios::binary);
			outfile.write((const char *)&a_id,sizeof(long));
			outfile.write((const char *)&size_a_name,sizeof(size_t));
			outfile.write(&a_name[0],size_a_name);
			outfile.close();

			increaseIndex();
			dataCorr = 0;
			cout << "Finished Writing Data\n";
		}
		else
		{
			cout << "ERROR RETRIEVING DATA: RELEASE WAS NOT WRITTEN, NO DATA CORRUPTION\n";
			return 0;
		}
		cout << endl;
	}
	
	return 0;
}