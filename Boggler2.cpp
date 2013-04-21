// Boggler2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <string>
#include <iostream>
#include <memory>
#include <regex>
#include <fstream>
#include <vector>
#include <ctime>
#include "Cube.h"

using namespace std;
using namespace Boggler;

typedef basic_string<TCHAR> tstring;
typedef basic_regex<TCHAR> tregex;
typedef basic_ifstream<TCHAR> tifstream;

bool LoadWordList(const tstring &wordFileName);
bool LoadCubes(const tstring &cubeFileName);

auto WordList = make_shared<vector<tstring>>();
//auto CubeList = make_shared<vector<shared_ptr<Cube<TCHAR>>>>(1000);
//auto CubeList = make_shared<vector<shared_ptr<Cube<TCHAR>>>>();
shared_ptr<vector<shared_ptr<Cube<TCHAR>>>> CubeList();

int _tmain(int argc, _TCHAR* argv[])
{
#pragma region Check command line arguments.

	auto abort = false;

	// Make sure the correct arguments are passed into program.
	if (argc != 3)
	{
		cout << "*** Invalid number of command line arguments." << endl;
		abort = true;
	}

	if ((abort == false) && ((_taccess(argv[1], 0) != 0) | (_taccess(argv[2], 0) != 0)))
	{
		cout << "*** One or both files were not found." << endl;
		abort = true;
	}

	if (abort)
	{
		cout << "Correct Usage: Boggler cube-filename word-filename" << endl;
		return 0;
	}

#pragma endregion

	tstring cubeFileName = argv[1];
	tstring wordFileName = argv[2];

	//// Load the word file.
	clock_t start1 = clock();
	LoadWordList(wordFileName);
	clock_t finish1 = clock();
	cout << "LoadWordList: Loaded " << WordList->size() << " words in " 
		<< ((float)(finish1 - start1)) / CLOCKS_PER_SEC << " seconds." << endl;

	// Load the cube file.
	clock_t start2 = clock();
	LoadCubes(cubeFileName);
	clock_t finish2 = clock();
	cout << "LoadCubes: Loaded " << WordList->size() << " cubes in " 
		<< ((float)(finish2 - start2)) / CLOCKS_PER_SEC << " seconds." << endl;

	string temp;
	cout << "Press enter to terminate program..." << endl;
	getline(cin, temp);
	return 0;
}

bool LoadWordList(const tstring &wordFileName)
{
	tregex alphaRegex(_T("^\\s*[a-zA-Z]+\\s*$"));
	tifstream wordFile(wordFileName);
	tstring currentLine;

	while (wordFile.good())
	{
		getline(wordFile, currentLine);

		// Skip empty lines and match on the alpha-numeric regex.
		if (regex_match(currentLine, alphaRegex)) 
		{
			WordList->push_back(currentLine);
		}
	}
	wordFile.close();

	return true;
}

bool LoadCubes(const tstring &cubeFileName)
{
	tifstream cubeFile(cubeFileName);
	tstring currentLine;

	while (cubeFile.good())
	{
		getline(cubeFile, currentLine);
		//auto cube = make_shared<Cube<TCHAR>>(currentLine);
		//CubeList->push_back(cube);
	}
	cubeFile.close();

	// Calculate cube path cache.
	//Parallel::ForEach(cubeList, [&] (void *cube)
	//{
	//	cube::PopulatePathCache();
	//});

	return true;
}