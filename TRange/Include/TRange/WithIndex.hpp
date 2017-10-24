#pragma once
#include"IRange.h"

namespace trange
{

	namespace detail
	{
		//イテレーター
		template<class It>
		class WithIndexIterator :public std::iterator_traits<It>
		{
		public:
			using value_type = struct
			{
				decltype(*std::declval<It>())& value;
				const std::size_t index;
			};
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

		template<class Range, bool isConst = false>
		class WithIndexRange
		{
			using iterator = std::conditional_t<isConst,
				decltype(std::cbegin(std::declval<Range&>())),
				decltype(std::begin(std::declval<Range&>()))>;
		private:
			Range m_range;
		public:

			WithIndexRange(Range&& range) :
				m_range(std::forward<Range>(range))
			{}

			WithIndexIterator<iterator> begin()
			{
				return { std::begin(m_range),0 };
			}
			WithIndexIterator<iterator> end()
			{
				return { std::end(m_range) , std::size(m_range) };
			}

			const_iterator<WithIndexIterator<iterator>> begin()const
			{
				return { std::begin(m_range),0 };
			}
			const_iterator<WithIndexIterator<iterator>> end()const
			{
				return { std::end(m_range) , std::size(m_range) };
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
	}with_index;

	//添え字付き 値const
	constexpr struct _WithIndexConst_OP
	{

		template<class Range>
		friend detail::WithIndexRange<Range, true> operator -(Range&& v, _WithIndexConst_OP)
		{
			return detail::WithIndexRange<Range, true>(std::forward<Range>(v));
		}

	}with_index_const;

}