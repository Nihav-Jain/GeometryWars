#include "pch.h"
#include "Graph.h"

namespace Library
{

#pragma region Graph

	template<typename T>
	Graph<T>::Graph() :
		mVertexList(), mEdgeList()
	{
	}

	template<typename T>
	Graph<T>::~Graph()
	{
		Clear();
	}

	template<typename T>
	typename Graph<T>::Traversor Graph<T>::AddVertex(T& vertexData)
	{
		Vertex& addedVertex = **(mVertexList.PushBack(new Vertex(vertexData)));
		Traversor currentVertexTraversor(addedVertex, this);
		return currentVertexTraversor;
	}

	template<typename T>
	typename Graph<T>::Traversor Graph<T>::AddVertex(T& vertexData, Traversor& parentVertex)
	{
		Vertex& newVertex = **(mVertexList.PushBack(new Vertex(vertexData)));
		CreateEdge(newVertex, *parentVertex.mCurrentVertex);
		Traversor currentVertexTraversor(newVertex, this);
		return currentVertexTraversor;
	}

	template<typename T>
	typename Graph<T>::Traversor Graph<T>::Begin() const
	{
		Traversor rootTraversor(**(mVertexList.begin()), this);
		return rootTraversor;
	}

	template<typename T>
	bool Graph<T>::IsEmpty() const
	{
		return mVertexList.IsEmpty();
	}

	template<typename T>
	void Graph<T>::Clear()
	{
		for (auto vertex : mVertexList)
		{
			delete vertex;
		}
		for (auto edge : mEdgeList)
		{
			delete edge;
		}
		mVertexList.Clear();
		mEdgeList.Clear();
	}

	template<typename T>
	void Graph<T>::CreateEdge(Traversor& tailVertex, Traversor& headVertex)
	{
		Vector<Edge*>::Iterator newEdge = mEdgeList.PushBack(new Edge(*(headVertex.mCurrentVertex), *(tailVertex.mCurrentVertex)));
		tailVertex.mCurrentVertex->AddEdge(**newEdge);
	}

	template<typename T>
	void Graph<T>::CreateEdge(Vertex& headVertex, Vertex& tailVertex)
	{
		Vector<Edge*>::Iterator newEdge = mEdgeList.PushBack(new Edge(headVertex, tailVertex));
		tailVertex.AddEdge(**newEdge);
	}

#pragma endregion

#pragma region Vertex
	template<typename T>
	Graph<T>::Vertex::Vertex(T& vertexData) :
		mEdgeList(), mData(vertexData)
	{}

	template<typename T>
	void Graph<T>::Vertex::AddEdge(Edge& newEdge)
	{
		mEdgeList.PushBack(&newEdge);
	}

	template<typename T>
	typename Vector<typename Graph<T>::Edge*>& Graph<T>::Vertex::EdgeList()
	{
		return mEdgeList;
	}

	template<typename T>
	T& Graph<T>::Vertex::Data()
	{
		return mData;
	}

#pragma endregion

#pragma region Edge

	template<typename T>
	Graph<T>::Edge::Edge(Vertex& headVertex, Vertex& tailVertex) :
		mHead(&headVertex), mTail(&tailVertex)
	{}

	template<typename T>
	typename Graph<T>::Vertex* Graph<T>::Edge::GetHead() const
	{
		return mHead;
	}

	template<typename T>
	typename Graph<T>::Vertex* Graph<T>::Edge::GetTail() const
	{
		return mTail;
	}
#pragma endregion

#pragma region Traversor

	template<typename T>
	Graph<T>::Traversor::Traversor() :
		mCurrentVertex(nullptr), mOwner(nullptr), mCurrentChildIndex(0)
	{}

	template<typename T>
	Graph<T>::Traversor::Traversor(const Traversor& rhs) :
		mCurrentVertex(rhs.mCurrentVertex), mOwner(rhs.mOwner), mCurrentChildIndex(rhs.mCurrentChildIndex)
	{}

