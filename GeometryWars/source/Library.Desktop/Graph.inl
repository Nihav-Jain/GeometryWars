#include "pch.h"
#include "Graph.h"

namespace Library
{

#pragma region Graph

	template<typename T>
	Graph<T>::Graph() :
		mVertexList(0), mEdgeList(0)
	{}

	template<typename T>
	Graph<T>::~Graph()
	{
		Clear();
	}

	template<typename T>
	typename Graph<T>::Traversor Graph<T>::AddVertex(const T& vertexData)
	{
		Vertex& addedVertex = **(mVertexList.PushBack(new Vertex(vertexData)));
		Traversor currentVertexTraversor(addedVertex, this);
		return currentVertexTraversor;
	}

	template<typename T>
	typename Graph<T>::Traversor Graph<T>::AddVertex(const T& vertexData, Traversor& parentVertex)
	{
		Vertex& newVertex = **(mVertexList.PushBack(new Vertex(vertexData)));
		CreateEdge(newVertex, *parentVertex.mCurrentVertex);
		Traversor currentVertexTraversor(newVertex, this);
		return currentVertexTraversor;
	}

	template<typename T>
	typename Graph<T>::Traversor Graph<T>::AddParentVertex(const T& vertexData, Traversor& childVertex)
	{
		Vertex& newVertex = **(mVertexList.PushBack(new Vertex(vertexData)));
		CreateEdge(*childVertex.mCurrentVertex, newVertex);
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
	typename Graph<T>::BreadthFirstTraversor Graph<T>::BeginBreadthFirstTraversal() const
	{
		return BreadthFirstTraversor(**(mVertexList.begin()), this);
	}

	template<typename T>
	typename Graph<T>::DepthFirstTraversor Graph<T>::BeginDepthFirstTraversal() const
	{
		return DepthFirstTraversor(**(mVertexList.begin()), this);
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
	Graph<T>::Vertex::Vertex(const T& vertexData) :
		mEdgeList(0), mData(vertexData)
	{}

	template<typename T>
	Graph<T>::Vertex::Vertex(const Vertex& rhs) :
		mEdgeList(rhs.mEdgeList), mData(rhs.mData)
	{}

	template<typename T>
	Graph<T>::Vertex::Vertex(Vertex&& rhs) :
		mEdgeList(std::move(rhs.mEdgeList)), mData(std::move(rhs.mData))
	{}

	template <typename T>
	typename Graph<T>::Vertex& Graph<T>::Vertex::operator=(const Vertex& rhs)
	{
		if (this != &rhs)
		{
			mEdgeList = rhs.mEdgeList;
			mData = rhs.mData;
		}

		return *this;
	}

	template <typename T>
	typename Graph<T>::Vertex& Graph<T>::Vertex::operator=(Vertex&& rhs)
	{
		if (this != &rhs)
		{
			mEdgeList = std::move(rhs.mEdgeList);
			mData = std::move(rhs.mData);
		}
		return *this;
	}

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
	Graph<T>::Edge::Edge(const Edge& rhs) :
		mHead(rhs.mHead), mTail(rhs.mTail)
	{}

	template<typename T>
	Graph<T>::Edge::Edge(Edge&& rhs) :
		mHead(rhs.mHead), mTail(rhs.mTail)
	{
		rhs.mHead = nullptr;
		rhs.mTail = nullptr;
	}

	template <typename T>
	typename Graph<T>::Edge& Graph<T>::Edge::operator=(const Edge& rhs)
	{
		if (this != &rhs)
		{
			mHead = rhs.mHead;
			mTail = rhs.mTail;
		}

		return *this;
	}

	template <typename T>
	typename Graph<T>::Edge& Graph<T>::Edge::operator=(Edge&& rhs)
	{
		if (this != &rhs)
		{
			mHead = rhs.mHead;
			mTail = rhs.mTail;

			rhs.mHead = nullptr;
			rhs.mTail = nullptr;
		}

		return *this;
	}


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
			throw std::exception("ClearScreen operation. Cannot dereference non-hosted Traversor.");
		if (mCurrentVertex->EdgeList().Size() <= mCurrentChildIndex)
			return false;
		return true;
	}

	template<typename T>
	T& Graph<T>::Traversor::GetCurrentChildVertex() const
	{
		if (mCurrentVertex == nullptr || mOwner == nullptr)
			throw std::exception("ClearScreen operation. Cannot dereference non-hosted Traversor.");
		if (mCurrentVertex->EdgeList().Size() <= mCurrentChildIndex)
			throw std::exception("ClearScreen operation. Current vertex does not have any more children.");
		return (mCurrentVertex->EdgeList()[mCurrentChildIndex])->GetHead()->Data();
	}

	template<typename T>
	bool Graph<T>::Traversor::MoveToNextChild()
	{
		if (mCurrentVertex == nullptr || mOwner == nullptr)
			throw std::exception("ClearScreen operation. Cannot dereference non-hosted Traversor.");
		mCurrentChildIndex++;
		if (mCurrentVertex->EdgeList().Size() <= mCurrentChildIndex)
			return false;
		return true;
	}

	template<typename T>
	bool Graph<T>::Traversor::TraverseToCurrentChild()
	{
		if (mCurrentVertex == nullptr || mOwner == nullptr)
			throw std::exception("ClearScreen operation. Cannot dereference non-hosted Traversor.");
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
			throw std::exception("ClearScreen operation. Cannot dereference non-hosted Traversor.");
		return mCurrentVertex->Data();
	}

	template<typename T>
	const T& Graph<T>::Traversor::operator*() const
	{
		if (mCurrentVertex == nullptr || mOwner == nullptr)
			throw std::exception("ClearScreen operation. Cannot dereference non-hosted Traversor.");
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

#pragma region BreadthFirstTraversor

	template<typename T>
	Graph<T>::BreadthFirstTraversor::BreadthFirstTraversor() :
		mOwner(nullptr), mQueue(), mVertexVisitedData()
	{}
	
	template<typename T>
	Graph<T>::BreadthFirstTraversor::BreadthFirstTraversor(const BreadthFirstTraversor& rhs) :
		mOwner(rhs.mOwner), mQueue(rhs.mQueue), mVertexVisitedData(rhs.mVertexVisitedData)
	{}

	template<typename T>
	Graph<T>::BreadthFirstTraversor::BreadthFirstTraversor(BreadthFirstTraversor&& rhs) :
		mOwner(rhs.mOwner), mQueue(std::move(rhs.mQueue)), mVertexVisitedData(std::move(rhs.mVertexVisitedData))
	{
		rhs.mOwner = nullptr;
		mCurrentVertex = nullptr;
	}

	template<typename T>
	typename Graph<T>::BreadthFirstTraversor& Graph<T>::BreadthFirstTraversor::operator=(const BreadthFirstTraversor& rhs)
	{
		if (this != &rhs)
		{
			mOwner = rhs.mOwner;
			mQueue = rhs.mQueue;
			mVertexVisitedData = rhs.mVertexVisitedData;
		}
		return *this;
	}

	template<typename T>
	typename Graph<T>::BreadthFirstTraversor& Graph<T>::BreadthFirstTraversor::operator=(BreadthFirstTraversor&& rhs)
	{
		if (this != &rhs)
		{
			mOwner = rhs.mOwner;
			mQueue = std::move(rhs.mQueue);
			mVertexVisitedData = std::move(rhs.mVertexVisitedData);

			mOwner = nullptr;
		}
		return *this;
	}

	template<typename T>
	bool Graph<T>::BreadthFirstTraversor::HasMoreVertices() const
	{
		return !mQueue.IsEmpty();
	}

	template<typename T>
	void Graph<T>::BreadthFirstTraversor::MoveToNext()
	{
		if (mQueue.IsEmpty())
			throw std::exception("All vertices already traversed.");

		Vertex* currentVertex = mQueue.Front();
		
		for (auto& edge : currentVertex->EdgeList())
		{
			if (!mVertexVisitedData.ContainsKey(edge->GetHead()))
			{
				mQueue.PushBack(edge->GetHead());
				mVertexVisitedData[edge->GetHead()] = false;
			}
		}

		mQueue.PopFront();

		if(!mQueue.IsEmpty())
			mVertexVisitedData[mQueue.Front()] = true;
	}

	template<typename T>
	const T& Graph<T>::BreadthFirstTraversor::CheckUpcomingVertex() const
	{
		if (mQueue.Size() > 1U)
		{
			SList<Vertex*>::Iterator itr = mQueue.begin();
			++itr;
			return *itr;
		}
		else
		{
			throw std::exception("No more vertices left to traverse after the current one.");
		}
	}

	template<typename T>
	T& Graph<T>::BreadthFirstTraversor::operator*()
	{
		if (mOwner == nullptr)
			throw std::exception("ClearScreen operation. Cannot dereference non-hosted Traversor.");
		if(mQueue.IsEmpty())
			throw std::exception("ClearScreen operation. Cannot dereference empty Traversor.");
		return mQueue.Front()->Data();
	}

	template<typename T>
	const T& Graph<T>::BreadthFirstTraversor::operator*() const
	{
		if (mOwner == nullptr)
			throw std::exception("ClearScreen operation. Cannot dereference non-hosted Traversor.");
		if (mQueue.IsEmpty())
			throw std::exception("ClearScreen operation. Cannot dereference empty Traversor.");
		return mQueue.Front()->Data();
	}

	template<typename T>
	Graph<T>::BreadthFirstTraversor::BreadthFirstTraversor(Vertex& currentVertex, const Graph* owner) :
		mOwner(owner), mQueue(), mVertexVisitedData()
	{
		mQueue.PushBack(&currentVertex);
		mVertexVisitedData.Insert(&currentVertex, true);
	}

#pragma endregion

#pragma region DepthFirstTraversor

	template<typename T>
	Graph<T>::DepthFirstTraversor::DepthFirstTraversor() :
		mOwner(nullptr), mVertexStack(), mVertexVisitedData()
	{}

	template<typename T>
	Graph<T>::DepthFirstTraversor::DepthFirstTraversor(const DepthFirstTraversor& rhs) :
		mOwner(rhs.mOwner), mVertexStack(rhs.mVertexStack), mVertexVisitedData(rhs.mVertexVisitedData)
	{}

	template<typename T>
	Graph<T>::DepthFirstTraversor::DepthFirstTraversor(DepthFirstTraversor&& rhs) :
		mOwner(rhs.mOwner), mVertexStack(std::move(rhs.mVertexStack)), mVertexVisitedData(std::move(rhs.mVertexVisitedData))
	{
		rhs.mOwner = nullptr;
		mCurrentVertex = nullptr;
	}

	template<typename T>
	typename Graph<T>::DepthFirstTraversor& Graph<T>::DepthFirstTraversor::operator=(const DepthFirstTraversor& rhs)
	{
		if (this != &rhs)
		{
			mOwner = rhs.mOwner;
			mVertexStack = rhs.mVertexStack;
			mVertexVisitedData = rhs.mVertexVisitedData;
		}
		return *this;
	}

	template<typename T>
	typename Graph<T>::DepthFirstTraversor& Graph<T>::DepthFirstTraversor::operator=(DepthFirstTraversor&& rhs)
	{
		if (this != &rhs)
		{
			mOwner = rhs.mOwner;
			mVertexStack = std::move(rhs.mVertexStack);
			mVertexVisitedData = std::move(rhs.mVertexVisitedData);

			mOwner = nullptr;
		}
		return *this;
	}

	template<typename T>
	bool Graph<T>::DepthFirstTraversor::HasMoreVertices() const
	{
		return !mVertexStack.IsEmpty();
	}

	template<typename T>
	void Graph<T>::DepthFirstTraversor::MoveToNext()
	{
		if (mVertexStack.IsEmpty())
			throw std::exception("ClearScreen operation. Traversal already completed.");

		Vertex* currentVertex = mVertexStack.Top();

		Stack<Vertex*> tempStack;
		for (auto& edge : currentVertex->EdgeList())
		{
			if (!mVertexVisitedData.ContainsKey(edge->GetHead()))
			{
				tempStack.Push(edge->GetHead());
				mVertexVisitedData[edge->GetHead()] = false;
			}
		}

		mVertexStack.Pop();
		while (!tempStack.IsEmpty())
		{
			mVertexStack.Push(tempStack.Top());
			tempStack.Pop();
		}

		if (!mVertexStack.IsEmpty())
			mVertexVisitedData[mVertexStack.Top()] = true;
	}

	template<typename T>
	T& Graph<T>::DepthFirstTraversor::operator*()
	{
		if (mOwner == nullptr)
			throw std::exception("ClearScreen operation. Cannot dereference non-hosted Traversor");
		if(mVertexStack.IsEmpty())
			throw std::exception("ClearScreen operation. Cannot dereference empty Traversor");
		return mVertexStack.Top()->Data();
	}

	template<typename T>
	const T& Graph<T>::DepthFirstTraversor::operator*() const
	{
		if (mOwner == nullptr)
			throw std::exception("ClearScreen operation. Cannot dereference non-hosted Traversor");
		if (mVertexStack.IsEmpty())
			throw std::exception("ClearScreen operation. Cannot dereference empty Traversor");
		return mVertexStack.Top()->Data();
	}

	template<typename T>
	Graph<T>::DepthFirstTraversor::DepthFirstTraversor(Vertex& currentVertex, const Graph* owner) :
		mOwner(owner), mVertexStack(), mVertexVisitedData()
	{
		mVertexStack.Push(&currentVertex);
		mVertexVisitedData.Insert(&currentVertex, true);
	}

#pragma endregion

}