#pragma once

#include <memory>
#include <string>
#include <vector>

namespace Boggler
{
	template<typename T>
	class Cubie
	{
		public:
			Cubie();
			Cubie(T val, int cubieNum);
			~Cubie() {}
			//void AddNeighbors(const shared_ptr<vector<shared_ptr<Cubie<T>>>> neighbors);
			//const shared_ptr<vector<shared_ptr<Cubie<T>>>> Cubie<T>::GetNeighbors() const;
			const T & getValue() const;
			void setValue(const T & value);
			const int getCubieNumber() const;
			void setCubieNumber(const int value);

		private:
			std::unique_ptr<std::vector<std::shared_ptr<Cubie<T>>>> _neighbors;
			T _value;
			int _cubieNumber;
	};
}

