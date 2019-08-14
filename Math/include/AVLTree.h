#pragma once
#define NodeNull 0

namespace yuh {

	template<class T> struct AVLNode {
		T   data;
		AVLNode<T>* left;
		AVLNode<T>* right;
		AVLNode<T>* parent;
		int height;
	};

	template<typename T> bool AVLNodePredEqual(const T& a, const T& b) {
		return a == b;
	}

	template<typename T> bool AVLNodePredMin(const T& a, const T& b) {
		return a < b;
	}

	template<
		class T,
		bool (*PredEqual)(const T&, const T&) = AVLNodePredEqual<T>,
		bool (*PredMin  )(const T&, const T&) = AVLNodePredMin<T>
		> 
	class AVLTree {
	public:
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
		void			RemoveNode(AVLNode<T> **node);

		/// Lấy node gốc
		AVLNode<T>*		GetRoot();

	private:
		/// Tạo node
		AVLNode<T>* CreateNode(const T& data);

		/// Node gốc
		AVLNode<T> *m_root;
	};



	template<class T, bool(*PredEqual)(const T &, const T &), bool(*PredMin)(const T &, const T &)>
	inline AVLTree<T, PredEqual, PredMin>::AVLTree()
	{
		m_root = NodeNull;
	}

	template<class T, bool(*PredEqual)(const T &, const T &), bool(*PredMin)(const T &, const T &)>
	inline AVLTree<T, PredEqual, PredMin>::~AVLTree()
	{
		/// Code xóa tấc cả node trên cây
		/// Chưa xây dựng
	}

	template<class T, bool(*PredEqual)(const T &, const T &), bool(*PredMin)(const T &, const T &)>
	inline AVLNode<T>* AVLTree<T, PredEqual, PredMin>::AddValue(const T & value)
	{
		if (!m_root) {
			m_root = CreateNode(value);
			return m_root;
		}

		AVLNode<T> **node = &m_root;
		AVLNode<T>  *nodeParent = NULL;

		while (*node)
		{
			nodeParent = *node;

			/// Trả về node đã tồn tại
			if (PredEqual(nodeParent->data, value)) {
				return nodeParent;
			}

			/// Left
			else if (PredMin(value, nodeParent->data)) {
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

		/// Insert Node
		*node = nodeNew;

		return nodeNew;
	}

	template<class T, bool(*PredEqual)(const T &, const T &), bool(*PredMin)(const T &, const T &)>
	inline bool AVLTree<T, PredEqual, PredMin>::RemoveValue(const T & t)
	{
		AVLNode<T> **node = FindNode(t);

		if (!node)
			return false;

		RemoveNode(node);
	}

	template<class T, bool(*PredEqual)(const T &, const T &), bool(*PredMin)(const T &, const T &)>
	inline AVLNode<T>** AVLTree<T, PredEqual, PredMin>::FindNode(const T & value)
	{
		AVLNode<T> **node = &m_root;
		AVLNode<T>  *cur = *node;

		while (cur) {

			/// Khi node chứa value
			if (PredEqual(cur->data, value)) {
				return node;
			}

			/// Left
			if (PredMin(value, cur->data)) {
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

	template<class T, bool(*PredEqual)(const T &, const T &), bool(*PredMin)(const T &, const T &)>
	inline void AVLTree<T, PredEqual, PredMin>::RemoveNode(AVLNode<T>** node)
	{
		AVLNode<T> *vNode = *node;
		AVLNode<T> *parent = vNode->parent;

		/// Không có lá phải
		if (vNode->right == NULL) {
			/// Con trái
			*node = vNode->left;
		}

		/// Không có lá trái
		else if (vNode->left == NULL) {
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

				/// Tìm địa chỉ của con trỏ giữ node bên phải xâu nhất
				while ((*temp)->left != NodeNull) {
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

		delete vNode;
	}

	template<class T, bool(*PredEqual)(const T &, const T &), bool(*PredMin)(const T &, const T &)>
	inline AVLNode<T>* AVLTree<T, PredEqual, PredMin>::GetRoot()
	{
		return m_root;
	}

	template<class T, bool(*PredEqual)(const T &, const T &), bool(*PredMin)(const T &, const T &)>
	inline AVLNode<T>* AVLTree<T, PredEqual, PredMin>::CreateNode(const T & data)
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