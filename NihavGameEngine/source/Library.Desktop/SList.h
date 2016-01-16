#pragma once

/**
 *	Template class for a singly linked list
 */
template <class T>
class SList
{
public:
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
 	 *	Destructor for SList
	 */
	~SList();

	/**
	 *	Assignment operator overload for SList
	 *	@param reference to the right hand side variable
	 *	@return reference to the resultant SList
	 */
	SList<T>& operator= (SList<T>& rhs);

	/**
	 *	Pushes the given element on the front of the list
	 *	@param reference to the element to be pushed
	 */
	void PushFront(const T& item);

	/**
	 *	Pops out the frontmost element of the list
	 */
	void PopFront();

	/**
	 *	Pushes the given element in the back (end) of the list
	 *	@param reference to the element to be pushed
	 */
	void PushBack(const T& item);

	/**
	 *	Checks if the list is empty
	 *	@return returns true if there is no element in the list, false otherwise
	 */
	bool isEmpty() const;

	/**
	 *	Frontmost element of the list
	 *	@return constant reference of the template type
	 */
	const T& Front() const;

	/**
	 *	Last element of the list
	 *	@return constant reference of the template type
	 */
	const T& Back() const;

	/**
	 *	Number of elements in the list
	 *	@return integer
	 */
	int Size() const;

	/**
	 *	Deletes all the elements of the list
	 */
	void Clear();

private:
	/**
	 *	Auxiliary class Node, represents individual elements of the list
	 */
	class Node
	{
	public:
		Node(const T& data);
		const T& GetData() const;

		Node* Next;
	private:
		T mData;
	};

	Node* mFront;
	Node* mBack;

	int mSize;
};

#include "SList.inl"