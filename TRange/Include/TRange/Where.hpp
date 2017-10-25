#pragma once
#include"IRange.hpp"
#include"ParameterExpand.hpp"
#include<vector>
namespace trange
{


	namespace detail
	{
		template<class Range>
		using value_t = std::conditional_t<
			std::is_reference<decltype(*std::begin(std::declval<Range&>()))>::value,
			std::reference_wrapper<range_value_t<Range>>,
			range_value_t<Range>>;

		template<class Range>
		using ret_range =std::vector<value_t<Range>>;
		template<class Range>
		using where_iterator = typename ret_range <Range>::iterator;

		template<class Range>
		class WhereRange :public IRange<trange_iterator<where_iterator<Range>>>
		{
		public:
			using iterator = where_iterator<Range>;
		private:
			Range m_range;
			ret_range <Range> m_ret;
		public:
			template<class Pred>
			WhereRange(Range&& range,Pred pred) :
				m_range(std::forward<Range>(range))
			{
				m_ret.reserve(std::size(m_range));
				for (auto&&elm :m_range)
				{
					if (param_expand(pred,elm))
					{
						m_ret.emplace_back(elm);
					}
				}
			}

			trange_iterator<iterator> begin()
			{
				return std::begin(m_ret);
			}

			trange_iterator<iterator> end()
			{
				return  std::end(m_ret);
			}
			const_iterator<iterator> begin()const
			{
				return iterator{ std::begin(const_cast<ret_range<Range>&>(m_ret)) };
			}

			const_iterator<iterator> end()const
			{
				return iterator{ std::end(const_cast<ret_range<Range>&>(m_ret)) };
			}

			std::size_t size()const
			{
				return std::size(m_ret);
			}

		};

	}
	constexpr struct _Where_OP
	{
	private:
		template<class Pred>
		struct Parm
		{
			Pred pred;
		};
	public:
		template<class Range, class Pred>
		auto operator ()(Range&& v, Pred pred)const
		{
			return detail::WhereRange<Range>(std::forward<Range>(v), pred);
		}

		template<class Type, class Pred>
		auto operator ()(std::initializer_list<Type> v, Pred pred)const
		{
			return detail::WhereRange<std::initializer_list<Type>>(std::forward<std::initializer_list<Type>>(v), pred);
		}
		template<class Pred>
		Parm<Pred>  operator ()(Pred pred)const
		{
			return Parm<Pred>{ pred };
		}

		template<class Range, class Pred>
		friend auto operator -(Range&& v, Parm<Pred> p)
		{
			return  detail::WhereRange<Range>(std::forward<Range>(v), p.pred);
		}
	}where;

}