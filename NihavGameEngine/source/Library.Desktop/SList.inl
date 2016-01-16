#include "pch.h"
#include "SList.h"

template <class T>
SList<T>::SList() :
	mFront(nullptr), mBack(nullptr), mSize(0)
{
}

template<class T>
SList<T>::SList(const SList<T>& rhs) : 
	mFront(nullptr), mBack(nullptr), mSize(0)
{
	if (rhs.isEmpty() == false)
	{		
		Node* iterator = rhs.mFront;
		Node* newNode;
		while (iterator != nullptr)
		{
			newNode = new Node(iterator->GetData());
			if (mSize == 0)
			{
				mBack = newNode;
				mFront = newNode;
			}
			else
			{
				mBack->Next = newNode;
				mBack = mBack->Next;
			}
			iterator = iterator->Next;
			mSize++;
		}
	}
}


template <class T>
SList<T>::~SList()
{
	Clear();
	mFront = nullptr;
	mBack = nullptr;
}

template<class T>
SList<T>& SList<T>::operator=(SList<T>& rhs)
{
	if (this != &rhs)
	{
		// clear the list corresponding to the this object
		Clear();

		// deep copy the rhs list
		if (rhs.isEmpty() == false)
		{
			Node* iterator = rhs.mFront;
			Node* newNode;
			while (iterator != nullptr)
			{
				newNode = new Node(iterator->GetData());
				if (mSize == 0)
				{
					mBack = newNode;
					mFront = newNode;
				}
				else
				{
					mBack->Next = newNode;
					mBack = mBack->Next;
				}
				iterator = iterator->Next;
				mSize++;
			}
		}
	}

	return *this;
}

template <class T>
void SList<T>::PushFront(const T& item)
{
	// if this is the first item in the list
	if (mFront == nullptr)
	{
		mFront = new Node(item);
		mBack = mFront;
	}
	// otherwise add it to the front of the list and update the Front pointers
	else
	{
		Node* temp = new Node(item);
		temp->Next = mFront;
		mFront = temp;
	}
	mSize++;
}

template <class T>
void SList<T>::PopFront()
{
	// if the list is empty, throw an exception
	if (isEmpty())
	{
		throw std::exception("Invalid operation. List is empty.");
	}
	// delete the Front node and update the Front pointer to the next Node in the list
	else
	{
		Node* temp = mFront->Next;

		delete mFront;
		mFront = temp;
		mSize--;

		if (isEmpty())
		{
			mBack = nullptr;
		}
	}
}

template <class T>
void SList<T>::PushBack(const T& item)
{
	// if this is the first item in the list
	if (mBack == nullptr)
	{
		mBack = new Node(item);
		mFront = mBack;
	}
	// otherwise add it to the front of the list and update the Back pointer
	else
	{
		Node* temp = new Node(item);
		mBack->Next = temp;
		mBack = temp;
	}
	mSize++;
}

template <class T>
bool SList<T>::isEmpty() const
{
	bool returnVal;
	if (mSize == 0)
	{
		returnVal = true;
	}
	else
	{
		returnVal = false;
	}
	return returnVal;
}

template <class T>
const T& SList<T>::Front() const
{
	if (mFront == nullptr)
	{
		// throw eception
		throw std::exception("Invalid operation. List is empty.");
	}
	return mFront->GetData();
}

template <class T>
const T& SList<T>::Back() const
{
	if (mBack == nullptr)
	{
		throw std::exception("Invalid operation. List is empty.");
	}
	return mBack->GetData();
}

template <class T>
int SList<T>::Size() const
{
	return mSize;
}

template <class T>
void SList<T>::Clear()
{
	Node* temp = mFront;
	Node* referenceToNextNode = nullptr;
	while (temp != nullptr)
	{
		referenceToNextNode = temp->Next;
		delete temp;
		temp = referenceToNextNode;
		mSize--;
	}
	mFront = nullptr;
	mBack = nullptr;
}

template <class T>
SList<T>::Node::Node(const T& data) :
	mData(data), Next(nullptr)
{
}

template <class T>
const T& SList<T>::Node::GetData() const
{
	return mData;
}
