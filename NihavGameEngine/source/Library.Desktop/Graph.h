#pragma once
#include "Vector.h"

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
			*	Constructor for SList, initializes an empty list
			*/
			Edge(Vertex& headVertex, Vertex& tailVertex);

			/**
			*	Copy constructor for SList
			*	@param reference to the right hand side variable
			*/
			Edge(const Edge& rhs);

			Edge(Edge&& rhs);

			/**
			*	Assignment operator overload for SList
			*	@param reference to the right hand side variable
			*	@return reference to the resultant SList
			*/
			Edge& operator=(const Edge& rhs);

			Edge& operator=(Edge&& rhs);

			/**
			*	Destructor for SList
			*/
			~Edge() = default;

			Vertex* GetHead() const;
			Vertex* GetTail() const;

		private:
			Vertex* mHead;
			Vertex* mTail;
		};

		class Vertex
		{
		public:
			/**
			*	Constructor for SList, initializes an empty list
			*/
			Vertex(T& vertexData);

			/**
			 *	Copy constructor for SList
			 *	@param reference to the right hand side variable
			 */
			Vertex(const Vertex& rhs);

			Vertex(Vertex&& rhs);

			/**
			*	Assignment operator overload for SList
			*	@param reference to the right hand side variable
			*	@return reference to the resultant SList
			*/
			Vertex& operator=(const Vertex& rhs);

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

		/**
		 *	Constructor for SList, initializes an empty list
		 */
		Graph();

		/**
		 *	Copy constructor for SList
		 *	@param reference to the right hand side variable
		 */
		Graph(const Graph& rhs) = delete;

		/**
		*	Assignment operator overload for SList
		*	@param reference to the right hand side variable
		*	@return reference to the resultant SList
		*/
		Graph<T>& operator=(const Graph& rhs) = delete;

		/**
		 *	Destructor for SList
		 */
		~Graph();

		Traversor AddVertex(T& vertexData);
		Traversor AddVertex(T& vertexData, Traversor& parentVertex);
		void CreateEdge(Traversor& tailVertex, Traversor& headVertex);

		Traversor Begin() const;

		bool IsEmpty() const;
		void Clear();

	private:
		void CreateEdge(Vertex& headVertex, Vertex& tailVertex);

		Vector<Vertex*> mVertexList;
		Vector<Edge*> mEdgeList;
	};

}
#include "Graph.inl"

