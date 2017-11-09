#pragma once
#include"Util.hpp"
#include"ParameterExpand.hpp"
namespace trange
{
	namespace detail
	{
		template<class It>
		class WithIndexIterator
		{
		public:
			using iterator_category = std::bidirectional_iterator_tag;
			using difference_type = std::size_t;
			using value_type = WithIndexPair<decltype(*std::declval<It>())>;
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
		public:
			using iterator = WithIndexIterator<range_iterator_t<Range>>;
			using const_iterator = WithIndexIterator<range_const_iterator_t<Range>>;

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

			const_iterator begin()const
			{
				return { std::begin(m_range),0 };
			}
			const_iterator end()const
			{
				return { std::end(m_range) , std::size(m_range) };
			}

			std::size_t size()const
			{
				return std::size(m_range);
			}

		};
	}

	//“Y‚¦Žš•t‚«
	constexpr struct _WithIndex_OP
	{
		template<class Range>
		auto operator()(Range&& v)const
		{
			return detail::WithIndexRange<Range>(std::forward<Range>(v));
		}
		template<class Range>
		friend auto operator -(Range&& v, _WithIndex_OP op)
		{
			return op(std::forward<Range>(v));
		}
	}withIndex;


}