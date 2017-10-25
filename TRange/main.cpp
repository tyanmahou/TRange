
#include<iostream>
#include<Trange.hpp>
#include<vector>
#include<unordered_map>
#include<Trange/Query.hpp>

int main()
{
	using namespace trange;
	std::vector<int> v{ 1,2,3 ,4 };
	std::vector<std::pair<int, std::string>> map{
		{2,"One"},
		{132,"Two"}
	};
	int ar[] = { 1,2,3 };

	auto p = v - where([](const int& a) {return a % 2 == 0; });
	for (auto&elm : map - where([](const auto& a) {return a.first >= 0; }))
	{
		std::cout << elm.first << std::endl;
	}
	std::cout << "--------------" << std::endl;
	for (auto&& elm : v)
	{
		std::cout << elm << std::endl;
	}

}