	template<typename T>
	Graph<T>::Traversor::Traversor(Traversor&& rhs) :
		mCurrentVertex(rhs.mCurrentVertex), mOwner(rhs.mOwner), mCurrentChildIndex(rhs.mCurrentChildIndex)
	{
		rhs.mCurrentVertex = nullptr;
		rhs.mOwner = nullptr;
		rhs.mCurrentChildIndex = 0;
	}

	template<typename T>
	typename Graph<T>::Traversor& Graph<T>::Traversor::operator=(const Traversor& rhs)
	{
		if (this != &rhs)
		{
			mCurrentVertex = rhs.mCurrentVertex;
			mOwner = rhs.mOwner;
			mCurrentChildIndex = rhs.mCurrentChildIndex;
		}
		return *this;
	}

	template<typename T>
	typename Graph<T>::Traversor& Graph<T>::Traversor::operator=(Traversor&& rhs)
	{
		if (this != &rhs)
		{
			mCurrentVertex = rhs.mCurrentVertex;
			mOwner = rhs.mOwner;
			mCurrentChildIndex = rhs.mCurrentChildIndex;

			rhs.mCurrentVertex = nullptr;
			rhs.mOwner = nullptr;
			rhs.mCurrentChildIndex = 0;
		}
		return *this;
	}

	template<typename T>
	bool Graph<T>::Traversor::operator==(const Traversor& rhs) const
	{
		return (mOwner == rhs.mOwner) && (mCurrentVertex == rhs.mCurrentVertex);
	}

	template<typename T>
	bool Graph<T>::Traversor::operator!=(const Traversor& rhs) const
	{
		return !operator==(rhs);
	}

	template<typename T>
	bool Graph<T>::Traversor::HasMoreChildren() const
	{
		if (mCurrentVertex == nullptr || mOwner == nullptr)
			throw std::exception("Invalid operation. Cannot dereference non-hosted Traversor.");
		if (mCurrentVertex->EdgeList().Size() <= mCurrentChildIndex)
			return false;
		return true;
	}

	template<typename T>
	T& Graph<T>::Traversor::GetCurrentChildVertex() const
	{
		if (mCurrentVertex == nullptr || mOwner == nullptr)
			throw std::exception("Invalid operation. Cannot dereference non-hosted Traversor.");
		if (mCurrentVertex->EdgeList().Size() <= mCurrentChildIndex)
			throw std::exception("Invalid operation. Current vertex does not have any more children.");
		return (mCurrentVertex->EdgeList()[mCurrentChildIndex])->GetHead()->Data();
	}

	template<typename T>
	bool Graph<T>::Traversor::MoveToNextChild()
	{
		if (mCurrentVertex == nullptr || mOwner == nullptr)
			throw std::exception("Invalid operation. Cannot dereference non-hosted Traversor.");
		mCurrentChildIndex++;
		if (mCurrentVertex->EdgeList().Size() <= mCurrentChildIndex)
			return false;
		return true;
	}

	template<typename T>
	bool Graph<T>::Traversor::TraverseToCurrentChild()
	{
		if (mCurrentVertex == nullptr || mOwner == nullptr)
			throw std::exception("Invalid operation. Cannot dereference non-hosted Traversor.");
		if (mCurrentVertex->EdgeList().Size() <= mCurrentChildIndex)
			return false;
		mCurrentVertex = (mCurrentVertex->EdgeList()[mCurrentChildIndex])->GetHead();
		mCurrentChildIndex = 0;
		return true;
	}

	template<typename T>
	T& Graph<T>::Traversor::operator*()
	{
		if (mCurrentVertex == nullptr || mOwner == nullptr)
			throw std::exception("Invalid operation. Cannot dereference non-hosted Traversor.");
		return mCurrentVertex->Data();
	}

	template<typename T>
	void Graph<T>::Traversor::ResetChildrenIterator()
	{
		mCurrentChildIndex = 0;
	}

	template<typename T>
	Graph<T>::Traversor::Traversor(Vertex& currentVertex, const Graph* owner) :
		mCurrentVertex(&currentVertex), mOwner(owner), mCurrentChildIndex(0)
	{}

#pragma endregion
}