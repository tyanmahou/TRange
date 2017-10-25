#pragma once
#include<iterator>
namespace trange
{

	namespace detail
	{
		template<class Type>
		struct value_type
		{
		public:
			using type = Type;
		};
		template<class Type>
		struct value_type<std::reference_wrapper<Type>>
		{
		public:
			using type = Type;
		};
		template<class Type>
		struct value_type<std::reference_wrapper<const Type>>
		{
		public:
			using type = Type;
		};

		template<class It>
		using value_type_t = typename value_type<typename std::iterator_traits<It>::value_type>::type;

		///<summary>
		/// 
		///</summary>
		template<class Iterator>
		class trange_iterator
		{
		protected:
			Iterator m_it;
		public:
			using iterator_category = std::bidirectional_iterator_tag;
			using difference_type = std::size_t;
			using value_type = value_type_t<Iterator>;
			using pointer = value_type*;
			using reference = value_type&;
			trange_iterator() = default;
			trange_iterator(const Iterator& it) :
				m_it(it)
			{}
			trange_iterator(const trange_iterator& other) :
				m_it(other.m_it)
			{}

			value_type& operator *()const
			{
				return *m_it;
			}
			value_type* operator ->()const
			{
				return &(*m_it);
			}
			trange_iterator& operator ++()
			{
				++m_it;
				return *this;
			}
			trange_iterator& operator --()
			{
				--m_it;
				return *this;
			}
			bool operator ==(const trange_iterator& other)const
			{
				return m_it == other.m_it;
			}
			bool operator !=(const trange_iterator& other)const
			{
				return m_it != other.m_it;
			}
		};



		///<summary>
		/// constイテレーターにする
		///</summary>
		template<class Iterator>
		class const_iterator 
		{
		private:
			Iterator m_it;
		public:
			using iterator_category = std::bidirectional_iterator_tag;
			using difference_type = std::size_t;
			using value_type = value_type_t<Iterator>;
			using pointer = value_type*;
			using reference = value_type&;

			const_iterator() = default;
			const_iterator(const Iterator& it) :
				m_it(it)
			{}
			const_iterator(const const_iterator& other) :
				m_it(other.m_it)
			{}

			const value_type& operator *()const
			{
				return *m_it;
			}
			const value_type* operator ->()const
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

	}//detail

}//trange