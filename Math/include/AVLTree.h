﻿#pragma once
#define NodeNull 0
#include "YMath.h"

namespace yuh {

	/// Node AVL Tree
	template<class T> struct AVLNode {
		T   data;
		AVLNode<T>* left;
		AVLNode<T>* right;
		AVLNode<T>* parent;
		int height;
	};



	/// Cây AVL
	/// Tham sô mẫu T : Kiểu dữ liệu chứa trong mỗi Node
	/// Các tham số mẫu T cần được kế thừa 2 toán tử là '==' và '<' các hàm này tốt nhất nên nội tuyến 'inline'
	template<class T> 
	class AVLTree {

	public:
		typedef AVLNode<T> Node; 

		/// Khởi tạo cây rỗng
		AVLTree();
		~AVLTree();
		
		/// Chèn vào một khóa
		/// Nếu trong cây chưa khóa chèn cây sẽ không chèn mà trả về node chứa khóa đó
		AVLNode<T>*		AddValue(const T& t);

		/// Xóa một khóa
		bool			RemoveValue(const T& t);

		/// Tìm một khóa
		AVLNode<T>**	FindNode(const T& t);

		/// Xóa một node
		/// Con trỏ giữ con trỏ đến node cần xóa
		void			RemoveNode(AVLNode<T> **node);

		/// Xóa một node
		/// Địa chỉ node cần xóa
		void			RemoveNodeC(AVLNode<T> *node);

		/// Lấy node gốc
		AVLNode<T>*		GetRoot();

		/// Lấy Balance max
		int GetBalanceMax();

	private:
		/// Cập nhật chiều cao của node
		void UpdateHeight(AVLNode<T> *node);

		/// Balance
		AVLNode<T>* Balance(AVLNode<T> *node);

		/// Tạo node
		AVLNode<T>* CreateNode(const T& data);

		/// Node gốc
		AVLNode<T> *m_root;
	};



	template<class T>
	inline AVLTree<T>::AVLTree()
	{
		m_root = NodeNull;
	}

	template<class T>
	inline AVLTree<T>::~AVLTree()
	{
		/// Code xóa tấc cả node trên cây
		/// Chưa xây dựng
	}

	template<class T>
	inline AVLNode<T>* AVLTree<T>::AddValue(const T & value)
	{
		if (!m_root) {
			m_root = CreateNode(value);
			return m_root;
		}

		AVLNode<T> **node = &m_root;
		AVLNode<T>  *nodeParent = NodeNull;

		while (*node)
		{
			nodeParent = *node;

			/// Trả về node đã tồn tại
			if (nodeParent->data == value) {
				return nodeParent;
			}

			/// Left
			else if (value < nodeParent->data) {
				node = &nodeParent->left;
			}
			/// Right
			else {
				node = &nodeParent->right;
			}

		}

		/// Tạo Node
		AVLNode<T>* nodeNew = CreateNode(value);
		nodeNew->parent = nodeParent;

		/// Gán Node
		*node = nodeNew;

		/// Cập nhât Height
		UpdateHeight(nodeParent);

		return nodeNew;
	}

	template<class T>
	inline bool AVLTree<T>::RemoveValue(const T & t)
	{
		AVLNode<T> **node = FindNode(t);

		if (!node)
			return false;

		RemoveNode(node);
	}

	template<class T>
	inline AVLNode<T>** AVLTree<T>::FindNode(const T & value)
	{
		AVLNode<T> **node = &m_root;
		AVLNode<T>  *cur = *node;

		while (cur) {

			/// Khi node chứa value
			if (cur->data == value) {
				return node;
			}

			/// Left
			if (value < cur->data) {
				node = &cur->left;
			}

			/// Right
			else {
				node = &cur->right;
			}

			/// Value
			cur = *node;
		}
		return NodeNull;
	}

	template<class T>
	inline void AVLTree<T>::RemoveNode(AVLNode<T>** node)
	{
		AVLNode<T> *vNode = *node;
		AVLNode<T> *parent = vNode->parent;
		AVLNode<T> *nodeNeedUpdate = parent;

		/// Không có lá phải
		if (vNode->right == NodeNull) {
			/// Con trái
			*node = vNode->left;
		}

		/// Không có lá trái
		else if (vNode->left == NodeNull) {
			/// Con phải
			*node = vNode->right;
		}

		/// Có 2 con
		else {
			/// Node bên phải của node cần xóa
			AVLNode<T> *nodeRight = vNode->right;
			AVLNode<T> *nodeRightLeft = nodeRight->left;

			/// Nếu node bên Trái của node bên phải của node cần xóa là NULL => Dồn Node phải lên
			if (!nodeRightLeft) {
				*node = nodeRight;
				(*node)->left = vNode->left;
				vNode->left->parent = *node;
			}
			else {
				/// Địa chỉ con trỏ giữ node bên trái của node bên phải node cần xóa
				AVLNode<T> **temp = &nodeRight->left;
				nodeNeedUpdate = nodeRight;

				/// Tìm địa chỉ của con trỏ giữ node bên phải xâu nhất
				while ((*temp)->left != NodeNull) {
					nodeNeedUpdate = nodeRight->left;
					temp = &((*temp)->left);
				}

				/// Node cần swap
				*node = *temp;

				/// Xóa liên kết với temp với parent
				*temp = (*temp)->right;
				if (*temp) {
					(*temp)->parent = (*node)->parent;
				}

				/// Gán left, right
				(*node)->left = vNode->left;
				(*node)->right = vNode->right;
				vNode->left->parent = *node;
				vNode->right->parent = *node;
			}
		}

		/// Gán cha
		if (*node) {
			(*node)->parent = parent;
		}

		/// Cập nhật chiều cao
		UpdateHeight(nodeNeedUpdate);

		delete vNode;
	}

