#include "stdafx.h"
#include "Cube.h"

namespace Boggler
{
	// Must explicitly instantiate template so linker can find it.
	template Cube<TCHAR>;

	//template<typename T>
	//Cube<T>::Cube()
	//{}

	template<typename T>
	Cube<T>::Cube(tstring &rawData)
	{
		_cubies = unique_ptr<array<shared_ptr<Cubie<T>>, NumCubies>>(new array<shared_ptr<Cubie<T>>, NumCubies>);
		//_pathCache = unique_ptr<unordered_map<tstring, shared_ptr<vector<shared_ptr<Cubie<T>[]>>>>>;
	}

	template<typename T>
	void Cube<T>::PopulateCube(tstring & rawData)
    {
        //char characters[] = rawData
        for (int i = 0; i < NumCubies; i++)
        {
            auto cubie = make_shared<Cubie<T>>(new Cubie<T>(rawData[i], i));
            //_cubies[i] = cubie;
        }
    }

	//template<typename T>
	//Cube<T>::~Cube()
	//{}
}