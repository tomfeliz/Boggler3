#define _ITERATOR_DEBUG_LEVEL 0

#include "stdafx.h"
#include <atomic>
#include <ctime>
#include <fstream>
#include <io.h>
#include <iostream>
#include <memory>
#include <ppl.h>
#include <regex>
#include <set>
#include <string>
#include <vector>

#include "Cube.h"

using namespace std;
using namespace Boggler;

typedef basic_string<TCHAR> tstring;
typedef basic_regex<TCHAR> tregex;
typedef basic_ifstream<TCHAR> tifstream;

bool LoadWordList(const tstring &wordFileName);
bool LoadCubes(const tstring &cubeFileName);

vector<tstring> WordList;
vector<shared_ptr<Cube<TCHAR>>> CubeList;

int _tmain(int argc, _TCHAR* argv[])
{
	// Preallocate cubes and words vectors.
	WordList.reserve(25000);
	CubeList.reserve(1000);

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

	// Set global start clock
	clock_t start0 = clock();

	// Load the word file.
	clock_t start1 = clock();
	LoadWordList(wordFileName);
	clock_t finish1 = clock();
	cout << "LoadWordList: Loaded " << WordList.size() << " words in " 
		<< ((float)(finish1 - start1)) / CLOCKS_PER_SEC << " seconds." << endl;

	// Load the cube file.
	clock_t start2 = clock();
	LoadCubes(cubeFileName);
	clock_t finish2 = clock();
	cout << "LoadCubes: Loaded " << CubeList.size() << " cubes in " 
		<< ((float)(finish2 - start2)) / CLOCKS_PER_SEC << " seconds." << endl;

	for (unsigned int i = 0; i < CubeList.size(); i++)
    {
        clock_t start3 = clock();
        atomic<int> wordCount = 0;
		auto cube = CubeList[i];

		concurrency::parallel_for_each(begin(WordList), end(WordList), [&](tstring word) 
		{
			if (cube->FindWord(word))
            {
                wordCount++;
            }
		});

		clock_t finish3 = clock();

        cout << "Cube " << (i + 1) << ": " << wordCount << " words (" 
			<< ((float)(finish3 - start3) * 1000) / CLOCKS_PER_SEC << " ms)" << endl;
    }

	clock_t finish0 = clock();

    cout << "Total execution time: " << ((float)(finish0 - start0)) / CLOCKS_PER_SEC << " seconds" << endl;

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
			WordList.emplace_back(currentLine);
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
		if (currentLine.length() > 0)
		{
			auto cube = make_shared<Cube<TCHAR>>(currentLine);
			CubeList.emplace_back(cube);
		}
	}
	cubeFile.close();

	// Calculate cube path cache.
	for(auto cube : CubeList)
	{
		cube->PopulatePathCache();
	}

	return true;
}