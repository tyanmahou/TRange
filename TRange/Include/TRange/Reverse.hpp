#pragma once
#include"Util.hpp"
namespace trange
{
	namespace detail
	{
		template<class Range>
		class ReverseRange
		{
		public:
			using iterator = std::reverse_iterator<range_iterator_t<Range>>;
			using const_iterator = std::reverse_iterator<range_const_iterator_t<Range>>;
		private:
			Range m_range;
		public:
			ReverseRange(Range&& range) :
				m_range(std::forward<Range>(range))
			{}

			iterator begin()
			{
				return iterator{ std::end(m_range) };
			}
			iterator end()
			{
				return iterator{ std::begin(m_range) };
			}

			const_iterator begin()const
			{
				return const_iterator{ std::end(m_range) };
			}
			const_iterator end()const
			{
				return const_iterator{ std::begin(m_range) };
			}
			std::size_t size()const
			{
				return std::size(m_range);
			}
		};
	}


	//‹t‡
	constexpr struct _Reverse_OP
	{
		template<class Range>
		detail::ReverseRange<Range> operator ()(Range&& v)const
		{
			return detail::ReverseRange<Range>(std::forward<Range>(v));
		}

		template<class Range>
		friend auto operator -(Range&& v, _Reverse_OP op)
		{
			return op(std::forward<Range>(v));
		}

	}reverse;
}

