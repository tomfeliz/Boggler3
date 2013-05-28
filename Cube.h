#pragma once
#pragma warning(disable:4503)

//#include "stdafx.h"
#include <string>
#include <unordered_map>
#include <vector>
#include <memory>
#include <array>
#include "Cubie.h"

typedef std::basic_string<TCHAR> tstring;

namespace Boggler
{
	template<typename T>
	class Cube
	{
		public:
			Cube() { }
			Cube(const tstring &rawData);
			~Cube() { }
			void Cube<T>::PopulatePathCache();

		private:
			static const int Dimension = 4;
			static const int NumCubies = Dimension * Dimension * Dimension;
			static const int PrefixLength = 2;
			std::unique_ptr<std::vector<std::shared_ptr<Cubie<T>>>> _cubies;
			std::unique_ptr<std::unordered_map<tstring, std::vector<std::vector<std::shared_ptr<Cubie<T>>>>>> _pathCache;
			void Cube<T>::PopulateCube(const tstring & rawData);
			void Cube<T>::PopulateNeighbors();
			std::vector<std::shared_ptr<Cubie<T>>> Cube<T>::GetCubieNeighbors(int cubieNum);
			void Cube<T>::AddPathCacheEntry(tstring pattern, std::vector<std::shared_ptr<Cubie<T>>> cubiePath);
	};
}
