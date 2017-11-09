#pragma once
#include"Select.hpp"
namespace trange
{
	namespace detail
	{
		template<class It1, class It2>
		class ZipIterator
		{

		public:
			using iterator_category = std::bidirectional_iterator_tag;
			using difference_type = std::size_t;
			using value_type =
				std::pair<decltype(*std::declval<It1>())&, decltype(*std::declval<It2>())&>;
			using pointer = value_type*;
			using reference = value_type&;
		private:
			It1 m_it1;
			It2 m_it2;
		public:
			ZipIterator(It1 it1, It2 it2):
				m_it1(it1),
				m_it2(it2)
			{}
			value_type operator *()const
			{
				return std::make_pair(std::ref(*m_it1), std::ref(*m_it2));
			}
			value_type* operator ->()const
			{
				return &*(*this);
			}
			ZipIterator& operator ++()
			{
				++m_it1;
				++m_it2;
				return *this;
			}
			ZipIterator& operator --()
			{
				--m_it1;
				--m_it2;
				return *this;
			}
			bool operator ==(const ZipIterator& other)const
			{
				return m_it1 == other.m_it1|| m_it2 == other.m_it2;
			}
			bool operator !=(const ZipIterator& other)const
			{
				return m_it1 != other.m_it1 && m_it2 != other.m_it2;
			}


		};

		template<class Range1, class Range2>
		class ZipRange
		{
		private:
			Range1 m_range1;
			Range2 m_range2;
		public:
			using iterator = ZipIterator<range_iterator_t<Range1>, range_iterator_t<Range2>>;
			using const_iterator = ZipIterator<range_const_iterator_t<Range1>, range_const_iterator_t<Range2>>;

			ZipRange(Range1&& range1, Range2&& range2):
				m_range1(range1),
				m_range2(range2)
			{}
			iterator begin()
			{
				return { std::begin(m_range1),std::begin(m_range2) };
			}
			iterator end()
			{
				return { std::end(m_range1),std::end(m_range2)};
			}
			const_iterator begin()const
			{
				return { std::begin(m_range1),std::begin(m_range2) };
			}
			const_iterator end()const
			{
				return { std::end(m_range1),std::end(m_range2) };
			}

			std::size_t size()const
			{
				return trange::detail::size(this->begin(),this->end());
			}


		};
	}

	constexpr struct _Zip_OP
	{
	private:
		template<class Range2>
		struct Parm
		{
			Range2 range2;
		};
	public:
		template<class Range1, class Range2>
		auto operator ()(Range1&& range1, Range2&& range2)const
		{
			return detail::ZipRange<Range1, Range2>(std::forward<Range1>(range1), std::forward<Range2>(range2));
		}
		template<class Range1, class Range2,class Pred>
		auto operator ()(Range1&& range1, Range2&& range2,Pred pred)const
		{
			return detail::ZipRange<Range1, Range2>(std::forward<Range1>(range1), std::forward<Range2>(range2))-trange::select(pred);
		}
		template<class Range2>
		auto  operator ()(Range2&& range2)const
		{
			return Parm<Range2>{ std::forward<Range2>(range2)};
		}
		template<class Range1, class Range2>
		friend auto operator -(Range1&& range1, Parm<Range2> p)
		{
			return  detail::ZipRange<Range1, Range2>(std::forward<Range1>(range1), std::forward<Range2>(p.range2));
		}
	}zip;
}