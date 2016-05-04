#pragma once
#include "Vector.h"
#include "SList.h"
#include "Hashmap.h"
#include "Stack.h"

namespace Library
{
	/**
	 *	Template class for a singly linked list
	 */
	template <typename T>
	class Graph
	{
	private:
		class Vertex;
		class Edge
		{
		public:

			/**
			 *	Constructor
			 *	@param reference to head vertex
			 *	@param reference to tail vertex
			 */
			Edge(Vertex& headVertex, Vertex& tailVertex);

			/**
			 *	Copy constructor
			 *	@param reference to the right hand side variable
			 */
			Edge(const Edge& rhs);

			/**
			 *	Move constructor
			 *	@param R value reference to be moved
			 */
			Edge(Edge&& rhs);

			/**
			 *	Copy Assignment operator
			 *	@param reference to the right hand side variable
			 *	@return reference to the copied Edge
			 */
			Edge& operator=(const Edge& rhs);

			/**
			 *	Move Assignment operator
			 *	@param R value reference to the right hand side variable
			 *	@return reference to the moved Edge
			 */
			Edge& operator=(Edge&& rhs);

			/**
			 *	default Destructor
			 */
			~Edge() = default;

			/**
			 *	Getter for the head vertex of the edge
			 *	@return pointer to head vertex
			 */
			Vertex* GetHead() const;

			/**
			 *	Getter for the tail vertex of the edge
			 *	@return pointer to tail vertex
			 */
			Vertex* GetTail() const;

		private:
			Vertex* mHead;
			Vertex* mTail;
		};

		class Vertex
		{
		public:
			/**
			 *	Constructor
			 *	@param data to be saved in the vertex
			 */
			Vertex(const T& vertexData);

			/**
			 *	Copy constructor
			 *	@param reference to the right hand side variable
			 */
			Vertex(const Vertex& rhs);

			/**
			 *	Move constructor
			 *	@param R value reference to be moved
			 */
			Vertex(Vertex&& rhs);

			/**
			 *	Copy Assignment operator
			 *	@param reference to the right hand side variable
			 *	@return reference to the copied Vertex
			 */
			Vertex& operator=(const Vertex& rhs);

			/**
			 *	Move Assignment operator
			 *	@param R value reference to the right hand side variable
			 *	@return reference to the moved Vertex
			 */
			Vertex& operator=(Vertex&& rhs);

			/**
			 *	Destructor for SList
			 */
			~Vertex() = default;

			void AddEdge(Edge& newEdge);
			Vector<Edge*>& EdgeList();

			T& Data();

		private:
			Vector<Edge*> mEdgeList;
			T mData;
		};

	public:

		class Traversor
		{
			friend Graph;
		public:
			Traversor();
			Traversor(const Traversor& rhs);
			Traversor(Traversor&& rhs);
			~Traversor() = default;

			Traversor& operator=(const Traversor& rhs);
			Traversor& operator=(Traversor&& rhs);

			bool operator==(const Traversor& rhs) const;
			bool operator!=(const Traversor& rhs) const;

			bool HasMoreChildren() const;
			T& GetCurrentChildVertex() const;
			bool MoveToNextChild();
			bool TraverseToCurrentChild();
			void ResetChildrenIterator();

			T& operator*();
			const T& operator*() const;

		private:
			Traversor(Vertex& currentVertex, const Graph* owner);

			const Graph* mOwner;
			Vertex* mCurrentVertex;
			std::uint32_t mCurrentChildIndex;
		};

		class BreadthFirstTraversor
		{
			friend Graph;
		public:
			BreadthFirstTraversor();
			BreadthFirstTraversor(const BreadthFirstTraversor& rhs);
			BreadthFirstTraversor(BreadthFirstTraversor&& rhs);
			~BreadthFirstTraversor() = default;

			BreadthFirstTraversor& operator=(const BreadthFirstTraversor& rhs);
			BreadthFirstTraversor& operator=(BreadthFirstTraversor&& rhs);

			bool HasMoreVertices() const;
			void MoveToNext();
			const T& CheckUpcomingVertex() const;
			T& operator*();
			const T& operator*() const;

		private:
			BreadthFirstTraversor(Vertex& currentVertex, const Graph* owner);

			const Graph* mOwner;
			SList<Vertex*> mQueue;
			Hashmap<Vertex*, bool> mVertexVisitedData;
		};

		class DepthFirstTraversor
		{
			friend Graph;
		public:
			DepthFirstTraversor();
			DepthFirstTraversor(const DepthFirstTraversor& rhs);
			DepthFirstTraversor(DepthFirstTraversor&& rhs);
			~DepthFirstTraversor() = default;

			DepthFirstTraversor& operator=(const DepthFirstTraversor& rhs);
			DepthFirstTraversor& operator=(DepthFirstTraversor&& rhs);

			bool HasMoreVertices() const;
			void MoveToNext();
			//const T& CheckUpcomingVertex() const;
			T& operator*();
			const T& operator*() const;

		private:
			DepthFirstTraversor(Vertex& currentVertex, const Graph* owner);

			const Graph* mOwner;
			Stack<Vertex*> mVertexStack;
			Hashmap<Vertex*, bool> mVertexVisitedData;
		};

		/**
		 *	Constructor for SList, initializes an empty list
		 */
		Graph();

		/**
		 *	disallowed copy constructor
		 */
		Graph(const Graph& rhs) = delete;

		/**
		 *	disallowed copy assignment operator
		 */
		Graph<T>& operator=(const Graph& rhs) = delete;

		/**
		 *	Destructor
		 */
		~Graph();

		/**
		 *	adds an isolated vertex with the given data to the graph
		 *	@param data for the vertex
		 *	@return Traversor pointing to the vertex
		 */
		Traversor AddVertex(const T& vertexData);

		/**
		 *	adds a vertex with the given data as a child of the provided parent vertex
		 *	@param data for the vertex
		 *	@param parent vertex traversor
		 *	@return Traversor pointing to the vertex
		 */
		Traversor AddVertex(const T& vertexData, Traversor& parentVertex);

		/**
		 *	adds a vertex with the given data as a parent of the provided vertex
		 *	@param data for the vertex
		 *	@param child vertex traversor
		 *	@return Traversor pointing to the vertex
		 */
		Traversor AddParentVertex(const T& vertexData, Traversor& childVertex);

		/**
		 *	creates an edge between the two vertices
		 *	@param traversor to tail vertex
		 *	@param traversor to head vertex
		 */
		void CreateEdge(Traversor& tailVertex, Traversor& headVertex);

		/**
		 *	Traversor to root node of the graph
		 *	@return traversor
		 */
		Traversor Begin() const;

		/**
		 *	BreadthFirstTraversor to root node of the graph
		 *	@return BreadthFirstTraversor
		 */
		BreadthFirstTraversor BeginBreadthFirstTraversal() const;

		/**
		 *	DepthFirstTraversor to root node of the graph
		 *	@return DepthFirstTraversor
		 */
		DepthFirstTraversor BeginDepthFirstTraversal() const;

		/**
		 *	Is the graph empty?
		 *	@return true if there is no vertex in the graph, false otherwise
		 */
		bool IsEmpty() const;

		/**
		 *	Deletes all the vertices and edges of the graph
		 */
		void Clear();

	private:
		void CreateEdge(Vertex& headVertex, Vertex& tailVertex);

		Vector<Vertex*> mVertexList;
		Vector<Edge*> mEdgeList;
	};

}
#include "Graph.inl"

