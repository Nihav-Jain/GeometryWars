#pragma once

namespace Library
{

	/**
	 *	Template class for a singly linked list
	 */
	template <class T>
	class SList
	{
	private:
		/**
		 *	Auxiliary class Node, represents individual elements of the list
		 */
		class Node
		{
		public:
			Node(const T& data, Node* nextNode = nullptr);
			Node(const Node& rhs) = delete;
			Node& operator=(const Node& rhs) = delete;

			Node* Next;
			T mData;
		};

		Node* mFront;
		Node* mBack;

		std::uint32_t mSize;

	public:
		/**
		 *	Iterator to traverse the list
		 */
		class Iterator
		{
		public:

			/**
			 *	Constructor for a non-hosted SList<T>::Iterator
			 */
			Iterator();

			/**
			 *	Copy Constructor for SList::Iterator, copies the RHS list iterator
			 *	@param iterator to be copied
			 */
			Iterator(const Iterator& rhs);

			/**
			 *	Assignment operator for SList::Iterator, copies the RHS list iterator
			 *	@param iterator to be copied
			 *	@return reference to the current Iterator as a copy of the RHS list iterator
			 */
			Iterator& operator=(const Iterator& rhs);


			/**
			 *	Move Constructor for SList::Iterator
			 *	@param R value reference to be moved into this instance of the iterator
			 */
			Iterator(Iterator&& rhs);

			/**
			 *	Move assignement operator for SList::Iterator
			 *	@param R value reference to be moved into this instance of the iterator
			 */
			Iterator& operator=(Iterator&& rhs);

			/**
			 *	Compares two iterators based on their host list and current node
			 *	@return equality
			 */
			bool operator==(const Iterator& rhs) const;

			/**
			 *	Compliment of the equality operator
			 *	@return negative equality
			 */
			bool operator!=(const Iterator& rhs) const;

			/**
			 *	Prefix increment operator, moves the iterator to the next element in the list
			 *	@return reference to the current updated Iterator
			 */
			Iterator& operator++();

			/**
			 *	Postfix increment operator, moves the iterator to the next element in the list
			 *	@param just a placeholder to differentiate with the prefix operator
			 *	@return copy of current value of Iterator (postfix operator uses the current value then updates it)
			 */
			Iterator operator++(int);

			/**
			 *	Dereference operator
			 *	@return the element stored in the list at the current position of the iterator
			 */
			T& operator*() const;

			friend class SList<T>;
		private:

			/**
			 *	Constructor for SList::Iterator
			 *	@param current node this iterator points to
			 *	@param owner list of this iterator
			 */
			Iterator(Node* currentNode, const SList<T>* ownerList);

			Node* mCurrentNode;
			const SList<T>* mOwnerList;
		};

		/**
		 *	Constructor for SList, initializes an empty list
		 */
		SList();

		/**
		 *	Copy constructor for SList
		 *	@param reference to the right hand side variable
		 */
		SList(const SList<T>& rhs);

		/**
		*	Assignment operator overload for SList
		*	@param reference to the right hand side variable
		*	@return reference to the resultant SList
		*/
		SList<T>& operator= (const SList<T>& rhs);

		/**
		 *	Move constructor for SList
		 *	@param R value reference to be moved into this instance of SList
		 */
		SList(SList<T>&& rhs);

		/**
		 *	Move assignment operator for SList
		 *	@param R value reference to be moved into this instance of SList
		 *	@return reference to the newly moved list
		 */
		SList<T>& operator= (SList<T>&& rhs);

		/**
		 *	Destructor for SList
		 */
		~SList();

		/**
		 *	Pushes the given element on the front of the list
		 *	@param reference to the element to be pushed
		 */
		Iterator PushFront(const T& item);

		/**
		 *	Pops out the frontmost element of the list
		 */
		void PopFront();

		/**
		 *	Pushes the given element in the back (end) of the list
		 *	@param reference to the element to be pushed
		 */
		Iterator PushBack(const T& item);

		/**
		 *	Checks if the list is empty
		 *	@return returns true if there is no element in the list, false otherwise
		 */
		bool IsEmpty() const;

		/**
		 *	Frontmost element of the list
		 *	@return reference of the template type
		 */
		T& Front();

		/**
		 *	constant overload of Front()
		 *	@return constant reference of the template type
		 */
		const T& Front() const;


		/**
		 *	Last element of the list
		 *	@return reference of the template type
		 */
		T& Back();

		/**
		 *	constant overload of Front()
		 *	@return constant reference of the template type
		 */
		const T& Back() const;

		/**
		 *	Number of elements in the list
		 *	@return integer
		 */
		std::uint32_t Size() const;

		/**
		 *	Deletes all the elements of the list
		 */
		void Clear();

		/**
		 *	Starting point of the iterator
		 *	@return Iterator pointing to the first element of the list
		 */
		Iterator begin() const;

		/**
		 *	One past the end of the list
		 *	@return Iterator pointing to one past the end of the list
		 */
		Iterator end() const;

		/**
		 *	Insert the given value after the element pointed to by the Iterator
		 *	@param the value to be inserted
		 *	@param the element after which the value is to be inserted
		 */
		void InsertAfter(const T& dataToInsert, const Iterator& itr);

		/**
		 *	finds the given value in the list
		 *	@param the value to be searched
		 *	@return Iterator pointing to the element to be searched, points to end of list if the element is not found
		 */
		Iterator Find(const T& dataToFind) const;

		/**
		 *	Removes the first instance of the given value from the list
		 *	@param the value to be removed
		 */
		void Remove(const T& dataToRemove);
	};
}
#include "SList.inl"

