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
	Cube<T>::Cube(tstring & rawData)
	{
		//_pathCache = unique_ptr<unordered_map<tstring, shared_ptr<vector<shared_ptr<Cubie<T>[]>>>>>;
	}

	//template<typename T>
	//Cube<T>::~Cube()
	//{}
}