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
			//void SetNeighbors(std::vector<std::shared_ptr<Cubie<T>>> &neighbors);
			void SetNeighbors(const std::vector<Cubie<T>*> & neighbors);
			//std::shared_ptr<std::vector<std::shared_ptr<Cubie<T>>>> GetNeighbors();
			std::vector<Cubie<T>*> & GetNeighbors();
			const T & GetValue() const;
			void SetValue(const T &value);
			const int GetCubieNumber() const;
			void SetCubieNumber(const int value);
			bool Cubie<T>::operator==(const Cubie<T> &other) const;
			bool Cubie<T>::operator!=(const Cubie<T> &other) const;

		private:
			//std::shared_ptr<std::vector<std::shared_ptr<Cubie<T>>>> _neighbors;
			std::vector<Cubie<T>*> _neighbors;
			T _value;
			int _cubieNumber;
	};
}

