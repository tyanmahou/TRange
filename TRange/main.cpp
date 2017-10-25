
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
	for (auto&& elm : clone(v)-skip(1))
	{
		std::cout << ++elm<<std::endl;
	}
	std::cout << "--------------" << std::endl;
	for (auto&& elm : v)
	{
		std::cout << elm;
	}


}

