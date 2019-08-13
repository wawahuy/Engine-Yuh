#pragma once
#include "Export.h"
#include "YMath.h"
#include <vector>
#include <algorithm>
#include <iostream>

S_NS_PHYSICAL
;

struct PSLNodePair;


/// Node
struct PSLNode {
	/// Giá trị của Node
	int			value;

	/// Node lận cận
	PSLNode*	left;
	PSLNode*	right;
	PSLNode*	parent;

	/// Các node pair nằm bên cây còn lại
	PSLNodePair* rootPair;

	/// Lá
	bool IsLeaf() {
		return left == NULL && right == NULL;
	}

	/// Nhánh có 2 con
	bool HasTwoChild() {
		return left != NULL && right != NULL;
	}
};


/// Pair
struct PSLNodePair {
	PSLNode*	value;

	/// Node lận cận
	PSLNodePair* left;
	PSLNodePair* right;
	PSLNodePair* parent;

	/// Lá
	bool IsLeaf() {
		return left == NULL && right == NULL;
	}


	/// Nhánh có 2 con
	bool HasTwoChild() {
		return left != NULL && right != NULL;
	}
};


///		Pair A:		|1|	|1|	|2|	|4|	|4|	|4|	
///			 B:		|4|	|3|	|4|	|7|	|8|	|9|
///
///		[A Tree]							[B Tree]
///
///			  1  [4, 3]						    4	[1, 2]--------------->     Node Pair Tree
///			/	\							  /	  \										[node Pair]  A-Tree Node [value =  1]
///	 [4]  2		 4 [7, 8, 9]			[1]  3	   7  [4]								      \
///												 /	 \									        [node Pair] A-Tree Node [value = 2]
///											[4]	8	  9 [4]
///
///
/// Áp dụng AVL (Data struct)
/// Cấu trúc này bao gồm 2 cây chính, 2 node trong 2 cây tạo thành 1 pair, và không có pair nào giống nhau
/// Các node chứa cây riêng để quản lý các pair
/// Cây này được áp dụng để lưu trữ các cập va chạm trong broadphase
/// Space	:	2*O(n)
/// Add		:	2*O(log(n))
/// Delete	:     O(log(n)) + x


class YUH_API_PHYSICAL PairList {
public:
	PairList();
	~PairList();

	/// Thêm cặp phần tử
	/// Nếu cặp này đã tồn tại nó sẽ không được thêm nữa
	void Add(int val1, int val2);

	/// Xóa các cặp phận tử 
	/// Bao gồm xóa các Node có giá trị value nằm trên cả 2 cây
	void Remove(int value);

	/// Class Duyệt cây
	class Iterator {
	public:
		yuh::Pair<int, int> Value();
		Iterator			operator++(int);
		Iterator&			operator++();
		bool				operator==(void *ptr);
		bool				operator!=(void *ptr);

	private:
		PSLNode *stack[256];
		int cstack;
	};

	/// Phần tử đầu
	Iterator Query();

	/// RootA
	PSLNode *GetRootA();

	/// RootB
	PSLNode *GetRootB();

	/// Test
	void traverse_binary_tree(PSLNode* n)
	{
		if (n == NULL)     //Cay rong
			return;
		else
		{
			traverse_binary_tree(n->left);					//Duyet cay con trai theo thu tu giua
			std::cout << n->value << "\t";                  //Tham nut
			traverse_binary_tree(n->right);					//Duyet cay con phai theo thu tu giua
		}
	}

private:
	/// Tạo Node
	PSLNode *CreateNode(int value);

	/// Tạo Pair
	PSLNodePair *CreateNodePair(PSLNode *node);

	/// Chèn node vào cây
	PSLNode *InsertNode(PSLNode *&root, int value);

	/// Chèn pair vào node
	bool InsertNodePair(PSLNode *&node, PSLNode *nodePair);

	/// Tìm node trên cây
	bool FindNode(PSLNode **root, PSLNode **&node, int value);
	bool FindNodePair(PSLNodePair **root, PSLNodePair **&node, PSLNode* value);

	/// Xóa node trên cây
	void RemoveNode(PSLNode **m_root, PSLNode **node);
	void RemoveNodePair(PSLNodePair **m_root, PSLNodePair **node);

	/// Xóa node có giá trị value, bao gồm xóa pair và node trên cây tương ứng
	void RemoveNodeAndPair(PSLNode **root, int value);

	/// Cây A
	PSLNode *m_rootA;

	/// Cây B
	PSLNode *m_rootB;
};

E_NS