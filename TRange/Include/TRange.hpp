#pragma once

#include<type_traits>
#include<iterator>

namespace range
{
	template<class Iterator>
	class const_iterator: public std::iterator_traits<Iterator>
	{
	private:
		Iterator m_it;
	public:
		const_iterator(const Iterator& it):
			m_it(it)
		{}

		const typename std::iterator_traits<Iterator>::value_type& operator *()const
		{
			return *m_it;
		}
		const typename std::iterator_traits<Iterator>::value_type* operator ->()const
		{
			return &(*m_it);
		}
		const_iterator& operator ++()
		{
			++m_it;
			return *this;
		}
		const_iterator& operator --()
		{
			--m_it;
			return *this;
		}
		bool operator ==(const const_iterator& other)const
		{
			return m_it == other.m_it;
		}
		bool operator !=(const const_iterator& other)const
		{
			return m_it != other.m_it;
		}


	};
	namespace detail
	{
		//イテレーター
		template<class It>
		class WithIndexIterator:public std::iterator_traits<It>
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
			using reverse_iterator = std::reverse_iterator<WithIndexIterator<iterator>>;

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
			reverse_iterator rbegin()
			{
				return reverse_iterator(this->end());
			}
			reverse_iterator rend()
			{
				return reverse_iterator(this->begin());
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


	namespace detail
	{
		template<class Range>
		class ReverseRange
		{
			using reverse_iterator = decltype(std::begin(std::declval<Range&>()));
			using iterator = std::reverse_iterator<reverse_iterator>;

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

			const_iterator<iterator> begin()const
			{
				return iterator{ std::end(m_range) };
			}
			const_iterator<iterator> end()const
			{
				return iterator{ std::begin(m_range) };
			}
			reverse_iterator rbegin()
			{
				return std::begin(m_range);
			}
			reverse_iterator rend()
			{
				return std::end(m_range);
			}
			std::size_t size()const
			{
				return std::size(m_range);
			}
		};
	}


	//逆順
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
