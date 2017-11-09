#pragma once
#include"BaseIterator.hpp"
#include"ParameterExpand.hpp"
#include<vector>
namespace trange
{
	namespace detail
	{

		template<class It, class Pred>
		class WhereIterator :public base_iterator<It>
		{
		private:
			const Pred m_pred;
			const It m_begin;
			const It m_end;
		public:
			template<class Range>
			WhereIterator(It it,Range&& range, Pred pred) :
				base_iterator<It>(it),
				m_begin(std::begin(range)),
				m_end(std::end(range)),
				m_pred(pred)
			{}

			decltype(auto) operator++()
			{
				do
				{
					++m_it;
					if (m_it == m_end)
						break;
				} while (!param_expand(m_pred, *(m_it)));

				return *this;
			}
			decltype(auto) operator--()
			{
				do
				{
					--m_it;
					if (m_it == m_begin)
						break;
				} while (!param_expand(m_pred, *(m_it)));
				return *this;
			}
		};

		template<class Range, class Pred>
		class WhereRange
		{
		public:
			using iterator = WhereIterator<range_iterator_t<Range>, Pred>;
			using const_iterator = WhereIterator<range_const_iterator_t<Range>, Pred>;
		private:
			Range m_range;
			Pred m_pred;
		public:
			WhereRange(Range&& range, Pred pred) :
				m_range(std::forward<Range>(range)),
				m_pred(pred)
			{}

			iterator begin()
			{
				auto it = std::find_if(std::begin(m_range), std::end(m_range), [this](auto&& v) {return param_expand(m_pred,v); });
				return { it,m_range,m_pred };
			}

			iterator end()
			{
				return { std::end(m_range),m_range,m_pred };
			}
			const_iterator begin()const
			{
				auto it = std::find_if(std::begin(m_range), std::end(m_range), [this](auto&& v) {return param_expand(m_pred, v); });
				return { it,m_range,m_pred };
			}

			const_iterator end()const
			{
				return { std::end(m_range),m_range,m_pred };
			}

			std::size_t size()const
			{
				return trange::detail::size(this->begin(), this->end());
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
			return detail::WhereRange<Range,Pred>(std::forward<Range>(v), pred);
		}

		template<class Type, class Pred>
		auto operator ()(std::initializer_list<Type> v, Pred pred)const
		{
			return detail::WhereRange<std::initializer_list<Type>,Pred>(std::forward<std::initializer_list<Type>>(v), pred);
		}
		template<class Pred>
		Parm<Pred>  operator ()(Pred pred)const
		{
			return Parm<Pred>{ pred };
		}

		template<class Range, class Pred>
		friend auto operator -(Range&& v, Parm<Pred> p)
		{
			return  detail::WhereRange<Range,Pred>(std::forward<Range>(v), p.pred);
		}
	}where;

}