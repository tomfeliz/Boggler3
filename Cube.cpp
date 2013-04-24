#include "stdafx.h"
#include "Cube.h"

using namespace std;

namespace Boggler
{
	// Must explicitly instantiate template so linker can find it.
	template Cube<TCHAR>;

	template<typename T>
	Cube<T>::Cube(tstring & rawData)
	{
		_cubies = unique_ptr<vector<shared_ptr<Cubie<T>>>>(new vector<shared_ptr<Cubie<T>>>());
		//_pathCache = unique_ptr<unordered_map<tstring, shared_ptr<vector<shared_ptr<Cubie<T>[]>>>>>;

		PopulateCube(rawData);
	}

	template<typename T>
	void Cube<T>::PopulateCube(tstring & rawData)
    {
        //char characters[] = rawData
        for (int i = 0; i < NumCubies; i++)
        {
            auto cubie = make_shared<Cubie<T>>(Cubie<T>(rawData.at(i), i));
            _cubies->push_back(cubie);
			//(_cubies*)[i] = cubie;
        }
    }

}