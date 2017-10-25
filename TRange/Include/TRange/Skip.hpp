#pragma once
#include"IRange.h"
#include"Util.h"
namespace trange
{

	namespace detail
	{

		template<class Range>
		class SkipRange :public IRange<range_iterator_t<Range>>
		{
			using iterator = range_iterator_t<Range>;
		private:
			Range m_range;
			iterator m_begin;
			iterator m_end;
		public:
			SkipRange(Range&& range, std::size_t num) :
				m_range(std::forward<Range>(range)),
				m_end(std::end(m_range))
			{
				m_begin = std::begin(m_range);
				std::advance(m_begin, std::min(num,std::size(m_range)));
			}
			template<class Pred>
			SkipRange(Range&& range, Pred pred) :
				m_range(std::forward<Range>(range)),
				m_end(std::end(m_range))
			{
				m_begin = std::begin(m_range);

				bool skip = false;
				int num = 0;
				for (auto&&elm : m_range)
				{
					if (!pred(elm))
					{
						break;
					}
					++num;
				}

				std::advance(m_begin, num);
			}

			iterator begin()
			{
				return m_begin;
			}

			iterator end()
			{
				return m_end;
			}
			const_iterator<iterator> begin()const
			{
				return m_begin;
			}

			const_iterator<iterator> end()const
			{
				return m_end;
			}
			std::size_t size()const override
			{
				return trange::detail::size(m_begin, m_end);
			}
		};
	}

	constexpr struct _Skip_OP
	{
	private:
		struct Parm
		{
			std::size_t num;
		};
	public:
		template<class Range>
		auto operator ()(Range&& v, std::size_t num)const
		{
			return detail::SkipRange<Range>(std::forward<Range>(v), num);
		}
		template<class Type>
		auto operator ()(std::initializer_list<Type> v, std::size_t num)const
		{
			return detail::SkipRange<std::initializer_list<Type>>(std::forward<std::initializer_list<Type>>(v), num);
		}

		Parm  operator ()(std::size_t num)const
		{
			return Parm{ num };
		}

		template<class Range>
		friend detail::SkipRange<Range> operator -(Range&& v, Parm p)
		{
			return  detail::SkipRange<Range>(std::forward<Range>(v), p.num);
		}

	}skip;

	constexpr struct _SkipWhile_OP
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
			return detail::SkipRange<Range>(std::forward<Range>(v), pred);
		}

		template<class Type,class Pred>
		auto operator ()(std::initializer_list<Type> v, Pred pred)const
		{
			return detail::SkipRange<std::initializer_list<Type>>(std::forward<std::initializer_list<Type>>(v), pred);
		}
		template<class Pred>
		Parm<Pred>  operator ()(Pred pred)const
		{
			return Parm<Pred>{ pred };
		}

		template<class Range, class Pred>
		friend auto operator -(Range&& v, Parm<Pred> p)
		{
			return  detail::SkipRange<Range>(std::forward<Range>(v), p.pred);
		}
	}skipWhile;
}