#pragma once
#define NodeNull 0
#include "YMath.h"
#include <memory>

namespace yuhTest {

	/// Node AVL Tree
	template<class T, class U> struct AVLNode {
		T	key;
		U   data;
		AVLNode<T, U>* left;
		AVLNode<T, U>* right;
		AVLNode<T, U>* parent;
		int height;
	};



	/// Cây AVL
	/// Tham sô mẫu T : Kiểu dữ liệu chứa trong mỗi Node
	/// Các tham số mẫu T cần được kế thừa 2 toán tử là '==' và '<' các hàm này tốt nhất nên nội tuyến 'inline'
	template<class T, class U>
	class AVLTree {

	public:
		typedef AVLNode<T, U>	Node;
		typedef AVLNode<T, U>*	NodePtr;
		typedef AVLNode<T, U>** NodePtr2;

		/// Khởi tạo cây rỗng
		AVLTree();
		~AVLTree();

		/// Chèn vào một khóa
		/// Nếu trong cây chưa khóa chèn cây sẽ không chèn mà trả về node chứa khóa đó
		NodePtr			AddKey(const T& key, const U& data);

		/// Xóa một khóa
		bool			RemoveKey(const T& key);

		/// Tìm một khóa
		NodePtr2		FindNode(const T& key);

		/// Xóa một node
		/// Con trỏ giữ con trỏ đến node cần xóa
		void			RemoveNode(NodePtr2 node);

		/// Xóa một node
		/// Địa chỉ node cần xóa
		void			RemoveNode(NodePtr node);

		/// Lấy node gốc
		NodePtr			GetRoot();

		/// Lấy Balance max
		int GetBalanceMax();


		/// Giải phóng toàn bộ cây
		void Free();

	private:
		/// Cập nhật chiều cao của node
		void UpdateNode(NodePtr node);

		/// Balance
		NodePtr Balance(NodePtr node);

		/// Tính toán lại chiều cao của node
		void ComputeHeight(NodePtr node);

		/// Tạo node
		NodePtr CreateNode(const T& key, const U& data);

		/// Node gốc
		NodePtr m_root;
	};


	template<class T, class U>
	inline AVLTree<T, U>::AVLTree()
	{
		m_root = NodeNull;
	}

	template<class T, class U>
	inline AVLTree<T, U>::~AVLTree()
	{
	}

	template<class T, class U>
	inline AVLNode<T, U>* AVLTree<T, U>::AddKey(const T & key, const U & data)
	{
		if (!m_root) {
			m_root = CreateNode(key, data);
			return m_root;
		}

		NodePtr2 node = &m_root;
		NodePtr	 nodeParent = NodeNull;

		while (*node)
		{
			nodeParent = *node;

			/// Trả về node đã tồn tại
			if (nodeParent->key == key) {
				return nodeParent;
			}

			/// Left
			else if (key < nodeParent->key) {
				node = &nodeParent->left;
			}
			/// Right
			else {
				node = &nodeParent->right;
			}

		}

		/// Tạo Node
		NodePtr nodenew = CreateNode(key, data);
		nodenew->parent = nodeParent;

		/// Gán Node
		*node = nodenew;

		/// Cập nhât Height
		UpdateNode(nodeParent);

		return nodenew;
	}

	template<class T, class U>
	inline void yuhTest::AVLTree<T, U>::UpdateNode(NodePtr node)
	{
		while (node)
		{
			/// Cân bằng cây
			node = Balance(node);

			/// Cân bằng lại node cha
			node = node->parent;
		}
	}

