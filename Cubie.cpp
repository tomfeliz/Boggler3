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
		//_neighbors = make_shared<vector<shared_ptr<Cubie<T>>>>();
		//_neighbors = new vector<Cubie<T>*>();
	}

	template<typename T>
	Cubie<T>::Cubie(T val, int cubieNum) : _value(val), _cubieNumber(cubieNum) 
	{
		Cubie();
	}

	template<typename T>
	//void Cubie<T>::SetNeighbors(vector<shared_ptr<Cubie<T>>> neighbors)
	void Cubie<T>::SetNeighbors(vector<Cubie<T>*> & neighbors)
	{
		//_neighbors = make_shared<vector<shared_ptr<Cubie<T>>>>(neighbors);
		_neighbors = neighbors;
	}

	template<typename T>
	//shared_ptr<vector<shared_ptr<Cubie<T>>>> Cubie<T>::GetNeighbors()
	vector<Cubie<T>*> & Cubie<T>::GetNeighbors()
	{
		return _neighbors;
	}

	template<typename T>
	const T & Cubie<T>::GetValue() const
	{
		return _value;
	}

	template<typename T>
	void Cubie<T>::SetValue(const T &value)
	{
		_value = value;
	}

	template<typename T>
	const int Cubie<T>::GetCubieNumber() const
	{
		return _cubieNumber;
	}

	template<typename T>
	void Cubie<T>::SetCubieNumber(const int value)
	{
		_cubieNumber = value;
	}

	template<typename T>
	bool Cubie<T>::operator==(const Cubie<T> &other) const 
	{
		  return (_cubieNumber ==  other.GetCubieNumber());
	}

	template<typename T>
	bool Cubie<T>::operator!=(const Cubie<T> &other) const 
	{
		return !(*this == other);
	}


}