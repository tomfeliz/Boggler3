#include "stdafx.h"
#include "Cube.h"

using namespace std;

namespace Boggler
{
	// Must explicitly instantiate template so linker can find it.
	template Cube<TCHAR>;

	template<typename T>
	Cube<T>::Cube(const tstring & rawData)
	{
		_cubies = unique_ptr<vector<shared_ptr<Cubie<T>>>>(new vector<shared_ptr<Cubie<T>>>());
		//_pathCache = unique_ptr<unordered_map<tstring, shared_ptr<vector<shared_ptr<Cubie<T>[]>>>>>;

		PopulateCube(rawData);
	}

	template<typename T>
	void Cube<T>::PopulateCube(const tstring & rawData)
    {
        for (unsigned int i = 0; i < rawData.length(); i++)
        {
            auto cubie = make_shared<Cubie<T>>(rawData.at(i), i);
            _cubies->push_back(cubie);
        }
    }

}