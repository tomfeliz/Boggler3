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
		_cubies = unique_ptr<vector<shared_ptr<Cubie<T>>>>(new vector<shared_ptr<Cubie<T>>>());
		_pathCache = unique_ptr<unordered_map<tstring, unique_ptr<vector<unique_ptr<vector<shared_ptr<Cubie<T>>>>>>>>(
			new unordered_map<tstring, unique_ptr<vector<unique_ptr<vector<shared_ptr<Cubie<T>>>>>>>());

		// First, populate the Cubie array from the raw data.
		PopulateCube(rawData);

        // Second, find adjacent Cubies and populate path cache.
        PopulateNeighbors();
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
            auto oneCharPattern = string(cubie->GetValue());
            AddPathCacheEntry(string(oneCharPattern), cubie);

			// Add 2-character prefixes
            for (auto n1 : cubie->GetNeighbors())
            {
                string twoCharPattern = oneCharPattern + n1.GetValue();
                AddPathCacheEntry(twoCharPattern, cubie, n1);
            }
        }
    }

    // Utility function to add new path cache entry.
	template<typename T>
    void Cube<T>::AddPathCacheEntry(string &pattern, Cubie<T> &cubie)
    {
        // Only add a path prefix if the prefix exists in the dictionary prefixes.
        if (Program.DictPrefixes.Contains(pattern))
        {
            vector<Cubie<T>> cubieList;
            if (_pathCache.TryGetValue(pattern, out cubieList))
            {
                if (!cubieList.Contains(cubie))
                {
                    cubieList.Add(cubie);
                }
            }
            else
            {
                cubieList = new List<Cubie<T>> { cubie };
                _pathCache.Add(pattern, cubieList);
            }
        }
    }
}