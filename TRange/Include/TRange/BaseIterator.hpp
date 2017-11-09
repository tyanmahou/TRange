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
		class base_iterator
		{
		protected:
			Iterator m_it;
		public:
			using iterator_category = std::bidirectional_iterator_tag;
			using difference_type = std::size_t;
			using value_type = value_type_t<Iterator>;
			using pointer = value_type*;
			using reference = value_type&;
			base_iterator() = default;
			base_iterator(const Iterator& it) :
				m_it(it)
			{}
			base_iterator(const base_iterator& other) :
				m_it(other.m_it)
			{}

			decltype(auto) operator *()const
			{
				return *m_it;
			}
			value_type* operator ->()const
			{
				return &(*m_it);
			}
			base_iterator& operator ++()
			{
				++m_it;
				return *this;
			}
			base_iterator& operator --()
			{
				--m_it;
				return *this;
			}
			bool operator ==(const base_iterator& other)const
			{
				return m_it == other.m_it;
			}
			bool operator !=(const base_iterator& other)const
			{
				return m_it != other.m_it;
			}
		};

	}//detail

}//trange