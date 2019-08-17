#pragma once
#include <memory>

#define AVL_INC_CAPACITY_FACTOR 2

namespace yuhTest {

	/// Node AVL
	template<typename T>
	struct AVLNode {
		/// Null
		static const int Null = -1;

		/// parameter 'data' sử dụng 2 toán tử '==' và '<'
		/// Nếu 'T' không có 2 toán tử trên, hãy kế thừa inline function cho nó
		T	data;

		int height;
		int index;
		int parent;
		int left;
		int right;
	};


	/// Cây AVL
	template<typename T>
	class AVLTree {
	public:
		typedef AVLNode<T> Node;

		AVLTree();
		~AVLTree();

	private:
		/// Cập nhật chiều cao, và cân bằng lại cây
		void	UpdateNode(int nodeIndex);

		/// Cân bằng lại node
		void	Balance(int nodeIndex);

		/// Node gốc
		int		m_root;

		/// Danh sách node
		Node*	m_listNode;
		size_t	m_capacityNode;
		size_t	m_numNode;

		/// Danh sách node free
		int*	m_listNodeFree;
		size_t	m_capacityNodeFree;
		size_t	m_numNodeFree;
	};


	template<typename T>
	inline AVLTree<T>::AVLTree()
	{
		m_root = Node::Null;

		/// Allocate node
		m_capacityNode = 100;
		m_numNode = 0;
		m_listNode = (Node*)malloc(sizeof(Node)*m_capacityNode);

		/// Allocate node free
		m_capacityNodeFree = 100;
		m_numNodeFree = 0;
		m_listNodeFree = (int*)malloc(sizeof(int)*m_capacityNodeFree);
	}

	template<typename T>
	inline AVLTree<T>::~AVLTree()
	{
	}

	template<typename T>
	inline void AVLTree<T>::UpdateNode(int nodeIndex)
	{
	}

	template<typename T>
	inline void AVLTree<T>::Balance(int nodeIndex)
	{
	}

}
