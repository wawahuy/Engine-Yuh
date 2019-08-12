#pragma once
#include "Export.h"
#include "YMath.h"
#include <vector>
#include <algorithm>

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
};


///		Pair A:		|1|	|1|	|2|	|4|	|4|	|4|	
///			 B:		|4|	|3|	|4|	|7|	|8|	|9|
///
///		[A Tree]							[B Tree]
///
///			  1  [4, 3]						    4	[1, 2]--------------->     Node Pair Tree
///			/	\							  /	  \										1
///	 [4]  2		 4 [7, 8, 9]			[1]  3	   7  [4]								 \
///												 /	 \									  2
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
	/// Bao gồm xóa các Node nằm trên cả 2 cây
	void Remove(int val1);

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

private:
	/// Tạo Node
	PSLNode *CreateNode(int value);

	/// Tạo Pair
	PSLNodePair *CreateNodePair(PSLNode *node);

	/// Chèn node vào cây
	PSLNode *InsertNode(PSLNode *&root, int value);

	/// Chèn pair vào node
	bool InsertNodePair(PSLNode *&node, PSLNode *nodePair);

	/// Xóa node
	void RemoveLeaf(PSLNode *node);

	/// Cây A
	PSLNode *m_rootA;

	/// Cây B
	PSLNode *m_rootB;
};

E_NS
