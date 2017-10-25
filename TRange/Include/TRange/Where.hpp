#pragma once
#include"IRange.hpp"
#include"Util.hpp"
#include<vector>
namespace trange
{


	namespace detail
	{
		template<class Range>
		using where_refs =std::vector<std::reference_wrapper<range_value_t<Range>>>;
		template<class Range>
		using where_iterator = typename where_refs<Range>::iterator;

		template<class Range>
		class WhereRange :public IRange<where_iterator<Range>>
		{
		public:
			using iterator = where_iterator<Range>;
		private:
			Range m_range;
			where_refs<Range> m_refs;
		public:
			template<class Pred>
			WhereRange(Range&& range,Pred pred) :
				m_range(std::forward<Range>(range))
			{
				m_refs.reserve(std::size(m_range));
				for (auto&&elm :m_range)
				{
					if (pred(elm))
					{
						m_refs.emplace_back(elm);
					}
				}
			}

			iterator begin()
			{
				return std::begin(m_refs);
			}

			iterator end()
			{
				return  std::end(m_refs);
			}
			const_iterator<iterator> begin()const
			{
				return iterator{ std::begin(const_cast<where_refs<Range>&>(m_refs)) };
			}

			const_iterator<iterator> end()const
			{
				return iterator{ std::end(const_cast<where_refs<Range>&>(m_refs)) };
			}

			std::size_t size()const
			{
				return std::size(m_refs);
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