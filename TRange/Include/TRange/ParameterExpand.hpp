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
		template<class Func, class Args, class = void>
		struct ParameterType {
			using normal_type = decltype(std::declval<Func>()(std::declval<Args>()));
		};
		template<class Func, class F, class S>
		struct ParameterType<Func, std::pair<F, S>, std::void_t<std::invoke_result_t<Func, F, S>>>
		{
			using pair_type =  decltype(std::declval<Func>()(std::declval<F>(), std::declval<S>()));
		};
		template<class Func, class Type>
		struct ParameterType < Func, WithIndexPair<Type>,
			std::void_t < std::invoke_result_t < Func, Type, std::size_t >> >
		{
			using pair_type = decltype(std::declval<Func>()(std::declval<Type>(), std::declval<std::size_t>()));
		};
		template<class Func, class ...Args>
		struct ParameterType<Func, std::tuple<Args...>, std::void_t<std::invoke_result_t<Func, Args...>>>
		{
			using tuple_type = decltype(std::declval<Func>()(std::declval<Args>()...));
		};

		template<class Func,class Arg>
		using parameter_t = ParameterType<Func, Arg>;

		template<class Func, class Arg>
		auto normal_expand(Func func, Arg&& args)->std::invoke_result_t<Func, Arg>
		{
			return func(args);
		}

		template<class Func, class F,class S>
		auto pair_expand(Func func,std::pair<F,S>& pair)->std::invoke_result_t<Func, F,S>
		{
			return func(pair.first, pair.second);
		}
		template<class Func, class Type>
		auto pair_expand(Func func, WithIndexPair<Type>& pair)->std::invoke_result_t<Func, Type,std::size_t>
		{
			return func(pair.value, pair.index);
		}

		template<class Func, class... TupleArgs, class... Args>
		auto tuple_expand(Func func, std::tuple<TupleArgs...> tuple, Args&&... args)->
			std::enable_if_t<sizeof...(Args) == sizeof...(TupleArgs), std::invoke_result_t<Func, TupleArgs...>>
		{
			return func(args...);
		}
		template<class Func, class... TupleArgs, class... Args>
		auto tuple_expand(Func func, std::tuple<TupleArgs...> tuple, Args&&... args)->
			std::enable_if_t < sizeof...(Args) < sizeof...(TupleArgs), std::invoke_result_t<Func, TupleArgs...> >
		{
			return tuple_invoke(func, tuple, args..., std::get<sizeof...(Args)>(tuple));
		}


		/*
		
		*/
		template<class Func,class Arg,class =void>
		struct Expand_impl
		{
			static decltype(auto) Func(Func func, Arg& arg)
			{
				return  normal_expand(func, arg);
			}
		};
		template<class Func, class Arg>
		struct Expand_impl<Func,Arg, std::void_t<typename parameter_t<Func, Arg>::pair_type>>
		{
			static decltype(auto) Func(Func func, Arg& arg)
			{
				return pair_expand(func, arg);
			}
		};
		template<class Func, class Arg>
		struct Expand_impl<Func, Arg, std::void_t<typename parameter_t<Func, Arg>::tuple_type>>
		{
			static decltype(auto) Func(Func func, Arg& arg)
			{
				return tuple_expand(func, arg);
			}
		};

		/*

		*/
		template<class Func, class Arg>
		decltype(auto) param_expand(Func func,Arg&& arg)
		{
			return Expand_impl<Func, std::decay_t<Arg>>::Func(func, std::forward<Arg>(arg));
		}
	}

}