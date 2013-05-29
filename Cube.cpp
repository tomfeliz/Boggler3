#include "stdafx.h"
#include "Cube.h"

using namespace std;

namespace Boggler
{
	// Must explicitly instantiate template so linker can find it.
	template Cube<TCHAR>;

	template<typename T>
	Cube<T>::Cube(const tstring rawData)
	{
		_cubies = unique_ptr<vector<shared_ptr<Cubie<T>>>>(new vector<shared_ptr<Cubie<T>>>());
		_pathCache = unique_ptr<unordered_map<tstring, vector<vector<shared_ptr<Cubie<T>>>>>>(new unordered_map<tstring, vector<vector<shared_ptr<Cubie<T>>>>>());

		// First, populate the Cubie array from the raw data.
		PopulateCube(rawData);

        // Second, find adjacent Cubies and populate path cache.
        PopulateNeighbors();
	}

    // Searches for a word/pattern in the cube data utilizing adjacency relationships. This method must be thread-safe.
	template<typename T>
    bool Cube<T>::FindWord(tstring word)
    {
        bool found = false;

        if (word.size() <= PrefixLength)
        {
            // For short words, just check the path cache.
            found = (_pathCache->find(word) != _pathCache->end()) ? true : false;
        }
        else // word is greater than PrefixLength
        {
            // Split the word into chunks.
            vector<tstring> chunks = ChunkString(word, PrefixLength);

            // Get the first string chunk.
            tstring currChunk = chunks[0];

            vector<vector<shared_ptr<Cubie<T>>>> paths;
            if (_pathCache.TryGetValue(currChunk, paths))
            {
                chunks.erase(chunks.begin());
                for (auto toPath : paths)
                {
					stack<shared_ptr<Cubie<T>>> s;
					vector<tstring> c(chunks);
                    if (FindWordRecursive(toPath, c, s))
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
    bool Cube<T>::FindWordRecursive(vector<shared_ptr<Cubie<T>>> fromPath, vector<tstring> chunks, stack<shared_ptr<Cubie<T>>> pathStack)
    {
        bool found = false;
        for (auto cubie : fromPath)
        {
            pathStack.push(cubie);
        }

        // Get the first chunk of the remaining string.
        tstring currChunk = chunks[0];

        vector<vector<shared_ptr<Cubie<T>>>> paths;
		auto iter = _pathCache->find(currChunk);
        if (_pathCache.TryGetValue(currChunk, paths))
        {
            chunks.erase(chunks.begin());
            for (auto toPath : paths)
            {
                // Make sure path is contiguous and does not overlap the path already traversed.
                if (fromPath[PrefixLength - 1].Neighbors.Contains(toPath[0]) && !pathStack.Intersect(toPath).Any())
                {
                    if (chunks.size() == 0)
                    {
                        // We're done... The word has been found.
                        found = true;
                        break;
                    }
                    else
                    {
                        // Recurse further...
						vector<tstring> c(chunks);
                        if (FindWordRecursive(toPath, c, pathStack))
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
            pathStack.pop();
        }
        return found;
    }

    // Breaks a string up into regular size chunks.
	template<typename T>
    vector<tstring> Cube<T>::ChunkString(tstring str, int chunkSize)
    {
        vector<tstring> chunks;
        for (unsigned int i = 0; i < str.size(); i += chunkSize)
        {
            chunks.push_back(str.substr(i, min<int>(chunkSize, str.size() - i)));
        }
        return chunks;
    }

	template<typename T>
	void Cube<T>::PopulateCube(const tstring &rawData)
    {
        for (unsigned int i = 0; i < rawData.length(); i++)
        {
            auto cubie = make_shared<Cubie<T>>(rawData[i], i);
            _cubies->push_back(cubie);
        }
    }


	template<typename T>
	void Cube<T>::PopulateNeighbors()
    {
        for (int c = 0; c < Dimension * Dimension * Dimension; c++)
        {
            auto neighbors = GetCubieNeighbors(c);
            _cubies->at(c)->SetNeighbors(neighbors);
        }
    }

	template<typename T>
	vector<shared_ptr<Cubie<T>>> Cube<T>::GetCubieNeighbors(int cubieNum)
    {
		vector<shared_ptr<Cubie<T>>> cubieNeighbors;
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
                                if (_cubies->at(c1)->GetValue() != _cubies->at(cubieNum)->GetValue())
                                {
                                    cubieNeighbors.push_back(_cubies->at(c1));
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
            auto cubie = _cubies->at(c);

            // Add 1-character prefixes
            tstring oneCharPattern;
			oneCharPattern += cubie->GetValue();
			vector<shared_ptr<Cubie<T>>> cubiePath;
			cubiePath.push_back(cubie);
            AddPathCacheEntry(oneCharPattern, cubiePath);

			// Add 2-character prefixes
			auto neighbors = cubie->GetNeighbors();
            for (auto n1 : *neighbors)
            {
                tstring twoCharPattern = oneCharPattern + n1->GetValue();
				vector<shared_ptr<Cubie<T>>> cubiePath;
				cubiePath.push_back(cubie);
				cubiePath.push_back(n1);
                AddPathCacheEntry(twoCharPattern, cubiePath);
            }
        }
    }

    // Utility function to add new path cache entry.
	template<typename T>
    void Cube<T>::AddPathCacheEntry(tstring pattern, vector<shared_ptr<Cubie<T>>> cubiePath)
    {
		auto iter = _pathCache->find(pattern);
		if (iter == _pathCache->end())
		{
			vector<vector<shared_ptr<Cubie<T>>>> paths;
			paths.push_back(cubiePath);
			_pathCache->emplace(pattern, paths);
		}
		else
		{
			iter->second.push_back(cubiePath);
		}
    }

}