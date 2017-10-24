
#include<iostream>
#include<Trange.hpp>
#include<vector>

int main()
{
	std::vector<int> v{ 1,2,3 };

	for (auto&& elm : v-trange::skipWhile([](const auto& v) {return v <= 2; }))
	{
		std::cout << elm++<<std::endl;
	}
	for (auto&& elm : v)
	{
		std::cout << elm;
	}
}