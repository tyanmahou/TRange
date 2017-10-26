
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

	std::vector<int> v{ 1,2,3 ,4 ,5,6,7};
	std::vector<std::pair<int, std::string>> map{
		{2,"One"},
		{132,"Two"}
	};
	int ar[] = { 12,23,34 };
	for (auto&& elm : zip(v, ar, [](auto& x) {x.first+=10;return x;}))
	{
		
		std::cout << elm.second<< std::endl;
	}
	std::cout << "--------------" << std::endl;
	for (auto&& elm : ar)
	{
		std::cout << elm << std::endl;
		//std::cout << elm.age<<elm.name.c_str() << std::endl;
	}

}

