#pragma once
#pragma warning(disable:4503)

//#include "stdafx.h"
#include <array>
#include <deque>
#include <map>
#include <memory>
#include <string>
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

			//std::vector<std::shared_ptr<Cubie<T>>> _cubies;
			std::vector<Cubie<T>*> _cubies;
			//std::map<tstring, std::vector<std::vector<std::shared_ptr<Cubie<T>>>>> _pathCache;
			std::map<tstring, std::vector<std::vector<Cubie<T>*>>> _pathCache;
			

			//bool Cube<T>::FindWordRecursive(const std::vector<std::shared_ptr<Cubie<T>>> &fromPath, tstring subWord, 
			//	std::deque<std::shared_ptr<Cubie<T>>> &pathStack);
			//bool Cube<T>::FindWordRecursive(const std::vector<Cubie<T>*> &fromPath, tstring subWord, std::deque<Cubie<T>*> &pathStack);
			bool Cube<T>::FindWordRecursive(const std::vector<Cubie<T>*> &fromPath, tstring subWord, std::vector<bool> &cubieMap);
			int Cube<T>::CountWords(std::vector<tstring> &wordList);
			void Cube<T>::PopulateCube(const tstring &rawData);
			bool Cube<T>::FindPrefix(tstring & prefix);
			void Cube<T>::PopulateNeighbors();
			//std::vector<std::shared_ptr<Cubie<T>>> Cube<T>::GetCubieNeighbors(int cubieNum);
			std::vector<Cubie<T>*> Cube<T>::GetCubieNeighbors(int cubieNum);
			//void Cube<T>::AddPathCacheEntry(const tstring &pattern, std::vector<std::shared_ptr<Cubie<T>>> &cubiePath);
			void Cube<T>::AddPathCacheEntry(const tstring &pattern, std::vector<Cubie<T>*> &cubiePath);
	};
}
