#ifndef RANDOMFILE_H
#define RANDOMFILE_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "../model/artist.h"
#include "../model/release.h"
#include "../model/request.h"
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
    long lastPosR;
    long lastPosA;

public:
	RandomFile()
	{
        releasesFile = "../proyecto1_bd2/releases.dat";
        artistsFile = "../proyecto1_bd2/artists.dat";
        lastPosA = 0;
        lastPosR = 0;
	}

	void create()
	{
		ifstream file;
		ofstream randomFile;
        file.open("../proyecto1_bd2/releases.dat", ios::in |  ios::binary);
        randomFile.open("../proyecto1_bd2/randomFileReleases.dat", ios::out | ios::binary);
        long count = 0;
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
                    count++;
				}
			}
		}
        cout << count <<endl;
		file.close();
		randomFile.close();

        file.open("../proyecto1_bd2/artists.dat", ios::in |  ios::binary);
        randomFile.open("../proyecto1_bd2/randomFileArtists.dat", ios::out | ios::binary);
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
        file.open("../proyecto1_bd2/randomFileReleases.dat", ios::in |  ios::binary);
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

        file.open("../proyecto1_bd2/randomFileArtists.dat", ios::in | ios::binary);
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
        lastPosA = addressA;
        lastPosR = addressR;
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
            return NULL;
		}
		long pos = it->second;
		fstream file;
        file.open("../proyecto1_bd2/randomFileReleases.dat", ios::in |  ios::binary);
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
        file.open("../proyecto1_bd2/randomFileReleases.dat", ios::in |  ios::binary);
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
                        if(std::stol(val) == r_id)
                        {
                            Release* release = new Release(r_id,r_name,r_year,r_country,a_id);
                            release->print();
                            cout << endl;
                            answer.push_back(release);
                            return answer;
                        }
                    }
                    else if (varName=="name")
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
                        if(std::stoi(val) == r_year)
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
                        if(std::stol(val) == a_id)
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
        file.open("../proyecto1_bd2/randomFileReleases.dat", ios::in |  ios::binary);
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
                        if(std::stol(val) != r_id)
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
                        if(std::stoi(val) != r_year)
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
                        if(std::stol(val) != a_id)
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
        file.open("../proyecto1_bd2/randomFileReleases.dat", ios::in |  ios::binary);
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
                        if(std::stol(val) > r_id)
						{
							Release* release = new Release(r_id,r_name,r_year,r_country,a_id);
							release->print();
							cout << endl;
					        answer.push_back(release);
						}
					}
					else if (varName=="year")
					{
                        if(std::stoi(val) > r_year)
						{
							Release* release = new Release(r_id,r_name,r_year,r_country,a_id);
							release->print();
							cout << endl;
					        answer.push_back(release);
						}
					}
					else if (varName=="artistId")
					{
                        if(std::stol(val) > a_id)
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
        file.open("../proyecto1_bd2/randomFileReleases.dat", ios::in |  ios::binary);
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
                        if(std::stol(val) >= r_id)
						{
							Release* release = new Release(r_id,r_name,r_year,r_country,a_id);
							release->print();
							cout << endl;
					        answer.push_back(release);
						}
					}
					else if (varName=="year")
					{
                        if(std::stoi(val) >= r_year)
						{
							Release* release = new Release(r_id,r_name,r_year,r_country,a_id);
							release->print();
							cout << endl;
					        answer.push_back(release);
						}
					}
					else if (varName=="artistId")
					{
                        if(std::stol(val) >= a_id)
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
        file.open("../proyecto1_bd2/randomFileReleases.dat", ios::in |  ios::binary);
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
                        if(std::stol(val) < r_id)
						{
							Release* release = new Release(r_id,r_name,r_year,r_country,a_id);
							release->print();
							cout << endl;
					        answer.push_back(release);
						}
					}
					else if (varName=="year")
					{
                        if(std::stoi(val) < r_year)
						{
							Release* release = new Release(r_id,r_name,r_year,r_country,a_id);
							release->print();
							cout << endl;
					        answer.push_back(release);
						}
					}
					else if (varName=="artistId")
					{
                        if(std::stol(val) < a_id)
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
        file.open("../proyecto1_bd2/randomFileReleases.dat", ios::in |  ios::binary);
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
                        if(std::stol(val) <= r_id)
						{
							Release* release = new Release(r_id,r_name,r_year,r_country,a_id);
							release->print();
							cout << endl;
					        answer.push_back(release);
						}
					}
					else if (varName=="year")
					{
                        if(std::stoi(val) <= r_year)
						{
							Release* release = new Release(r_id,r_name,r_year,r_country,a_id);
							release->print();
							cout << endl;
					        answer.push_back(release);
						}
					}
					else if (varName=="artistId")
					{
                        if(std::stol(val) <= a_id)
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
        file.open("../proyecto1_bd2/randomFileReleases.dat", ios::in |  ios::binary);
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
                        if(std::stol(valMin) <= r_id  &&  r_id <= std::stol(valMax))
						{
							Release* release = new Release(r_id,r_name,r_year,r_country,a_id);
							release->print();
							cout << endl;
					        answer.push_back(release);
						}
					}
					else if (varName=="year")
					{
                        if(std::stoi(valMin) <= r_year  && r_year <= std::stoi(valMax))
						{
							Release* release = new Release(r_id,r_name,r_year,r_country,a_id);
							release->print();
							cout << endl;
					        answer.push_back(release);
						}
					}
					else if (varName=="artistId")
					{
                        if(std::stol(valMin) <= a_id  && a_id <= std::stol(valMax))
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
        file.open("../proyecto1_bd2/randomFileReleases.dat", ios::in |  ios::binary);
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
                        if( r_id < std::stol(valMin)   ||  std::stol(valMax) < r_id)
						{
							Release* release = new Release(r_id,r_name,r_year,r_country,a_id);
							release->print();
							cout << endl;
					        answer.push_back(release);
						}
					}
					else if (varName=="year")
					{
                        if( r_year < std::stoi(valMin)   ||  std::stoi(valMax) < r_year)
						{
							Release* release = new Release(r_id,r_name,r_year,r_country,a_id);
							release->print();
							cout << endl;
					        answer.push_back(release);
						}
					}
					else if (varName=="artistId")
					{
                        if( a_id < std::stol(valMin)   ||  std::stol(valMax) < a_id)
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



    vector<Release*> searchRelease(vector<string> val,string varName="id", string type="equal", string indexing="random file")
	{
		if(type == "equal")
		{
			if(varName == "id")
			{
                if(indexing == "random file")
                {
                    vector<Release*> answer;
                    Release* result = searchReleaseIndex(std::stol(val[0]));
                    if (result != NULL)
                        answer.push_back(result);
                    return answer;
                }
                else if(indexing == "no index")
                {
                    return searchReleaseVarEqual(val[0],varName);
                }
                else
                {
                    return searchReleaseVarEqual(val[0],varName);
                }
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
            return NULL;
		}

		long pos = it->second;
		fstream file;
        file.open("../proyecto1_bd2/randomFileArtists.dat", ios::in |  ios::binary);
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
        file.open("../proyecto1_bd2/randomFileArtists.dat", ios::in | ios::binary);
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
                        if(std::stol(val) == a_id)
                        {
                            Artist* artist = new Artist(a_id,a_name);
                            artist->print();
                            cout << endl;
                            answer.push_back(artist);
                            return answer;
                        }
                    }
                    else if (varName=="name")
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
        file.open("../proyecto1_bd2/randomFileArtists.dat", ios::in | ios::binary);
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
                        if(std::stol(val) != a_id)
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
        file.open("../proyecto1_bd2/randomFileArtists.dat", ios::in | ios::binary);
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
                        if(std::stol(val) > a_id)
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
        file.open("../proyecto1_bd2/randomFileArtists.dat", ios::in | ios::binary);
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
                        if(std::stol(val) >= a_id)
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
        file.open("../proyecto1_bd2/randomFileArtists.dat", ios::in | ios::binary);
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
                        if(std::stol(val) < a_id)
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
        file.open("../proyecto1_bd2/randomFileArtists.dat", ios::in | ios::binary);
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
                        if(std::stol(val) <= a_id)
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

        file.open("../proyecto1_bd2/randomFileArtists.dat", ios::in | ios::binary);
		if (file.is_open())
		{
            cout << "AHA" << endl;
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
                        if(std::stol(valMin) <= a_id && a_id <= std::stol(valMax))
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
        file.open("../proyecto1_bd2/randomFileArtists.dat", ios::in | ios::binary);
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
                        if(a_id < std::stol(valMin) ||  std::stol(valMax) < a_id)
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

    vector<Artist*> searchArtist(vector<string> val,string varName="id", string type = "equal", string indexing = "random file")
	{
		if (type=="equal")
		{
			if(varName == "id")
			{
                if (indexing == "random file")
                {
                    vector<Artist*> answer;
                    Artist* result = searchArtistIndex(std::stol(val[0]));
                    if (result != NULL)
                        answer.push_back(result);
                    return answer;
                }
                else if (indexing == "no index")
                {
                    return searchArtistVarEqual(val[0],varName);
                }
                else
                {
                    return searchArtistVarEqual(val[0],varName);
                }
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


    int insertRelease(vector<string> val,string indexing="random file")
    {
        if(val.size()>5)
        {
            cout << "Error:too many arguments"<<endl;
            return 1;
        }
        else if(val.size()<5)
        {
            cout << "Error:not enough arguments"<<endl;
            return 1;
        }

        cout << "Looking for registers with same id"<<endl;
        vector<Release*> coincidences = searchRelease(val,"id","equal",indexing);
        if(coincidences.size() > 0)
        {
            cout << "Error: Register with given id already exists!"<<endl;
            return 1;
        }
        cout << "Inserting new register"<<endl;

        long r_id = stol(val[0]);
        string r_name = val[1];
        size_t size_r_name = r_name.size();
        int r_year = stoi(val[2]);
        string r_country = val[3];
        size_t size_r_country = r_country.size();
        long a_id = stol(val[4]);

        if (r_id > 0)
        {
            releaseIndex[r_id] = lastPosR;
            ofstream randomFile;
            randomFile.open("../proyecto1_bd2/randomFileReleases.dat", ios::out | ios::binary | ios::app);
            randomFile.write((const char *)&r_id,sizeof(long));
            randomFile.write((const char *)&size_r_name,sizeof(size_t));
            randomFile.write(&r_name[0],size_r_name);
            randomFile.write((const char *)&r_year,sizeof(int));
            randomFile.write((const char *)&size_r_country,sizeof(size_t));
            randomFile.write(&r_country[0],size_r_country);
            randomFile.write((const char *)&a_id,sizeof(long));
            randomFile.close();

            lastPosR += sizeof(long);
            lastPosR += sizeof(size_t);
            lastPosR += size_r_name;
            lastPosR += sizeof(int);
            lastPosR += sizeof(size_t);
            lastPosR += size_r_country;
            lastPosR += sizeof(long);
            cout << "Register Inserted"<<endl;
            return 0;
        }
        else
        {
            cout << "id cannot be smaller than 1"<<endl;
            return 1;
        }
    }

    int insertArtist(vector<string> val,string indexing="random file")
    {
        if(val.size()>2)
        {
            cout << "Error:too many arguments"<<endl;
            return 1;
        }
        else if(val.size()<2)
        {
            cout << "Error:not enough arguments"<<endl;
            return 1;
        }

        cout << "Looking for registers with same id"<<endl;
        vector<Artist*> coincidences = searchArtist(val,"id","equal",indexing);
        if(coincidences.size() > 0)
        {
            cout << "Error: Register with given id already exists!"<<endl;
            return 1;
        }
        cout << "Inserting new register"<<endl;

        long a_id = stol(val[0]);
        string a_name = val[1];
        size_t size_a_name = a_name.size();

        if (a_id > 0)
        {
            artistIndex[a_id] = lastPosA;
            ofstream randomFile;
            randomFile.open("../proyecto1_bd2/randomFileArtists.dat", ios::out | ios::binary | ios::app);

            randomFile.write((const char *)&a_id,sizeof(long));
            randomFile.write((const char *)&size_a_name,sizeof(size_t));
            randomFile.write(&a_name[0],size_a_name);


            randomFile.close();
            lastPosA += sizeof(long);
            lastPosA += sizeof(size_t);
            lastPosA += size_a_name;
            cout << "Register Inserted"<<endl;
            return 0;
        }
        else
        {
            cout << "id cannot be smaller than 1"<<endl;
            return 1;
        }
    }

};

#endif
