#pragma once
#pragma warning(disable:4503)

//#include "stdafx.h"
#include <array>
#include <deque>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
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
			bool Cube<T>::FindWord(const tstring &word);
			void Cube<T>::PopulatePathCache();

		private:
			static const int Dimension = 4;
			static const int NumCubies = Dimension * Dimension * Dimension;
			static const int PrefixLength = 2;

			std::vector<std::shared_ptr<Cubie<T>>> _cubies;
			std::unordered_map<tstring, std::vector<std::vector<std::shared_ptr<Cubie<T>>>>> _pathCache;
			
			bool Cube<T>::FindWordRecursive(const std::vector<std::shared_ptr<Cubie<T>>> &fromPath, std::vector<tstring> &chunks, 
				std::deque<std::shared_ptr<Cubie<T>>> &pathStack);
			static std::vector<tstring> Cube<T>::ChunkString(const tstring &str, int chunkSize);
			void Cube<T>::PopulateCube(const tstring &rawData);
			void Cube<T>::PopulateNeighbors();
			std::vector<std::shared_ptr<Cubie<T>>> Cube<T>::GetCubieNeighbors(int cubieNum);
			void Cube<T>::AddPathCacheEntry(const tstring &pattern, std::vector<std::shared_ptr<Cubie<T>>> &cubiePath);
	};
}
