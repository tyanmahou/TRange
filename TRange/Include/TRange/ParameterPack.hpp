#pragma once
#include<tuple>
#include<type_traits>
#include"WithIndex.hpp"
namespace trange
{
	namespace detail
	{
		template<class Func, class Args, class = void>
		struct ParameterType {
			using normal_type = decltype(std::declval<Func>()(std::declval<Args>()));
		};
		template<class Func, class F, class S>
		struct ParameterType<Func, std::pair<F, S>, std::void_t<std::invoke_result_t<Func, F, S>>>
		{
			using pair_type = decltype(std::declval<Func>()(std::declval<F>(), std::declval<S>()));
		};
		template<class It>
		using Test = typename WithIndexIterator<It>::value_type;
		//template<class Func, class It>
		//struct ParameterType < Func, typename WithIndexIterator<It>,
		//	std::void_t < std::invoke_result_t < Func, decltype(std::declval<typename WithIndexIterator<It>::value_type>().first), std::size_t >> >
		//{
		//	using pair_type = int;// decltype(std::declval<Func>()(std::declval<decltype(std::declval<typename WithIndexIterator<It>::value_type>().first)>(), std::declval<std::size_t>()));
		//};
		//template<class Func, class ...Args>
		//struct ParameterType<Func, std::tuple<Args...>, std::void_t<std::invoke_result_t<Func, Args...>>>
		//{
		//	using tuple_type = decltype(std::declval<Func>()(std::declval<Args>()...));
		//};

		template<class Func,class Arg>
		using parameter_t = ParameterType<Func, Arg>;

		template<class Func, class... TupleArgs, class... Args>
		auto tuple_pack(Func func, std::tuple<TupleArgs...> tuple, Args&&... args)->
			std::enable_if_t<sizeof...(Args) == sizeof...(TupleArgs), std::invoke_result_t<Func, TupleArgs...>>
		{
			return func(args...);
		}
		template<class Func, class... TupleArgs, class... Args>
		auto tuple_pack(Func func, std::tuple<TupleArgs...> tuple, Args&&... args)->
			std::enable_if_t < sizeof...(Args) < sizeof...(TupleArgs), std::invoke_result_t<Func, TupleArgs...> >
		{
			return tuple_pack(func, tuple, args..., std::get<sizeof...(Args)>(tuple));
		}
		template<class Func, class F,class S>
		auto pair_pack(Func func,std::pair<F,S>& pair)->std::invoke_result_t<Func, F,S>
		{
			return func(pair.first, pair.second);
		}
		template<class Func, class It>
		auto pair_pack(Func func,typename WithIndexIterator<It>::value_type& pair)->std::invoke_result_t<Func, decltype(pair.first),std::size_t>
		{
			return func(pair.value, pair.index);
		}
		template<class Func, class Arg>
		auto normal_pack(Func func, Arg&& args)->std::invoke_result_t<Func, Arg>
		{
			return func(args);
		}

		template<class Func,class Arg,class =void>
		struct ParameterPack_impl
		{
			static decltype(auto) Func(Func func, Arg&& arg)
			{
				return normal_pack(func, arg);
			}
		};
		template<class Func, class Arg>
		struct ParameterPack_impl<Func,Arg,std::void_t<typename parameter_t<Func,Arg>::pair_type>>
		{
			static decltype(auto) Func(Func func, Arg&& arg)
			{
				return pair_pack(func, arg);
			}
		};
		template<class Func, class Arg>
		struct ParameterPack_impl<Func, Arg, std::void_t<typename parameter_t<Func, Arg>::tuple_type>>
		{
			static decltype(auto) Func(Func func, Arg&& arg)
			{
				return tuple_pack(func, arg);
			}
		};

		template<class Func, class Arg>
		decltype(auto) parameter_pack(Func func,Arg&& arg)
		{
			return ParameterPack_impl<Func, Arg>::Func(func, arg);
		}
	}

}