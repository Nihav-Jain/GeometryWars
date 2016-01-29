#pragma once

namespace Library
{

	/**
	 *	Template class for a vector
	 */
	template <class T>
	class Vector
	{

		private:
			/**
			 *	Functor class to define the default reserve strategy to be used by this Vector
			 */
			class DefaultReserveStrategy
			{
				public:
					/**
					 *	Operaror () override for functor
					 *	@param unsigned integer representinf current size of the vector
					 *	@param unsigned integer representing current capacity of the vector
					 *	@return unsigned integer representing new capacity for the vector
					 */
					uint32_t operator()(uint32_t size, uint32_t capacity) const;
			};
			uint32_t mCapacity;
			uint32_t mSize;

			T* mDataArray;

			static const uint32_t DEFAULT_CAPACITY = 4;

		public:

			/**
			 *	Iterator to traverse the list
			 */
			class Iterator
			{
				friend Vector;
				public:
					/**
					 *	Parameterless constructor for Iterator
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
					 *	@param reference to the right hand side variable
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
					 */
					Iterator& operator++();

					/**
					 *	Postfix incerment operator overload for Iterator
					 *	@param dummy integer to differntiate function declaration with the prefix operator
					 *	@return copy of Iterator before incrementing it
					 */
					Iterator operator++(int);

					/**
					 *	Dereference operator overload for Iterator
					 *	@return reference to the data pointed to by the Iterator
					 */
					T& operator*() const;

				private:
					/**
					 *	Parametarized constructor for iterator, can only be invoked from inside this class or from Vector<T>
					 */
					Iterator(uint32_t currentIndex, const Vector<T>* ownerVector);

					uint32_t mCurrentIndex;
					const Vector<T>* mOwnerVector;
			};

			/**
			 *	Parameterless constructor to create an empty vector
			 */
			Vector();

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
			 */
			Vector& operator=(Vector&& rhs);

			/**
			 *	Destructor for Vector
			 */
			~Vector();

			/**
			 *	Subscript operator overload for Vector, throws std::exception if index is more than size of vector
			 *	@param unsigned integer index position for the requested data stored in Vector
			 *	@return reference to the data stored at the given index
			 */
			T& operator[](uint32_t index);

			/**
			 *	Subscript operator overload for Vector, throws std::exception if index is more than size of vector
			 *	@param unsigned integer index position for the requested data stored in Vector
			 *	@return reference to the data stored at the given index
			 */
			const T& operator[](uint32_t index) const;
			
			/**
			 *	remove the last element of the vector
			 */
			void PopBack();

			/**
			 *	Add a copy of the given data to the end of the vector
			 *	@param reference to data to push
			 *	@param functor to define the reserve strategy to be used in case capacity is full - Functor definition - uint32_t operator()(uint32_t size, uint32_t capacity) const;
			 *	@return Iterator to the newly added data
			 */
			template <typename F = DefaultReserveStrategy>
			Iterator PushBack(const T& dataToPush, F reserveStrategyFunctor = DefaultReserveStrategy());

			/**
			 *	Return the first element of the Vector
			 *	@return reference to the data stored at the front of the vector
			 */
			T& Front();

			/**
			 *	Return the first element of the Vector
			 *	@return reference to the data stored at the front of the vector
			 */
			const T& Front() const;

			/**
			 *	Return the last element of the Vector
			 *	@return reference to the data stored at the end of the vector
			 */
			T& Back();

			/**
			 *	Return the last element of the Vector
			 *	@return reference to the data stored at the end of the vector
			 */
			const T& Back() const;

			/**
			 *	Gets the reference to the element stored at the given index
			 *	@param unsigned integer index of the data stored in vector
			 *	@return reference to the data stored at the given index of the vector
			 */
			T& At(uint32_t index);

			/**
			 *	Reserves space for the given number of elements if it is more than the current capacity
			 *	@param new capacity for the vector
			 */
			void Reserve(uint32_t newCapacity);

			/**
			 *	Destructivly clears the Vector - release all memory
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
			 */
			void Remove(Iterator itr);

			/**
			 *	Removes a range of data from the vector, from param1 to less than param2
			 *	@param iterator from which removal has to begin
			 *	@param iterator pointing to one past the last data to be removed
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
			uint32_t Size() const;

			/**
			 *	Gets the number of elements for which memory has been reserved by this vector
			 *	@return capacity of the vector
			 */
			uint32_t Capacity() const;

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
			friend Iterator;
	};

}
#include "Vector.inl"

