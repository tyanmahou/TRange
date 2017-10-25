
#include<iostream>
#include<Trange.hpp>
#include<vector>
#include<unordered_map>
#include<Trange/Query.hpp>

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
	int ar[] = { 1,2,3 };
	for (auto&& elm : std::vector<int>{ 1,2,3 } -select([](const int& a) {return a+114514; }))
	{
		
		std::cout << elm<< std::endl;
	}
	std::cout << "--------------" << std::endl;
	for (auto&& elm : a)
	{
		std::cout << elm.age<<elm.name.c_str() << std::endl;
	}

}

