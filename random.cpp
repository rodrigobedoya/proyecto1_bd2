#include <iostream>
#include <string>
#include "model/artist.h"
#include "model/release.h"
#include "file_organization/randomfile.h"
using namespace std;

int main()
{
	Artist a(0,"IO");
	Release b(0,"Rel",1999,"Peru",1);

	RandomFile randomFile;
	randomFile.create();
	randomFile.load();

	randomFile.searchRelease({"3122"},"id","greaterequal");
	randomFile.searchArtist({"1","10"},"id","between");

	return 0;
}