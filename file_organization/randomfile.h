#ifndef RANDOMFILE_H
#define RANDOMFILE_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "../model/artist.h"
#include "../model/release.h"
#include <map>
#include <vector>
using namespace std;

class RandomFile
{
private:
	string releasesFile;
	string artistsFile;
	map<long,long> releaseIndex;
	map<long,long> artistIndex;

public:
	RandomFile()
	{
		releasesFile = "releases.dat";
		artistsFile = "artists.dat";
	}

	void create()
	{
		ifstream file;
		ofstream randomFile;
		file.open("releases.dat", ios::in |  ios::binary);
		randomFile.open("randomFileReleases.dat", ios::out | ios::binary);
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

				if (r_id != 0)
				{
					randomFile.write((const char *)&r_id,sizeof(long));
					randomFile.write((const char *)&size_r_name,sizeof(size_t));
					randomFile.write(&r_name[0],size_r_name);
					randomFile.write((const char *)&r_year,sizeof(int));
					randomFile.write((const char *)&size_r_country,sizeof(size_t));
					randomFile.write(&r_country[0],size_r_country);
					randomFile.write((const char *)&a_id,sizeof(long));
				}
			}
		}

		file.close();
		randomFile.close();

		file.open("artists.dat", ios::in |  ios::binary);
		randomFile.open("randomFileArtists.dat", ios::out | ios::binary);
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
				
				map<long,long>::iterator it = artistIndex.find(a_id);
				if (a_id != 0 && it == artistIndex.end())
				{
					artistIndex[a_id] = 0;
					randomFile.write((const char *)&a_id,sizeof(long));
					randomFile.write((const char *)&size_a_name,sizeof(size_t));
					randomFile.write(&a_name[0],size_a_name);
				}
			}
		}

		file.close();
		randomFile.close();
	}

	void load()
	{
		releaseIndex.clear();
		artistIndex.clear();
		ifstream file;
		file.open("randomFileReleases.dat", ios::in |  ios::binary);
		long addressR = 0, addressA = 0;
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


				if (r_id != 0)
				{
					releaseIndex[r_id] = addressR;
					addressR += sizeof(long);
					addressR += sizeof(size_t);
					addressR += size_r_name;
					addressR += sizeof(int);
					addressR += sizeof(size_t);
					addressR += size_r_country;
					addressR += sizeof(long);
				}

				
			}
		}

		file.close();

		file.open("randomFileArtists.dat", ios::in | ios::binary);
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

				if (a_id != 0)
				{
					artistIndex[a_id] = addressA;
					addressA += sizeof(long);
					addressA += sizeof(size_t);
					addressA += size_a_name;
				}
			}
		}

		file.close();	
	}

	long getReleasePos(long index)
	{
		return releaseIndex[index];
	}

	long getArtistPos(long index)
	{
		return artistIndex[index];
	}

	Release* searchReleaseIndex(long index)
	{
		map<long,long>::iterator it = releaseIndex.find(index);
		if(it == releaseIndex.end())
		{
			Release* release = new Release;
			release->print();
			cout << endl;
			return release;
		}
		long pos = it->second;
		fstream file;
        file.open("randomFileReleases.dat", ios::in |  ios::binary);
        file.seekg (pos,ios::beg);

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

		Release* release = new Release(r_id,r_name,r_year,r_country,a_id);
		release->print();
		cout << endl;
        return release;
	}
	
	vector<Release*> searchReleaseVarEqual(string val, string varName)
	{
		vector<Release*> answer;
		fstream file;
		file.open("randomFileReleases.dat", ios::in |  ios::binary);
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


				if (r_id != 0)
				{
					if (varName=="name")
					{
						if(val == r_name)
						{
							Release* release = new Release(r_id,r_name,r_year,r_country,a_id);
							release->print();
							cout << endl;
					        answer.push_back(release);
						}
					}
					else if (varName=="year")
					{
						if(stoi(val) == r_year)
						{
							Release* release = new Release(r_id,r_name,r_year,r_country,a_id);
							release->print();
							cout << endl;
					        answer.push_back(release);
						}
					}
					else if (varName=="country")
					{
						if(val == r_country)
						{
							Release* release = new Release(r_id,r_name,r_year,r_country,a_id);
							release->print();
							cout << endl;
					        answer.push_back(release);
						}
					}
					else if (varName=="artistId")
					{
						if(stol(val) == a_id)
						{
							Release* release = new Release(r_id,r_name,r_year,r_country,a_id);
							release->print();
							cout << endl;
					        answer.push_back(release);
						}
					}
					else
					{
						Release* release = new Release;
						release->print();
						cout << "ERROR: No attr named '"<< varName<<"'\n";
						cout << endl;
						return answer;
					}					
				}

				
			}
		}

		file.close();

		if (answer.size() == 0)
		{
			Release* release = new Release;
			release->print();
			cout << endl;
		}
		return answer;

	}




	vector<Release*> searchReleaseVarNotEqual(string val, string varName)
	{
		vector<Release*> answer;
		fstream file;
		file.open("randomFileReleases.dat", ios::in |  ios::binary);
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


				if (r_id != 0)
				{
					if (varName=="id")
					{
						if(stol(val) != r_id)
						{
							Release* release = new Release(r_id,r_name,r_year,r_country,a_id);
							release->print();
							cout << endl;
					        answer.push_back(release);
						}
					}
					else if (varName=="name")
					{
						if(val != r_name)
						{
							Release* release = new Release(r_id,r_name,r_year,r_country,a_id);
							release->print();
							cout << endl;
					        answer.push_back(release);
						}
					}
					else if (varName=="year")
					{
						if(stoi(val) != r_year)
						{
							Release* release = new Release(r_id,r_name,r_year,r_country,a_id);
							release->print();
							cout << endl;
					        answer.push_back(release);
						}
					}
					else if (varName=="country")
					{
						if(val != r_country)
						{
							Release* release = new Release(r_id,r_name,r_year,r_country,a_id);
							release->print();
							cout << endl;
					        answer.push_back(release);
						}
					}
					else if (varName=="artistId")
					{
						if(stol(val) != a_id)
						{
							Release* release = new Release(r_id,r_name,r_year,r_country,a_id);
							release->print();
							cout << endl;
					        answer.push_back(release);
						}
					}
					else
					{
						Release* release = new Release;
						release->print();
						cout << "ERROR: No attr named '"<< varName<<"'\n";
						cout << endl;
						return answer;
					}					
				}

				
			}
		}

		file.close();

		if (answer.size() == 0)
		{
			Release* release = new Release;
			release->print();
			cout << endl;
		}
		return answer;

	}


	vector<Release*> searchReleaseVarSmaller(string val, string varName)
	{
		vector<Release*> answer;
		fstream file;
		file.open("randomFileReleases.dat", ios::in |  ios::binary);
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


				if (r_id != 0)
				{
					if (varName=="id")
					{
						if(stol(val) > r_id)
						{
							Release* release = new Release(r_id,r_name,r_year,r_country,a_id);
							release->print();
							cout << endl;
					        answer.push_back(release);
						}
					}
					else if (varName=="year")
					{
						if(stoi(val) > r_year)
						{
							Release* release = new Release(r_id,r_name,r_year,r_country,a_id);
							release->print();
							cout << endl;
					        answer.push_back(release);
						}
					}
					else if (varName=="artistId")
					{
						if(stol(val) > a_id)
						{
							Release* release = new Release(r_id,r_name,r_year,r_country,a_id);
							release->print();
							cout << endl;
					        answer.push_back(release);
						}
					}
					else
					{
						Release* release = new Release;
						release->print();
						if (varName == "name" || varName == "country")
							cout << "ERROR: Can't do '"<< varName<<"' < <value>\n";
						else
							cout << "ERROR: No attr named '"<< varName<<"'\n";
						cout << endl;
						return answer;
					}					
				}

				
			}
		}

		file.close();

		if (answer.size() == 0)
		{
			Release* release = new Release;
			release->print();
			cout << endl;
		}
		return answer;

	}



	vector<Release*> searchReleaseVarSmallerEqual(string val, string varName)
	{
		vector<Release*> answer;
		fstream file;
		file.open("randomFileReleases.dat", ios::in |  ios::binary);
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


				if (r_id != 0)
				{
					if (varName=="id")
					{
						if(stol(val) >= r_id)
						{
							Release* release = new Release(r_id,r_name,r_year,r_country,a_id);
							release->print();
							cout << endl;
					        answer.push_back(release);
						}
					}
					else if (varName=="year")
					{
						if(stoi(val) >= r_year)
						{
							Release* release = new Release(r_id,r_name,r_year,r_country,a_id);
							release->print();
							cout << endl;
					        answer.push_back(release);
						}
					}
					else if (varName=="artistId")
					{
						if(stol(val) >= a_id)
						{
							Release* release = new Release(r_id,r_name,r_year,r_country,a_id);
							release->print();
							cout << endl;
					        answer.push_back(release);
						}
					}
					else
					{
						Release* release = new Release;
						release->print();
						if (varName == "name" || varName == "country")
							cout << "ERROR: Can't do '"<< varName<<"' < <value>\n";
						else
							cout << "ERROR: No attr named '"<< varName<<"'\n";
						cout << endl;
						return answer;
					}					
				}

				
			}
		}

		file.close();

		if (answer.size() == 0)
		{
			Release* release = new Release;
			release->print();
			cout << endl;
		}
		return answer;

	}



	vector<Release*> searchReleaseVarGreater(string val, string varName)
	{
		vector<Release*> answer;
		fstream file;
		file.open("randomFileReleases.dat", ios::in |  ios::binary);
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


				if (r_id != 0)
				{
					if (varName=="id")
					{
						if(stol(val) < r_id)
						{
							Release* release = new Release(r_id,r_name,r_year,r_country,a_id);
							release->print();
							cout << endl;
					        answer.push_back(release);
						}
					}
					else if (varName=="year")
					{
						if(stoi(val) < r_year)
						{
							Release* release = new Release(r_id,r_name,r_year,r_country,a_id);
							release->print();
							cout << endl;
					        answer.push_back(release);
						}
					}
					else if (varName=="artistId")
					{
						if(stol(val) < a_id)
						{
							Release* release = new Release(r_id,r_name,r_year,r_country,a_id);
							release->print();
							cout << endl;
					        answer.push_back(release);
						}
					}
					else
					{
						Release* release = new Release;
						release->print();
						if (varName == "name" || varName == "country")
							cout << "ERROR: Can't do '"<< varName<<"' > <value>\n";
						else
							cout << "ERROR: No attr named '"<< varName<<"'\n";
						cout << endl;
						return answer;
					}					
				}

				
			}
		}

		file.close();

		if (answer.size() == 0)
		{
			Release* release = new Release;
			release->print();
			cout << endl;
		}
		return answer;

	}



	vector<Release*> searchReleaseVarGreaterEqual(string val, string varName)
	{
		vector<Release*> answer;
		fstream file;
		file.open("randomFileReleases.dat", ios::in |  ios::binary);
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


				if (r_id != 0)
				{
					if (varName=="id")
					{
						if(stol(val) <= r_id)
						{
							Release* release = new Release(r_id,r_name,r_year,r_country,a_id);
							release->print();
							cout << endl;
					        answer.push_back(release);
						}
					}
					else if (varName=="year")
					{
						if(stoi(val) <= r_year)
						{
							Release* release = new Release(r_id,r_name,r_year,r_country,a_id);
							release->print();
							cout << endl;
					        answer.push_back(release);
						}
					}
					else if (varName=="artistId")
					{
						if(stol(val) <= a_id)
						{
							Release* release = new Release(r_id,r_name,r_year,r_country,a_id);
							release->print();
							cout << endl;
					        answer.push_back(release);
						}
					}
					else
					{
						Release* release = new Release;
						release->print();
						if (varName == "name" || varName == "country")
							cout << "ERROR: Can't do '"<< varName<<"' > <value>\n";
						else
							cout << "ERROR: No attr named '"<< varName<<"'\n";
						cout << endl;
						return answer;
					}					
				}

				
			}
		}

		file.close();

		if (answer.size() == 0)
		{
			Release* release = new Release;
			release->print();
			cout << endl;
		}
		return answer;

	}


	vector<Release*> searchReleaseVarBetween(string valMin,string valMax, string varName)
	{
		vector<Release*> answer;
		fstream file;
		file.open("randomFileReleases.dat", ios::in |  ios::binary);
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


				if (r_id != 0)
				{
					if (varName=="id")
					{
						if(stol(valMin) <= r_id  &&  r_id <= stol(valMax))
						{
							Release* release = new Release(r_id,r_name,r_year,r_country,a_id);
							release->print();
							cout << endl;
					        answer.push_back(release);
						}
					}
					else if (varName=="year")
					{
						if(stoi(valMin) <= r_year  && r_year <= stoi(valMax))
						{
							Release* release = new Release(r_id,r_name,r_year,r_country,a_id);
							release->print();
							cout << endl;
					        answer.push_back(release);
						}
					}
					else if (varName=="artistId")
					{
						if(stol(valMin) <= a_id  && a_id <= stol(valMax))
						{
							Release* release = new Release(r_id,r_name,r_year,r_country,a_id);
							release->print();
							cout << endl;
					        answer.push_back(release);
						}
					}
					else
					{
						Release* release = new Release;
						release->print();
						if (varName == "name" || varName == "country")
							cout << "ERROR: Can't do '"<< varName<<"' > <value>\n";
						else
							cout << "ERROR: No attr named '"<< varName<<"'\n";
						cout << endl;
						return answer;
					}					
				}

				
			}
		}

		file.close();

		if (answer.size() == 0)
		{
			Release* release = new Release;
			release->print();
			cout << endl;
		}
		return answer;

	}



	vector<Release*> searchReleaseVarNotBetween(string valMin,string valMax, string varName)
	{
		vector<Release*> answer;
		fstream file;
		file.open("randomFileReleases.dat", ios::in |  ios::binary);
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


				if (r_id != 0)
				{
					if (varName=="id")
					{
						if( r_id < stol(valMin)   ||  stol(valMax) < r_id)
						{
							Release* release = new Release(r_id,r_name,r_year,r_country,a_id);
							release->print();
							cout << endl;
					        answer.push_back(release);
						}
					}
					else if (varName=="year")
					{
						if( r_year < stoi(valMin)   ||  stoi(valMax) < r_year)
						{
							Release* release = new Release(r_id,r_name,r_year,r_country,a_id);
							release->print();
							cout << endl;
					        answer.push_back(release);
						}
					}
					else if (varName=="artistId")
					{
						if( a_id < stol(valMin)   ||  stol(valMax) < a_id)
						{
							Release* release = new Release(r_id,r_name,r_year,r_country,a_id);
							release->print();
							cout << endl;
					        answer.push_back(release);
						}
					}
					else
					{
						Release* release = new Release;
						release->print();
						if (varName == "name" || varName == "country")
							cout << "ERROR: Can't do '"<< varName<<"' > <value>\n";
						else
							cout << "ERROR: No attr named '"<< varName<<"'\n";
						cout << endl;
						return answer;
					}					
				}

				
			}
		}

		file.close();

		if (answer.size() == 0)
		{
			Release* release = new Release;
			release->print();
			cout << endl;
		}
		return answer;

	}



	vector<Release*> searchRelease(vector<string> val,string varName="id", string type="equal")
	{
		if(type == "equal")
		{
			if(varName == "id")
			{
				vector<Release*> answer;
				answer.push_back(searchReleaseIndex(stol(val[0])));
				return answer;
			}

			else		
				return searchReleaseVarEqual(val[0],varName);
		}
		else if(type == "smaller")
		{
			return searchReleaseVarSmaller(val[0],varName);
		}
		else if(type == "smallerequal")
		{
			return searchReleaseVarSmallerEqual(val[0],varName);
		}
		else if(type == "greater")
		{
			return searchReleaseVarGreater(val[0],varName);
		}
		else if(type == "greaterequal")
		{
			return searchReleaseVarGreaterEqual(val[0],varName);
		}
		else if(type == "between")
		{
			return searchReleaseVarBetween(val[0],val[1],varName);
		}
		else if(type == "notequal")
		{
			return searchReleaseVarNotEqual(val[0],varName);
		}
		else if(type == "notbetween")
		{
			return searchReleaseVarNotBetween(val[0],val[1],varName);
		}
		else
		{
			vector<Release*> releases;
			cout << "ERROR:unknown request"<<endl<<endl;
			return releases;
		}
	}

	Artist* searchArtistIndex(long index)
	{
		map<long,long>::iterator it = artistIndex.find(index);
		if(it == artistIndex.end())
		{
			Artist* artist = new Artist;
			artist->print();
			cout << endl;
			return artist;
		}

		long pos = it->second;
		fstream file;
        file.open("randomFileArtists.dat", ios::in |  ios::binary);
        file.seekg (pos,ios::beg);

        long a_id = 0;
		string a_name = "";
		size_t size_a_name = 0;
		
		file.read((char *)&a_id,sizeof(long));
		file.read((char *)&size_a_name,sizeof(size_t));
		a_name.resize(size_a_name);
		file.read(&a_name[0],size_a_name);

		Artist* artist = new Artist(a_id,a_name);
		artist->print();
		cout << endl;
        return artist;
	}

	vector<Artist*> searchArtistVarEqual(string val, string varName)
	{
		vector<Artist*> answer;
		fstream file;
		file.open("randomFileArtists.dat", ios::in | ios::binary);
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

				if (a_id != 0)
				{
					if (varName=="name")
					{
						if(val == a_name)
						{
							Artist* artist = new Artist(a_id,a_name);
							artist->print();
							cout << endl;
					        answer.push_back(artist);
						}
					}
					else
					{
						Artist* artist = new Artist;
						artist->print();
						cout << "ERROR: No attr named '"<< varName<<"'\n";
						cout << endl;
						return answer;
					}
				}
			}
		}

		file.close();

		if (answer.size() == 0)
		{
			Artist* artist = new Artist;
			artist->print();
			cout << endl;
		}
		return answer;
	}



	vector<Artist*> searchArtistVarNotEqual(string val, string varName)
	{
		vector<Artist*> answer;
		fstream file;
		file.open("randomFileArtists.dat", ios::in | ios::binary);
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

				if (a_id != 0)
				{
					if (varName=="id")
					{
						if(stol(val) != a_id)
						{
							Artist* artist = new Artist(a_id,a_name);
							artist->print();
							cout << endl;
					        answer.push_back(artist);
						}
					}
					else if (varName=="name")
					{
						if(val != a_name)
						{
							Artist* artist = new Artist(a_id,a_name);
							artist->print();
							cout << endl;
					        answer.push_back(artist);
						}
					}
					else
					{
						Artist* artist = new Artist;
						artist->print();
						cout << "ERROR: No attr named '"<< varName<<"'\n";
						cout << endl;
						return answer;
					}
				}
			}
		}

		file.close();

		if (answer.size() == 0)
		{
			Artist* artist = new Artist;
			artist->print();
			cout << endl;
		}
		return answer;
	}


	vector<Artist*> searchArtistVarSmaller(string val, string varName)
	{
		vector<Artist*> answer;
		fstream file;
		file.open("randomFileArtists.dat", ios::in | ios::binary);
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

				if (a_id != 0)
				{
					if (varName=="id")
					{
						if(stol(val) > a_id)
						{
							Artist* artist = new Artist(a_id,a_name);
							artist->print();
							cout << endl;
					        answer.push_back(artist);
						}
					}
					else
					{
						Artist* artist = new Artist;
						artist->print();
						if (varName == "name")
							cout << "ERROR: Can't do '"<< varName<<"' > <value>\n";
						else
							cout << "ERROR: No attr named '"<< varName<<"'\n";
						cout << endl;
						return answer;
					}
				}
			}
		}

		file.close();

		if (answer.size() == 0)
		{
			Artist* artist = new Artist;
			artist->print();
			cout << endl;
		}
		return answer;
	}

	vector<Artist*> searchArtistVarSmallerEqual(string val, string varName)
	{
		vector<Artist*> answer;
		fstream file;
		file.open("randomFileArtists.dat", ios::in | ios::binary);
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

				if (a_id != 0)
				{
					if (varName=="id")
					{
						if(stol(val) >= a_id)
						{
							Artist* artist = new Artist(a_id,a_name);
							artist->print();
							cout << endl;
					        answer.push_back(artist);
						}
					}
					else
					{
						Artist* artist = new Artist;
						artist->print();
						if (varName == "name")
							cout << "ERROR: Can't do '"<< varName<<"' > <value>\n";
						else
							cout << "ERROR: No attr named '"<< varName<<"'\n";
						cout << endl;
						return answer;
					}
				}
			}
		}

		file.close();

		if (answer.size() == 0)
		{
			Artist* artist = new Artist;
			artist->print();
			cout << endl;
		}
		return answer;
	}


	vector<Artist*> searchArtistVarGreater(string val, string varName)
	{
		vector<Artist*> answer;
		fstream file;
		file.open("randomFileArtists.dat", ios::in | ios::binary);
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

				if (a_id != 0)
				{
					if (varName=="id")
					{
						if(stol(val) < a_id)
						{
							Artist* artist = new Artist(a_id,a_name);
							artist->print();
							cout << endl;
					        answer.push_back(artist);
						}
					}
					else
					{
						Artist* artist = new Artist;
						artist->print();
						if (varName == "name")
							cout << "ERROR: Can't do '"<< varName<<"' > <value>\n";
						else
							cout << "ERROR: No attr named '"<< varName<<"'\n";
						cout << endl;
						return answer;
					}
				}
			}
		}

		file.close();

		if (answer.size() == 0)
		{
			Artist* artist = new Artist;
			artist->print();
			cout << endl;
		}
		return answer;
	}


	vector<Artist*> searchArtistVarGreaterEqual(string val, string varName)
	{
		vector<Artist*> answer;
		fstream file;
		file.open("randomFileArtists.dat", ios::in | ios::binary);
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

				if (a_id != 0)
				{
					if (varName=="id")
					{
						if(stol(val) <= a_id)
						{
							Artist* artist = new Artist(a_id,a_name);
							artist->print();
							cout << endl;
					        answer.push_back(artist);
						}
					}
					else
					{
						Artist* artist = new Artist;
						artist->print();
						if (varName == "name")
							cout << "ERROR: Can't do '"<< varName<<"' > <value>\n";
						else
							cout << "ERROR: No attr named '"<< varName<<"'\n";
						cout << endl;
						return answer;
					}
				}
			}
		}

		file.close();

		if (answer.size() == 0)
		{
			Artist* artist = new Artist;
			artist->print();
			cout << endl;
		}
		return answer;
	}


	vector<Artist*> searchArtistVarBetween(string valMin,string valMax, string varName)
	{
		vector<Artist*> answer;
		fstream file;
		file.open("randomFileArtists.dat", ios::in | ios::binary);
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

				if (a_id != 0)
				{
					if (varName=="id")
					{
						if(stol(valMin) <= a_id && a_id <= stol(valMax))
						{
							Artist* artist = new Artist(a_id,a_name);
							artist->print();
							cout << endl;
					        answer.push_back(artist);
						}
					}
					else
					{
						Artist* artist = new Artist;
						artist->print();
						if (varName == "name")
							cout << "ERROR: Can't do '"<< varName<<"' > <value>\n";
						else
							cout << "ERROR: No attr named '"<< varName<<"'\n";
						cout << endl;
						return answer;
					}
				}
			}
		}

		file.close();

		if (answer.size() == 0)
		{
			Artist* artist = new Artist;
			artist->print();
			cout << endl;
		}
		return answer;
	}


	vector<Artist*> searchArtistVarNotBetween(string valMin,string valMax, string varName)
	{
		vector<Artist*> answer;
		fstream file;
		file.open("randomFileArtists.dat", ios::in | ios::binary);
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

				if (a_id != 0)
				{
					if (varName=="id")
					{
						if(a_id < stol(valMin) ||  stol(valMax) < a_id)
						{
							Artist* artist = new Artist(a_id,a_name);
							artist->print();
							cout << endl;
					        answer.push_back(artist);
						}
					}
					else
					{
						Artist* artist = new Artist;
						artist->print();
						if (varName == "name")
							cout << "ERROR: Can't do '"<< varName<<"' > <value>\n";
						else
							cout << "ERROR: No attr named '"<< varName<<"'\n";
						cout << endl;
						return answer;
					}
				}
			}
		}

		file.close();

		if (answer.size() == 0)
		{
			Artist* artist = new Artist;
			artist->print();
			cout << endl;
		}
		return answer;
	}

	vector<Artist*> searchArtist(vector<string> val,string varName="id", string type = "equal")
	{
		if (type=="equal")
		{
			if(varName == "id")
			{
				vector<Artist*> answer;
				answer.push_back(searchArtistIndex(stol(val[0])));
				return answer;
			}

			else		
				return searchArtistVarEqual(val[0],varName);
		}
		else if(type == "notequal")
		{
			return searchArtistVarNotEqual(val[0],varName);
		}
		else if(type == "smaller")
		{
			return searchArtistVarSmaller(val[0],varName);
		}
		else if(type == "smallerequal")
		{
			return searchArtistVarSmallerEqual(val[0],varName);
		}
		else if(type == "greater")
		{
			return searchArtistVarGreater(val[0],varName);
		}
		else if(type == "greaterequal")
		{
			return searchArtistVarGreaterEqual(val[0],varName);
		}
		else if(type == "between")
		{
			return searchArtistVarBetween(val[0],val[1],varName);
		}
		else if(type == "notbetween")
		{
			return searchArtistVarNotBetween(val[0],val[1],varName);
		}
		else
		{
			vector<Artist*> artists;
			cout << "ERROR:unknown request"<<endl<<endl;
			return artists;
		}
	}


	void insertRelease(string name,int year, string country, long a_id)
	{
		
	}

};

#endif