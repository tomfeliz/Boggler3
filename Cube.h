#pragma once

//#include "stdafx.h"
#include <string>
#include <unordered_map>
#include <vector>
#include <memory>
#include <array>
#include "Cubie.h"

using namespace std;

typedef basic_string<TCHAR> tstring;

namespace Boggler
{
	template<typename T>
	class Cube
	{
		public:
			Cube() { }
			Cube(tstring & rawData);
			~Cube() { }

		private:
			static const int Dimension = 4;
			static const int PrefixLength = 2;
			unique_ptr<array<shared_ptr<Cubie<T>>, Dimension * Dimension * Dimension>> _cubies();
			unique_ptr<unordered_map<tstring, shared_ptr<vector<vector<shared_ptr<Cubie<T>>>>>>> _pathCache();
	};
}
