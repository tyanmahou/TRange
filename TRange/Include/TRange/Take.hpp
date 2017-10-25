#pragma once
#include"IRange.hpp"
#include"Util.hpp"
namespace trange
{

	namespace detail
	{

		template<class Range>
		class TakeRange :public IRange<range_iterator_t<Range>>
		{
			using iterator = range_iterator_t<Range>;
		private:
			Range m_range;
			std::size_t m_off;
		public:
			TakeRange(Range&& range, std::size_t num) :
				m_range(std::forward<Range>(range)),
				m_off(std::min(num, std::size(m_range)))
			{}
			template<class Pred>
			TakeRange(Range&& range, Pred pred) :
				m_range(std::forward<Range>(range))
			{
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

				m_off = num;
			}

			iterator begin()
			{
				return std::begin(m_range);
			}

			iterator end()
			{
				return advance(std::begin(m_range),m_off);
			}
			const_iterator<iterator> begin()const
			{
				return std::begin(const_cast<Range&>(m_range));
			}

			const_iterator<iterator> end()const
			{
				return advance(std::begin(const_cast<Range&>(m_range)), m_off);
			}
			std::size_t size()const override
			{
				return trange::detail::size(this->begin(), this->end());
			}
		};
	}

	constexpr struct _Take_OP
	{
	private:
		struct Parm
		{
			std::size_t num;
		};
	public:
		template<class Range>
		detail::TakeRange<Range> operator ()(Range&& v, std::size_t num)const
		{
			return detail::TakeRange<Range>(std::forward<Range>(v), num);
		}

		Parm  operator ()(std::size_t num)const
		{
			return Parm{ num };
		}

		template<class Range>
		friend detail::TakeRange<Range> operator -(Range&& v, Parm p)
		{
			return  detail::TakeRange<Range>(std::forward<Range>(v), p.num);
		}
	}take;

	constexpr struct _TakeWhile_OP
	{
	private:
		template<class Pred>
		struct Parm
		{
			Pred pred;
		};
	public:
		template<class Range, class Pred>
		detail::TakeRange<Range> operator ()(Range&& v, Pred pred)const
		{
			return detail::TakeRange<Range>(std::forward<Range>(v), pred);
		}
		template<class Pred>
		Parm<Pred>  operator ()(Pred pred)const
		{
			return Parm<Pred>{ pred };
		}

		template<class Range, class Pred>
		friend detail::TakeRange<Range> operator -(Range&& v, Parm<Pred> p)
		{
			return  detail::TakeRange<Range>(std::forward<Range>(v), p.pred);
		}
	}takeWhile;
}