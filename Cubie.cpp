#include "stdafx.h"
#include "Cubie.h"

using namespace std;

namespace Boggler
{
	// Must explicitly instantiate template so linker can find it.
	template Cubie<TCHAR>;

	template<typename T>
	Cubie<T>::Cubie()
	{
		_neighbors = make_shared<vector<shared_ptr<Cubie<T>>>>();
	}

	template<typename T>
	Cubie<T>::Cubie(T val, int cubieNum) : _value(val), _cubieNumber(cubieNum) 
	{
		Cubie();
	}

	//template<typename T>
	//void Cubie<T>::AddNeighbors(const shared_ptr<vector<shared_ptr<Cubie<T>>>> neighbors)
	//{
	//	_neighbors.AddRange(neighbors);
	//}

	//template<typename T>
	//const shared_ptr<vector<shared_ptr<Cubie<T>>>> Cubie<T>::GetNeighbors() const
	//{
	//	return _neighbors;
	//}

	template<typename T>
	const T & Cubie<T>::getValue() const
	{
		return _value;
	}

	template<typename T>
	void Cubie<T>::setValue(const T &value)
	{
		_value = value;
	}

	template<typename T>
	const int Cubie<T>::getCubieNumber() const
	{
		return _cubieNumber;
	}

	template<typename T>
	void Cubie<T>::setCubieNumber(const int value)
	{
		_cubieNumber = value;
	}

}