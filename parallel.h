#pragma once
#include "stdafx.h"
#include <thread>
#include <vector>

template<typename Iterator, typename Function>
void parallel_for_each(const Iterator& first, const Iterator& last, const Function& l)
{
	const auto nthreads = std::thread::hardware_concurrency();
	const size_t portion = (last - first) / nthreads;
	std::vector<std::thread> threads;
	for (Iterator it = first; it < last; it += portion)
	{
		Iterator begin = it;
		Iterator end = it + portion;
		if (end > last)
			end = last;

		threads.push_back(thread([begin, end, l]()
		{
			for (Iterator i = begin; i != end; ++i)
			{
				l(*i);
			}
		}));
	}
	for_each(begin(threads), end(threads), [](std::thread& th) { th.join(); });
}
