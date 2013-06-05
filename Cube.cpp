#include "stdafx.h"
#include "Cube.h"

using namespace std;

namespace Boggler
{
	// Must explicitly instantiate template so linker can find it.
	template Cube<TCHAR>;

	template<typename T>
	Cube<T>::Cube(const tstring &rawData)
	{
		_cubies.reserve(NumCubies);

		// First, populate the Cubie array from the raw data.
		PopulateCube(rawData);

        // Second, find adjacent Cubies and populate path cache.
        PopulateNeighbors();
	}

#pragma region FindWord

    // Searches for a word/pattern in the cube data utilizing adjacency relationships. This method must be thread-safe.
	template<typename T>
    bool Cube<T>::FindWord(const tstring &word)
    {
        bool found = false;

        if (word.size() <= PrefixLength)
        {
            // For short words, just check the path cache.
            found = (_pathCache.find(word) != _pathCache.end());
        }
        else // word is greater than PrefixLength
        {
            // Split the word into chunks.
            //vector<tstring> chunks = ChunkString(word, PrefixLength);

            // Get the first string chunk.
            //tstring currChunk = chunks[0];
			auto currChunk = word.substr(0, PrefixLength);

            if (_pathCache.find(currChunk) != _pathCache.end())
            {
				auto paths = _pathCache[currChunk];
                
				//chunks.erase(chunks.begin());
                
				for (auto toPath : paths)
                {
					deque<shared_ptr<Cubie<T>>> s;
					//deque<Cubie<T>*> s;
					//vector<tstring> c(chunks);
                    //if (FindWordRecursive(toPath, c, s))
					if (FindWordRecursive(toPath, word.substr(PrefixLength, word.size() - PrefixLength), s))
                    {
                        found = true;
                        break;
                    }
                }
            }
        }

        return found;
    }

    // Main recursive depth-first search routine.
	template<typename T>
	bool Cube<T>::FindWordRecursive(const vector<shared_ptr<Cubie<T>>> &fromPath, tstring subWord, deque<shared_ptr<Cubie<T>>> &pathStack)
    //bool Cube<T>::FindWordRecursive(const vector<shared_ptr<Cubie<T>>> &fromPath, vector<tstring> &chunks, deque<shared_ptr<Cubie<T>>> &pathStack)
	//bool Cube<T>::FindWordRecursive(const std::vector<Cubie<T>*> &fromPath, std::vector<tstring> &chunks, std::deque<Cubie<T>*> &pathStack)
    {
        bool found = false;
        for (auto cubie : fromPath)
        {
            pathStack.push_front(cubie);
        }

        // Get the first chunk of the remaining string.
        //tstring currChunk = chunks[0];
		auto currChunk = subWord.substr(0, min<int>(PrefixLength, subWord.size()));

		auto pathIter = _pathCache.find(currChunk);
		if (pathIter != _pathCache.end())
		{
            //chunks.erase(chunks.begin());

			for (const auto toPath : pathIter->second)
            {
				// Make sure path does not overlap the path already traversed.
				bool overlap = false;
				for (const auto c1 : pathStack)
				{
					for (const auto c2 : toPath)
					{
						if (*c1 == *c2)
						{
							overlap = true;
							break;
						}
					}

					if (overlap) break;
				}

                // Make sure path is contiguous and does not overlap the path already traversed.
				auto neighbors = fromPath[PrefixLength - 1]->GetNeighbors();
                if (!overlap && (find(neighbors->begin(), neighbors->end(), toPath[0]) != neighbors->end()))
				//if (!overlap && (find(neighbors.begin(), neighbors.end(), toPath[0]) != neighbors.end()))
				{
                    //if (subWord.size() == 0)
					if (subWord.size() <= PrefixLength)
                    {
                        // We're done... The word has been found.
                        found = true;
                        break;
                    }
                    else
                    {
                        // Recurse further...
						//vector<tstring> c(chunks);
                        //if (FindWordRecursive(toPath, c, pathStack))
						if (FindWordRecursive(toPath, subWord.substr(PrefixLength, subWord.size() - PrefixLength), pathStack))
                        {
                            found = true;
                            break;
                        }
                    }
                }
            }
        }

        for (int i = 0; i < PrefixLength; i++)
        {
            pathStack.pop_front();
        }
        return found;
    }