	template<class T>
	inline void AVLTree<T>::RemoveNodeC(AVLNode<T>* node)
	{
		AVLNode<T> *parent = node->parent;
		if (parent) {
			RemoveNode(&(parent->left == node ? parent->left : parent->right));
		}
		/// Xóa node là node gốc
		else {
			RemoveNode(&m_root);
		}
	}

	template<class T>
	inline AVLNode<T>* AVLTree<T>::GetRoot()
	{
		return m_root;
	}

	template<class T>
	inline int AVLTree<T>::GetBalanceMax()
	{
		int balanceMax = 0;
		AVLNode<T>* stack[2560];
		int cstack = 0;
		stack[cstack++] = m_root;

		while (cstack) {
			AVLNode<T>* node = stack[--cstack];
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

	template<class T>
	inline void AVLTree<T>::UpdateHeight(AVLNode<T>* node)
	{
		while (node)
		{
			/// Chiều cao cây con
			int heightLeft	= node->left  ? node->left->height  : 0;
			int heightRight = node->right ? node->right->height : 0;
			int newHeight = 1 + yuh::max(heightLeft, heightRight);

			/// Khi chiều cao không thay đổi giữ nguyên
			if (newHeight == node->height)
				break;

			/// Cập nhật chiều cao
			node->height = newHeight;

			/// Cân bằng
			if(yuh::abs(heightLeft-heightRight) > 1)
				node = Balance(node);

			/// Node cha
			node = node->parent;
		}
	}

	template<class T>
	inline AVLNode<T>* AVLTree<T>::Balance(AVLNode<T>* nodeA)
	{
		AVLNode<T> *nodeB = nodeA->left;
		AVLNode<T> *nodeC = nodeA->right;

		int heightB  = nodeB ? nodeB->height : 0;
		int heightC  = nodeC ? nodeC->height : 0;
		int u = heightC - heightB;

		/// Rotate nodeC Up
		if (u > 1) {
			/// Node cha
			AVLNode<T> *nodeParentA = nodeA->parent;

			/// Tìm con trỏ giữ node A
			AVLNode<T> **ptrPtrA;
			if (nodeParentA) {
				ptrPtrA = &(nodeParentA->left == nodeA ? nodeParentA->left : nodeParentA->right);
			}
			else
			{
				ptrPtrA = &m_root;
			}

			/// Chuyển node nhánh của node cha của A sang node C
			*ptrPtrA = nodeC;

			/// Gán cha trên NodeC
			nodeC->parent = nodeParentA;

			/// Giữ node trái của nodeC
			AVLNode<T> *nodeCLeft = nodeC->left;

			/// Chuyển NodeA xuống node trái C
			nodeC->left = nodeA;
			nodeA->parent = nodeC;

			/// Chuyển node trái cũ của nodeC sang làm node con của node phải A
			nodeA->right = nodeCLeft;
			if(nodeCLeft)
				nodeCLeft->parent = nodeA;

			/// Update Height nodeA
			int heightLeft = nodeA->left ? nodeA->left->height : 0;
			int heightRight = nodeA->right ? nodeA->right->height : 0;
			nodeA->height = 1 + yuh::max(heightLeft, heightRight);

			/// Update Height nodeC
			heightLeft = nodeC->left ? nodeC->left->height : 0;
			heightRight = nodeC->right ? nodeC->right->height : 0;
			nodeC->height = 1 + yuh::max(heightLeft, heightRight);


			/// Return
			nodeA = nodeC;
		}
		/// Rotate nodeB Up
		else if (u < -1) {
			/// Node cha
			AVLNode<T> *nodeParentA = nodeA->parent;

			/// Tìm con trỏ giữ node A
			AVLNode<T> **ptrPtrA;
			if (nodeParentA) {
				ptrPtrA = &(nodeParentA->left == nodeA ? nodeParentA->left : nodeParentA->right);
			}
			else
			{
				ptrPtrA = &m_root;
			}

			/// Chuyển node nhánh của node cha của A sang node B
			*ptrPtrA = nodeB;

			/// Gán cha trên NodeB
			nodeB->parent = nodeParentA;

			/// Giữ node trái của nodeB
			AVLNode<T> *nodeBRight = nodeB->right;

			/// Chuyển NodeA xuống node phai B
			nodeB->right = nodeA;
			nodeA->parent = nodeB;

			/// Chuyển node phai cũ của nodeB sang làm node con của node trái A
			nodeA->left = nodeBRight;
			if (nodeBRight)
				nodeBRight->parent = nodeA;

			/// Update Height nodeA
			int heightLeft = nodeA->left ? nodeA->left->height : 0;
			int heightRight = nodeA->right ? nodeA->right->height : 0;
			nodeA->height = 1 + yuh::max(heightLeft, heightRight);

			/// Update Height nodeC
			heightLeft = nodeB->left ? nodeB->left->height : 0;
			heightRight = nodeB->right ? nodeB->right->height : 0;
			nodeB->height = 1 + yuh::max(heightLeft, heightRight);


			/// Return
			nodeA = nodeB;
		}

		return nodeA;
	}


	template<class T>
	inline AVLNode<T>* AVLTree<T>::CreateNode(const T & data)
	{
		AVLNode<T> *node = new AVLNode<T>();
		node->data = data;
		node->left = NodeNull;
		node->right = NodeNull;
		node->parent = NodeNull;
		node->height = 0;
		return node;
	}

}