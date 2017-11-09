#pragma once
#include"Util.hpp"

#include"ParameterExpand.hpp"

namespace trange
{
	namespace detail
	{
		template<class Range, class Pred>
		class SelectManyRange
		{
		public:
			using value_type = range_value_t<decltype(param_expand(std::declval<Pred>(), std::declval<range_value_t<Range>>()))>;

			using iterator = typename std::vector<value_type>::iterator;
			using const_iterator = typename std::vector<value_type>::const_iterator;
		private:
			std::vector<value_type> m_ret;
		public:
			SelectManyRange(Range&& range, Pred pred) 
			{
				
				for (auto&& elm : range)
				{
					for (auto&& each : param_expand(pred,elm))
					{
						m_ret.emplace_back(each);
					}
				}
			}
			auto begin()
			{
				return std::begin(m_ret);
			}
			auto  end()
			{
				return  std::end(m_ret);
			}
			auto  begin()const
			{
				return  std::begin(m_ret);
			}
			auto  end()const
			{
				return std::end(m_ret);
			}

			std::size_t size()const
			{
				return std::size(m_ret);
			}
		};
	}

	constexpr struct _SelectMany_OP
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
			return detail::SelectManyRange<Range, Pred>(std::forward<Range>(v), pred);
		}

		template<class Type, class Pred>
		auto operator ()(std::initializer_list<Type> v, Pred pred)const
		{
			return detail::SelectManyRange<std::initializer_list<Type>, Pred>(std::forward<std::initializer_list<Type>>(v), pred);
		}
		template<class Pred>
		Parm<Pred>  operator ()(Pred pred)const
		{
			return Parm<Pred>{ pred };
		}

		template<class Range, class Pred>
		friend auto operator -(Range&& v, Parm<Pred> p)
		{
			return  detail::SelectManyRange<Range, Pred>(std::forward<Range>(v), p.pred);
		}
	}selectMany;
}
