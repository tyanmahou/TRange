#pragma once
#include<tuple>
#include<type_traits>
namespace trange
{
	namespace detail
	{
		template<class T>
		struct WithIndexPair
		{
			T value;
			const std::size_t index;
		};

		//í èÌ
		template<class Func, class Arg>
		auto param_expand_impl(Func func, Arg&& args)->std::invoke_result_t<Func, Arg>
		{
			return func(std::forward<Arg>(args));
		}


		//pair
		template<class Func, class F, class S>
		auto param_expand_impl(Func func,std::pair<F, S> pair)->std::invoke_result_t<Func, F, S>
		{
			return func(pair.first, pair.second);
		}

		//withIndex
		template<class Func, class Type>
		auto param_expand_impl(Func func, WithIndexPair<Type> pair)->std::invoke_result_t<Func, Type, std::size_t>
		{
			return func(pair.value, pair.index);
		}

		//tuple
		template<class Func, class... TupleArgs, class... Args>
		auto param_expand_impl(Func func, std::tuple<TupleArgs...> tuple, Args&&... args)->
			std::enable_if_t<sizeof...(Args) == sizeof...(TupleArgs), std::invoke_result_t<Func, TupleArgs...>>
		{
			return func(args...);
		}
		//tuple
		template<class Func, class... TupleArgs, class... Args>
		auto param_expand_impl(Func func, std::tuple<TupleArgs...> tuple, Args&&... args)->
			std::enable_if_t < sizeof...(Args) < sizeof...(TupleArgs), std::invoke_result_t<Func, TupleArgs...> >
		{
			return param_expand_impl(func, tuple, args..., std::get<sizeof...(Args)>(tuple));
		}



		template<class Func, class Arg>
		decltype(auto) param_expand(Func func, Arg&& arg)
		{
			return param_expand_impl(func, std::forward<Arg>(arg));
		}
	}

}