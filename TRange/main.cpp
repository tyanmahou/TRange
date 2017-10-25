
#include<iostream>
#include<Trange.hpp>
#include<vector>
#include<unordered_map>
#include<Trange/Query.hpp>

int main()
{
	using namespace trange;
	std::vector<int> v{ 1,2,3 ,4};
	std::unordered_map<std::string, std::string> map{
		{"1","One"},
		{"2","Two"}
	};
	int ar[] = { 1,2,3 };

	auto p = v - where([](const int& a) {return a % 2 == 0; });
	for (auto&&[elm,i] : p-trange::withIndexConst)
	{
		std::cout << elm<<std::endl;
		//elm++;
	}
	std::cout << "--------------" << std::endl;
	for (auto&& elm : v)
	{
		std::cout << elm<<std::endl;
	}

}