    // Breaks a string up into regular size chunks.
	template<typename T>
    inline vector<tstring> Cube<T>::ChunkString(const tstring &str, int chunkSize)
    {
        vector<tstring> chunks;
        for (unsigned int i = 0; i < str.size(); i += chunkSize)
        {
            chunks.emplace_back(str.substr(i, min<int>(chunkSize, str.size() - i)));
        }
        return chunks;
    }

#pragma endregion

#pragma region CountWords

	template<typename T>
	int Cube<T>::CountWords(vector<tstring> &wordList)
	{
		int count = 0;
		for (auto cubie : _cubies)
		{
			
		}
		return count;
	}

	template<typename T>
	bool Cube<T>::FindPrefix(tstring & prefix)
	{
		return false;
	}

#pragma endregion

	template<typename T>
	void Cube<T>::PopulateCube(const tstring &rawData)
    {
        for (unsigned int i = 0; i < rawData.length(); i++)
        {
            //auto cubie = make_shared<Cubie<T>>(rawData[i], i);
			auto cubie = new Cubie<T>(rawData[i], i);
            _cubies.emplace_back(cubie);
        }
    }


	template<typename T>
	void Cube<T>::PopulateNeighbors()
    {
        for (int c = 0; c < NumCubies; c++)
        {
            auto neighbors = GetCubieNeighbors(c);
            _cubies[c]->SetNeighbors(neighbors);
        }
    }

	template<typename T>
	vector<shared_ptr<Cubie<T>>> Cube<T>::GetCubieNeighbors(int cubieNum)
	//std::vector<Cubie<T>*> Cube<T>::GetCubieNeighbors(int cubieNum)
    {
		vector<shared_ptr<Cubie<T>>> cubieNeighbors;
		//vector<Cubie<T>*> cubieNeighbors;
        int z = cubieNum / (Dimension * Dimension);
        int y = (cubieNum - z * Dimension * Dimension) / Dimension;
        int x = (cubieNum - z * Dimension * Dimension - y * Dimension);
            
        for (int z1 = z - 1; z1 <= z + 1; z1++)
        {
            if (z1 >= 0 && z1 < Dimension)
            {
                for (int y1 = y - 1; y1 <= y + 1; y1++)
                {
                    if (y1 >= 0 && y1 < Dimension)
                    {
                        for (int x1 = x - 1; x1 <= x + 1; x1++)
                        {
                            if (x1 >= 0 && x1 < Dimension)
                            {
                                int c1 = x1 + y1 * Dimension + z1 * Dimension * Dimension;
                                if (_cubies[c1] != _cubies[cubieNum])
                                {
                                    cubieNeighbors.emplace_back(_cubies[c1]);
                                }
                            }
                        }
                    }
                }
            }
        }

        return cubieNeighbors;
    }

	template<typename T>
    void Cube<T>::PopulatePathCache()
    {
        for (int c = 0; c < Dimension * Dimension * Dimension; c++)
        {
            auto cubie = _cubies[c];

            // Add 1-character prefixes
            tstring oneCharPattern;
			oneCharPattern += cubie->GetValue();
			vector<shared_ptr<Cubie<T>>> cubiePath;
			//vector<Cubie<T>*> cubiePath;
			cubiePath.push_back(cubie);
            AddPathCacheEntry(oneCharPattern, cubiePath);

			// Add 2-character prefixes
			auto neighbors = cubie->GetNeighbors();
            for (auto n1 : *neighbors)
			//for (auto n1 : neighbors)
            {
                tstring twoCharPattern = oneCharPattern + n1->GetValue();
				vector<shared_ptr<Cubie<T>>> cubiePath;
				//vector<Cubie<T>*> cubiePath;
				cubiePath.push_back(cubie);
				cubiePath.push_back(n1);
                AddPathCacheEntry(twoCharPattern, cubiePath);
            }
        }
    }

    // Utility function to add new path cache entry.
	template<typename T>
    void Cube<T>::AddPathCacheEntry(const tstring &pattern, vector<shared_ptr<Cubie<T>>> &cubiePath)
	//void Cube<T>::AddPathCacheEntry(const tstring &pattern, std::vector<Cubie<T>*> &cubiePath)
    {
		auto iter = _pathCache.find(pattern);
		if (iter == _pathCache.end())
		{
			vector<vector<shared_ptr<Cubie<T>>>> paths;
			//vector<vector<Cubie<T>*>> paths;
			paths.emplace_back(cubiePath);
			_pathCache.emplace(pattern, paths);
		}
		else
		{
			iter->second.emplace_back(cubiePath);
		}
    }

}