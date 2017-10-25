#pragma once
#include"IRange.hpp"
#include"Util.hpp"
namespace trange
{
	namespace detail
	{
		template<class Range>
		class ReverseRange : public IRange<std::reverse_iterator<range_iterator_t<Range>>>
		{
			using reverse_iterator = range_iterator_t<Range>;
			using iterator = std::reverse_iterator<reverse_iterator>;

		private:
			Range m_range;
		public:
			ReverseRange(Range&& range) :
				m_range(std::forward<Range>(range))
			{}

			iterator begin()override
			{
				return iterator{ std::end(m_range) };
			}
			iterator end()override
			{
				return iterator{ std::begin(m_range) };
			}

			const_iterator<iterator> begin()const
			{
				return iterator{ std::end(m_range) };
			}
			const_iterator<iterator> end()const
			{
				return iterator{ std::begin(m_range) };
			}
			std::size_t size()const override
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
		friend detail::ReverseRange<Range> operator -(Range&& v, _Reverse_OP)
		{
			return detail::ReverseRange<Range>(std::forward<Range>(v));
		}

	}reverse;
}