	template<class T, class U>
	inline AVLNode<T, U>* AVLTree<T, U>::Balance(NodePtr nodeA)
	{
		NodePtr nodeB = nodeA->left;
		NodePtr nodeC = nodeA->right;
		
		int heightB = nodeB ? nodeB->height : 0;
		int heightC = nodeC ? nodeC->height : 0;
		int u = heightC - heightB;

		/// Lệch phải
		if (u > 1) {
			NodePtr nodeCB = nodeC->left;
			NodePtr nodeCC = nodeC->right;

			int heightCB = nodeCB ? nodeCB->height : 0;
			int heightCC = nodeCC ? nodeCC->height : 0;
			
			/// Lệch phải - trái
			if (heightCC < heightCB) {
				/// Xoay Phải NodeC
				/// Lưu tạm con phải của NodeCB
				NodePtr nodeCBRight = nodeCB->right;

				/// Chuyển nodeCB làm node cha NodeC
				nodeCB->right = nodeC;
				nodeC->parent = nodeCB;

				/// Đổi con phải NodeCB thành con trái NodeC
				nodeC->left = nodeCBRight;
				if (nodeCBRight) {
					nodeCBRight->parent = nodeC;
					
					/// Cập nhật height cho nodeC
					ComputeHeight(nodeC);
				}

				/// Cập nhật height cho nodeCB
				ComputeHeight(nodeCB);

				/// Gán lại địa chỉ node
				nodeC = nodeCB;
				nodeCB = nodeC->left;
			}

			/// Lệch phải - phải
			/// Lưu tạm ptr giữ ptr nodeA trên cây
			NodePtr  parentNodeA = nodeA->parent;
			NodePtr2 ptr2NodeA;

			/// Khi node cha là node gốc
			if (!nodeA->parent) {
				ptr2NodeA = &m_root;
			}
			/// Tìm nhánh trái hay phải giữ ptr nodeA
			else {
				ptr2NodeA = parentNodeA->left == nodeA ? &parentNodeA->left : &parentNodeA->right;
			}

			/// Xoay Trái NodeA
			nodeC->left = nodeA;
			nodeA->parent = nodeC;

			/// Gán node con tráu nodeC thành con phải nodeA
			nodeA->right = nodeCB;
			if (nodeCB) {
				nodeCB->parent = nodeA;

				/// Cập nhật height cho nodeA
				ComputeHeight(nodeA);
			}

			/// Cập nhật height cho nodeC
			ComputeHeight(nodeC);

			/// Gán lại địa chỉ cho nodeA
			*ptr2NodeA = nodeC;
			nodeC->parent = parentNodeA;

			return nodeC;
		}

		/// Lệch trái
		else if (u < -1) {
			NodePtr nodeBB = nodeB->left;
			NodePtr nodeBC = nodeB->right;

			int heightBB = nodeBB ? nodeBB->height : 0;
			int heightBC = nodeBC ? nodeBC->height : 0;

			/// Lệch trái - phải
			if (heightBC > heightBB) {
				/// Xoay Trái NodeB
				/// Lưu tạm con trái của NodeBC
				NodePtr nodeBCLeft = nodeBC->left;

				/// Chuyển nodeBC Up làm node cha NodeB
				nodeBC->left = nodeB;
				nodeB->parent = nodeBC;

				/// Đổi con trái NodeBC thành con phải NodeB
				nodeB->right = nodeBCLeft;
				if (nodeBCLeft) {
					nodeBCLeft->parent = nodeB;

					/// Cập nhật height cho nodeC
					ComputeHeight(nodeB);
				}

				/// Cập nhật height cho nodeCB
				ComputeHeight(nodeBC);

				/// Gán lại địa chỉ node
				nodeB = nodeBC;
				nodeBC = nodeB->right;
			}

			/// Lệch trái - trái
			/// Lưu tạm ptr giữ ptr nodeA trên cây
			NodePtr  parentNodeA = nodeA->parent;
			NodePtr2 ptr2NodeA;

			/// Khi node cha là node gốc
			if (!nodeA->parent) {
				ptr2NodeA = &m_root;
			}
			/// Tìm nhánh trái hay phải giữ ptr nodeA
			else {
				ptr2NodeA = parentNodeA->left == nodeA ? &parentNodeA->left : &parentNodeA->right;
			}

			/// Xoay Phải NodeA
			nodeB->right = nodeA;
			nodeA->parent = nodeB;

			/// Gán node con phải nodeC thành con trái nodeA
			nodeA->left = nodeBC;
			if (nodeBC) {
				nodeBC->parent = nodeA;

				/// Cập nhật height cho nodeA
				ComputeHeight(nodeA);
			}

			/// Cập nhật height cho nodeB
			ComputeHeight(nodeB);

			/// Gán lại địa chỉ cho nodeA
			*ptr2NodeA = nodeB;
			nodeB->parent = parentNodeA;

			return nodeB;
		}


		/// Khi node có nhánh cân bằng
		/// Cập nhật lai chiều cao cây
		nodeA->height = 1 + yuh::max(heightB, heightC);

		return nodeA;
	}

	template<class T, class U>
	inline void AVLTree<T, U>::ComputeHeight(NodePtr node)
	{
		node->height =
			1 + yuh::max(
				node->left  ? node->left->height  : 0,
				node->right ? node->right->height : 0
			);
	}


	template<class T, class U>
	inline AVLNode<T, U>* AVLTree<T, U>::CreateNode(const T & key, const U& data)
	{
		NodePtr node	= new Node();
		node->key		= key;
		node->data		= data;
		node->left		= NodeNull;
		node->right		= NodeNull;
		node->parent	= NodeNull;
		node->height	= 0;
		return node;
	}

	template<class T, class U>
	inline AVLNode<T, U>* AVLTree<T, U>::GetRoot() {
		return m_root;
	}

	template<class T, class U>
	inline int AVLTree<T, U>::GetBalanceMax()
	{
		int balanceMax = 0;
		NodePtr stack[2560];
		int cstack = 0;
		stack[cstack++] = m_root;

		while (cstack) {
			AVLNode<T, U>* node = stack[--cstack];
			if (node != NodeNull) {
				int l = node->left ? node->left->height : 0;
				int r = node->right ? node->right->height : 0;
				int balance = l - r;
				if (yuh::abs(balance) > yuh::abs(balanceMax))
					balanceMax = balance;
				stack[cstack++] = node->left;
				stack[cstack++] = node->right;
			}
		}

		return balanceMax;
	}
}