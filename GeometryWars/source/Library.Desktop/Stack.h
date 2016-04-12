#pragma once
#include "SList.h"

namespace Library
{
	/**
	 *	Template class for Stack data structure (LIFO)
	 */
	template <typename T>
	class Stack final
	{
		public:
			/**
			 *	Default constructor, initializes an empty stack
			 */
			Stack() = default;

			/**
			 *	Copy constructor for Stack
			 *	@param reference to the right hand side variable
			 */
			Stack(const Stack& rhs);

			/**
			 *	Move constructor for Stack
			 *	@param R value reference to be moved into this instance of Stack
			 */
			Stack(Stack&& rhs);

			/**
			 *	Assignment operator overload for Stack
			 *	@param reference to the right hand side variable
			 *	@return reference to the resultant Stack
			 */
			Stack& operator=(const Stack& rhs);

			/**
			 *	Move assignment operator
			 *	@param R value reference to be moved into this instance of Stack
			 *	@return reference to the newly moved stack
			 */
			Stack& operator=(Stack&& rhs);

			/**
			 *	Destructor
			 */
			~Stack();

			/**
			 *	Pushes the given element on to the top of the stack
			 *	@param reference to the element to be pushed
			 */
			void Push(const T& data);

			/**
			 *	Removes the topmost element of the list
			 */
			void Pop();

			/**
			 *	Most recently inserted element of the list
			 *	@return reference to the data
			 */
			T& Top();

			/**
			 *	constant overload of Top()
			 *	@return constant reference to the data
			 */
			const T& Top() const;

			/**
			 *	Number of elements in the stack
			 *	@return unsigned integer
			 */
			std::uint32_t Size() const;

			/**
			 *	Checks if the stack is empty
			 *	@return returns true if there is no element in the stack, false otherwise
			 */
			bool IsEmpty() const;
			
			/**
			 *	Removes all elements from the stack
			 */
			void Clear();

		private:
			SList<T> mDataList;
	};
}
#include "Stack.inl"

