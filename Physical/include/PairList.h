#pragma once
#include "Export.h"
#include "YMath.h"
#include <vector>
#include <algorithm>

S_NS_PHYSICAL
;

/// Node
struct PSLNode {
	/// Giá trị của Node
	int			value;

	/// Node lận cận
	PSLNode*	left;
	PSLNode*	right;
	PSLNode*	parent;

	/// Các node pair nằm bên cây còn lại
	std::vector<PSLNode*> pair;

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
///			  1  [4, 3]						    4	[1, 2] 
///			/	\							  /	  \
///	 [4]  2		 4 [7, 8, 9]			[1]  3	   7  [4]
///												 /	 \
///											[4]	8	  9 [4]
///
///
/// Áp dụng AVL (Data struct), các khóa trùng lập được lưu giữ lại
/// Cấu trúc này bao gồm 2 cây, 2 node trong 2 cây tạo thành 1 pair, và không có pair nào giống nhau
/// Cây này được áp dụng để lưu trữ các cập va chạm trong broadphase


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

	/// Chèn node vào cây
	/// Không chấp nhận chèn nếu cây còn lại chứa node có giá trị 'removeRefValue'
	PSLNode *InsertNode(PSLNode *&root, int value);


	/// Cây A
	PSLNode *m_rootA;

	/// Cây B
	PSLNode *m_rootB;
};

E_NS
