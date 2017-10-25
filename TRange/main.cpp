
#include<iostream>
#include<Trange.hpp>
#include<vector>
#include<Trange/Query.hpp>
int main()
{
	std::vector<int> v{ 1,2,3 ,4};
	std::unordered_map<std::string, std::string> map{
		{"1","One"},
		{"2","Two"}
	};
	int ar[] = { 1,2,3 };
	auto& b = query::algorithm::elmentAt(map, 0);
	std::cout << b.first.c_str() << b.second.c_str();

	for (auto&& elm : ar-trange::takeWhile([](const auto& v) {return v <= 2; }))
	{
		std::cout << elm<<std::endl;
	}
	for (auto&& elm : v)
	{
		std::cout << elm;
	}
}