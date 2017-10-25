#pragma once
#include"IRange.hpp"
#include"Util.hpp"
#include"ParameterExpand.hpp"
namespace trange
{
	namespace detail
	{
		template<class It, class Pred>
		class SelectIterator : public trange_iterator<It>
		{
		private:
			Pred m_pred;
		public:
			using value_type = 
				decltype(param_expand(std::declval<Pred>(), *std::declval<It>()));
			SelectIterator(It it,Pred pred):
				trange_iterator<It>(it),
				m_pred(pred)
			{}

			decltype(auto) operator *()const
			{
				return param_expand(m_pred, *m_it);
			}

		};
		template<class Range, class Pred>
		class SelectRange :public IRange<SelectIterator<range_iterator_t<Range>,Pred>>
		{
		private:
			Range m_range;
			Pred m_pred;
		public:
			using iterator = SelectIterator<range_iterator_t<Range>, Pred>;

			SelectRange(Range&& range, Pred pred) :
				m_range(range),
				m_pred(pred)
			{}
			iterator begin()
			{
				return { std::begin(m_range),m_pred };
			}
			iterator end()
			{
				return { std::end(m_range),m_pred };
			}
			const_iterator<iterator> begin()const
			{
				return iterator{ std::begin(const_cast<Range&>(m_range)),m_pred };
			}
			const_iterator<iterator> end()const
			{
				return iterator{ std::end(const_cast<Range&>(m_range)),m_pred };
			}

			std::size_t size()const override
			{
				return std::size(m_range);
			}
		};

	}

	constexpr struct _Select_OP
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
			return detail::SelectRange<Range,Pred>(std::forward<Range>(v), pred);
		}

		template<class Type, class Pred>
		auto operator ()(std::initializer_list<Type> v, Pred pred)const
		{
			return detail::SelectRange<std::initializer_list<Type>,Pred>(std::forward<std::initializer_list<Type>>(v), pred);
		}
		template<class Pred>
		Parm<Pred>  operator ()(Pred pred)const
		{
			return Parm<Pred>{ pred };
		}

		template<class Range, class Pred>
		friend auto operator -(Range&& v, Parm<Pred> p)
		{
			return  detail::SelectRange<Range,Pred>(std::forward<Range>(v), p.pred);
		}
	}select;

}