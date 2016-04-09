#pragma once
#include <initializer_list>
#include "DefaultReserveStrategy.h"

namespace Library
{

	/**
	 *	Template class for a contigous list of elements
	 *	T is type of data to be stored
	 *	(optional) F is the functor type which defines the reserve strategy for the Vector
	 */
	template <typename T, typename F = DefaultReserveStrategy>
	class Vector final
	{

		private:
			std::uint32_t mCapacity;
			std::uint32_t mSize;

			T* mDataArray;

			static const std::uint32_t DEFAULT_CAPACITY = 4;
			static const F ReserveStrategyFunctor;

		public:

			/**
			 *	Iterator to traverse the list
			 */
			class Iterator
			{
				friend Vector;
				public:
					/**
					 *	Parameterless constructor for Iterator, makes a non-hosted Iterator
					 */
					Iterator();

					/**
					 *	Copy constructor for Iterator
					 *	@param reference to the Iterator to be copied from
					 */
					Iterator(const Iterator& rhs);

					/**
					 *	Move constructor for Iterator
					 *	@param R value referene to the Iterator to be moved into this instance
					 */
					Iterator(Iterator&& rhs);

					/**
					 *	Assignment operator overload for Iterator
					 *	@param reference to the right hand side variable
					 *	@return reference to this Iterator
					 */
					Iterator& operator=(const Iterator& rhs);
					
					/**
					 *	Move assignment operator for Iterator
					 *	@param R value referene to the Iterator to be moved into this instance
					 */
					Iterator& operator=(Iterator&& rhs);

					/**
					 *	Equality operator overload for Iterator
					 *	@param reference to the Iterator to be comapred with
					 *	@return true if the owner vector and the current index of both iterators is same, false otherwise
					 */
					bool operator==(const Iterator& rhs) const;

					/**
					 *	Not equals operator overload for Iterator
					 *	@param reference to the right hand side variable
					 *	@return compliment of equality of two iterators
					 */
					bool operator!=(const Iterator& rhs) const;
					
					/**
					 *	Prefix incerment operator overload for Iterator
					 *	@return reference to the incremented iterator
					 *	@exception thrown if the Iterator is non-hosted (owner is null) or it points to an invalid element
					 */
					Iterator& operator++();

					/**
					 *	Postfix incerment operator overload for Iterator
					 *	@param dummy integer to differntiate function declaration with the prefix operator
					 *	@return copy of Iterator before incrementing it
					 *	@exception thrown if the Iterator is non-hosted (owner is null) or it points to an invalid element
					 */
					Iterator operator++(int);

					/**
					 *	Dereference operator overload for Iterator
					 *	@return reference to the data pointed to by the Iterator
					 *	@exception thrown if the Iterator is non-hosted (owner is null) or it points to an invalid element
					 */
					T& operator*();

					/**
					 *	const overload for the dereference operator of Iterator
					 *	@return reference to the data pointed to by the Iterator
					 *	@exception thrown if the Iterator is non-hosted (owner is null) or it points to an invalid element
					 */
					const T& operator*() const;

				private:
					/**
					 *	Parametarized constructor for iterator, can only be invoked from inside this class or from Vector
					 */
					Iterator(std::uint32_t currentIndex, const Vector* ownerVector);

					std::uint32_t mCurrentIndex;
					const Vector<T, F>* mOwnerVector;
			};

			/**
			 *	Parameterless constructor to create an empty vector
			 */
			explicit Vector(std::uint32_t defaultCapacity = DEFAULT_CAPACITY, bool fixedSize = false);

			Vector(std::initializer_list<T> initList);

			/**
			 *	Copy constructor for Vector
			 *	@param reference to the Vector to be copied from
			 */
			Vector(const Vector& rhs);

			/**
			 *	Move constructor for Vector
			 *	@param reference to R value Vector to be moved to this instance
			 */
			Vector(Vector&& rhs);

			/**
			 *	Assignment operator overload for Vector
			 *	@param reference to the Vector to be assigned
			 *	@return reference to this Vector
			 */
			Vector& operator=(const Vector& rhs);

			/**
			 *	Move assignment operator for Vector
			 *	@param reference to R value Vector to be moved to this instance
			 *	@return reference to this Vector
			 */
			Vector& operator=(Vector&& rhs);

