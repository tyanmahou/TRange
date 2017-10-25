#pragma once
#include<algorithm>
#include<numeric>
#include<vector>
#include"Reverse.hpp"
namespace trange
{
	template<class Range>
	decltype(auto) max(Range& range)
	{
		return *std::max_element(std::begin(range), std::end(range));
	}
	template<class Range>
	auto max(Range&& range)
	{
		return max(range);
	}

	template<class Range>
	decltype(auto) min(Range&& range)
	{
		return *std::min_element(std::begin(range), std::end(range));
	}
	template<class Range>
	auto min(Range&& range)
	{
		return min(range);
	}

	template<class Range>
	auto sum(const Range& range)
	{
		return std::accumulate(std::begin(range), std::end(range), 0);
	}

	template<class Range>
	auto average(const Range& range)
	{
		return trange::sum(range) / static_cast<double>(std::size(range));
	}

	template<class Range>
	auto count(const Range& range)
	{
		return std::size(range);
	}

	template<class Range, class Type, class Pred>
	auto aggregate(const Range& range, const Type& init, Pred pred)
	{
		Type result = init;
		for (auto&& elm : range)
		{
			result = pred(result, elm);
		}
		return 	result;
	}


	template<class Range>
	auto product(const Range& range)
	{
		return trange::aggregate(range, 1, [](const auto& l, const auto& r) {return l*r; });
	}


	template<class Range>
	decltype(auto) elmentAt(Range& range, std::size_t num)
	{
		auto begin = std::begin(range);
		std::advance(begin, num);
		return *begin;
	}
	template<class Range>
	auto elmentAt(Range&& range, std::size_t num)
	{
		return elmentAt(range, num);
	}
	template<class Range, class Pred>
	decltype(auto) first(Range& range, Pred pred)
	{
		return 	*std::find_if(std::begin(range), std::end(range), pred);
	}
	template<class Range, class Pred>
	auto first(Range&& range, Pred pred)
	{
		return 	first(range);
	}
	template<class Range>
	decltype(auto) first(Range&& range)
	{
		return first(std::forward<Range>(range), _always_true);
	}

	template<class Range, class Pred>
	decltype(auto) last(Range& range, Pred pred)
	{
		auto r = trange::reverse(range);
		return 	*std::find_if(std::begin(r), std::end(r), pred);
	}
	template<class Range, class Pred>
	auto last(Range&& range, Pred pred)
	{
		return 	last(range, pred);
	}
	template<class Range>
	decltype(auto) last(Range&& range)
	{
		return last(std::forward<Range>(range), _always_true);
	}
}