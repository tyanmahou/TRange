#pragma once
#include"IRange.hpp"
#include"Util.hpp"
#include"ParameterExpand.hpp"
namespace trange
{

	namespace detail
	{
		//イテレーター
		template<class It>
		class WithIndexIterator
		{
		public:
			using iterator_category = std::bidirectional_iterator_tag;
			using difference_type = std::size_t;
			using value_type = WithIndexPair<decltype(*std::declval<It>())&>;
			using pointer = value_type*;
			using reference = value_type&;
		private:
			It m_iterator;
			std::size_t m_index;
		public:
			WithIndexIterator(It it, std::size_t index) :
				m_iterator(it),
				m_index(index)
			{}

			value_type operator *()const
			{
				return { *m_iterator, m_index };
			}
			value_type* operator ->()const
			{
				return &*(*this);
			}
			WithIndexIterator& operator ++()
			{
				++m_iterator;
				++m_index;
				return *this;
			}
			WithIndexIterator& operator --()
			{
				--m_iterator;
				--m_index;
				return *this;
			}
			bool operator ==(const WithIndexIterator& other)const
			{
				return m_iterator == other.m_iterator;
			}
			bool operator !=(const WithIndexIterator& other)const
			{
				return m_iterator != other.m_iterator;
			}

		};

		template<class Range>
		class WithIndexRange
		{
			using _iterator = range_iterator_t<Range>;
			using iterator = WithIndexIterator<_iterator>;

			using _const_iterator = range_const_iterator_t<const Range>;
			using const_iterator = WithIndexIterator<_const_iterator>;

		private:
			Range m_range;
		public:

			WithIndexRange(Range&& range) :
				m_range(std::forward<Range>(range))
			{}

			iterator begin()
			{
				return { std::begin(m_range),0 };
			}
			iterator end()
			{
				return { std::end(m_range) , std::size(m_range) };
			}

			detail::const_iterator<const_iterator> begin()const
			{
				return const_iterator{ std::begin(m_range),0 };
			}
			detail::const_iterator<const_iterator> end()const
			{
				return const_iterator{ std::end(m_range) , std::size(m_range) };
			}

			std::size_t size()const
			{
				return std::size(m_range);
			}

		};
	}

	//添え字付き
	constexpr struct _WithIndex_OP
	{
		template<class Range>
		friend detail::WithIndexRange<Range> operator -(Range&& v, _WithIndex_OP)
		{
			return detail::WithIndexRange<Range>(std::forward<Range>(v));
		}
	}withIndex;

}