			/**
			 *	Destructor for Vector
			 */
			~Vector();

			/**
			 *	Subscript operator overload for Vector
			 *	@param unsigned integer index position for the requested data stored in Vector
			 *	@return reference to the data stored at the given index
			 *	@exception thrown if index is more than or equal to size of vector
			 */
			T& operator[](std::uint32_t index);

			/**
			 *	Subscript operator overload for Vector, throws std::exception if index is more than size of vector
			 *	@param unsigned integer index position for the requested data stored in Vector
			 *	@return reference to the data stored at the given index
			 *	@exception thrown if index is more than or equal to size of vector
			 */
			const T& operator[](std::uint32_t index) const;
			
			/**
			 *	Remove the last element of the vector
			 *	@exception thrown if vector is empty
			 */
			void PopBack();

			/**
			 *	Adds a copy of the given data to the end of the vector
			 *	@param reference to data to push
			 *	@return Iterator to the newly added data
			 *	@exception thrown if the reserve stategy returns the new capacity which less than, or equal to the old capacity
			 */
			Iterator PushBack(const T& dataToPush);

			/**
			 *	Return the first element of the Vector
			 *	@return reference to the data stored at the front of the vector
			 *	@exception thrown if vector is empty
			 */
			T& Front();

			/**
			 *	Return the first element of the Vector
			 *	@return reference to the data stored at the front of the vector
			 *	@exception thrown if vector is empty
			 */
			const T& Front() const;

			/**
			 *	Return the last element of the Vector
			 *	@return reference to the data stored at the end of the vector
			 *	@exception thrown if vector is empty
			 */
			T& Back();

			/**
			 *	Return the last element of the Vector
			 *	@return reference to the data stored at the end of the vector
			 *	@exception thrown if vector is empty
			 */
			const T& Back() const;

			/**
			 *	Gets the reference to the element stored at the given index
			 *	@param unsigned integer index of the data stored in vector
			 *	@return reference to the data stored at the given index of the vector
			 *	@exception thrown if index is more than or equal to size of vector
			 */
			T& At(std::uint32_t index);

			/**
			 *	Reserves space for the given number of elements if it is more than the current capacity
			 *	@param new capacity for the vector
			 *	@param boolean to indicate if the newly allocated memory should be default constructed or not
			 */
			void Reserve(std::uint32_t newCapacity, bool fixedSize = false);

			/**
			 *	Destructively clears the Vector, resets the vector to be in its initial empty state
			 */
			void Clear();

			/**
			 *	Searches the given data in the Vector
			 *	@param reference to the data to be searched
			 *	@return Iterator pointing to  the data, if found in the Vector; points to end() if data is not found
			 */
			Iterator Find(const T& dataToFind) const;

			/**
			 *	Removes the given data from the vector, no affect if the data is not present
			 *	@param data to be deleted from vector
			 */
			void Remove(const T& dataToRemove);

			/**
			 *	Removes the data represented by the given Iterator
			 *	@param iterator whose data has to be removed
			 *	@exception thrown if the provided Iterator does not belong to this instance of the Vector
			 */
			void Remove(const Iterator& itr);

			/**
			 *	Removes a range of data from the vector, from param1 to one less than param2
			 *	@param iterator from which removal has to begin
			 *	@param iterator pointing to one past the last data to be removed
			 *	@exception thrown if either of the provided Iterators does not belong to this instance of the Vector
			 *	@exception thrown if param1 points to an element which comes after the element pointed to by param2
			 */
			void Remove(Iterator itrBegin, Iterator itrEnd);

			/**
			 *	Checks if the vector has any elements in it or not
			 *	@return true if vector is empty, false otherwise
			 */
			bool IsEmpty() const;

			/**
			 *	Gets the number of elemets currently stored in the vector
			 *	@return size of the vector
			 */
			std::uint32_t Size() const;

			/**
			 *	Gets the number of elements for which memory has been reserved by this vector
			 *	@return capacity of the vector
			 */
			std::uint32_t Capacity() const;

			/**
			 *	Gets the iterator poitning to the first element of the vector
			 *	@return Iterator
			 */
			Iterator begin() const;

			/**
			 *	Gets the iterator poitning to one past the last element of the vector
		 	 *	@return Iterator
			 */
			Iterator end() const;
	};

}
#include "Vector.inl"

