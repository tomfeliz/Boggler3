#include "stdafx.h"
#define _ITERATOR_DEBUG_LEVEL 0

#include <fstream>
#include <io.h>
#include <iostream>
#include <memory>
#include <ppl.h>
#include <regex>
#include <thread>
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
	auto start0 = clock();

	// Load the word file.
	auto start1 = clock();
	LoadWordList(wordFileName);
	auto finish1 = clock();
	cout << "LoadWordList: Loaded " << WordList.size() << " words in " 
		<< static_cast<float>(finish1 - start1) / CLOCKS_PER_SEC << " seconds." << endl;

	// Load the cube file.
	auto start2 = clock();
	LoadCubes(cubeFileName);
	auto finish2 = clock();
	cout << "LoadCubes: Loaded " << CubeList.size() << " cubes in " 
		<< static_cast<float>(finish2 - start2) / CLOCKS_PER_SEC << " seconds." << endl;

	vector<thread> workers;
	map<int, pair<int, float>> results;
	auto numBlocks = thread::hardware_concurrency();; // number of threads
	cout << "Using " << numBlocks << " hardware threads." << endl;
	auto blockSize = CubeList.size() / numBlocks;
	mutex cons;

	for (auto block = 0; block < numBlocks; block++)
	{
		auto blockStart = block * blockSize;
		auto blockEnd = min((block + 1) * blockSize - 1, CubeList.size() - 1);

		auto worker = thread([blockStart, blockEnd, &results, &cons]()
		{
			for (auto i = blockStart; i <= blockEnd; i++)
			{
				auto cube = CubeList[i];
				auto count = 0;
				auto start3 = clock();
				for (auto word : WordList)
				{
					if (cube->FindWord(word))
					{
						++count;
					}
				}
				auto finish3 = clock();
				
				results[i] = make_pair(count, static_cast<float>(finish3 - start3) * 1000 / CLOCKS_PER_SEC);
				//cons.lock();
				//cout << "Cube " << (i + 1) << ": " << count << " words ("
				//	<< (static_cast<float>(finish3 - start3) * 1000) / CLOCKS_PER_SEC << " ms)" << endl;
				//cons.unlock();
			}
		});
		workers.push_back(move(worker));
	}

	for (auto& th : workers)
	{
		th.join();
	}

	for (auto i = 0; i < results.size(); i++)
	{
		cout << "Cube " << (i + 1) << ": " << results[i].first << " words ("
			<< results[i].second << " ms)" << endl;
	}

	auto finish0 = clock();
    cout << "Total execution time: " << static_cast<float>(finish0 - start0) / CLOCKS_PER_SEC << " seconds" << endl;

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
	concurrency::parallel_for_each(begin(CubeList), end(CubeList), [](shared_ptr<Cube<TCHAR>> cube)
	{
		cube->PopulatePathCache();
	});

	return true;
}