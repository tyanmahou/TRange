
#include<iostream>
#include<Trange.hpp>
#include<vector>
#include<unordered_map>
#include<Trange/experiment/Query.hpp>

struct A
{
	std::string name;
	int age;
};
int main()
{
	using namespace trange;
	std::vector<A> a{
		{"C#",8},
		{ "Java",8 },
		{ "C++",16}
	};

	std::vector<int> v{ 4,2,3 ,4 ,5,6,7};
	std::vector<std::pair<int, std::string>> map{
		{2,"One"},
		{132,"Two"},
		{ 2,"Three" },
		{ 342,"Four" }
	};
	int ar[] = { 12,23,34 };
	auto aa = map - where([](int i,std::string s){return i>=2; }) ;
	std::cout << aa.size() << std::endl;
	for (auto&& elm : aa)
	{
		elm.first += 100;
		std::cout << elm.first<< std::endl;
	}
	std::cout << "--------------" << std::endl;
	for (auto&& elm : map)
	{
		std::cout << elm.first << std::endl;
		//std::cout << elm.age<<elm.name.c_str() << std::endl;
	}

}

