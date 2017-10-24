#pragma once
#include<iterator>
namespace trange
{

	///<summary>
	/// constイテレーターにする
	///</summary>
	template<class Iterator>
	class const_iterator : public std::iterator_traits<Iterator>
	{
	private:
		Iterator m_it;
	public:
		const_iterator() = default;
		const_iterator(const Iterator& it) :
			m_it(it)
		{}
		const_iterator(const const_iterator& other) :
			m_it(other.m_it)
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

	template<class Iterator>
	class IRange
	{
	public:
		virtual Iterator begin() = 0;
		virtual Iterator end() = 0;	
		virtual std::size_t size()const = 0;
	};

}//trange