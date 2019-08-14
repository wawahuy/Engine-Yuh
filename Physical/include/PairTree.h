#pragma once
#include "Export.h"
#include <AVLTree.h>
#include <YMath.h>

S_NS_PHYSICAL
;

struct PTNode;

/// So sánh trên AVLNode<PTNode>
bool PTNode_PredEqual(const PTNode& a, const PTNode& b);
bool PTNode_PredMin(const PTNode& a, const PTNode& b);

/// So sánh trên AVLNode<AVLNode<AVLNode>>
bool AVL_PTNode_PredEqual(AVLNode<PTNode>* const &a, AVLNode<PTNode>* const &b);
bool AVL_PTNode_PredMin(AVLNode<PTNode>* const &a, AVLNode<PTNode>* const &b);


/// Node Pair Tree
struct PTNode {
	/// Giá trị 1 phần tử trong pair
	int value;

	/// Danh sách các mối liện kết đến phần tử khác
	AVLTree<AVLNode<PTNode>*, AVL_PTNode_PredEqual, AVL_PTNode_PredMin> pair;
};



/// Pair Tree
/// Chứa các cặp giá trị
class YUH_API_PHYSICAL PairTree {
	typedef AVLTree<PTNode, PTNode_PredEqual, PTNode_PredMin> Tree;

public:
	void Add(int value1, int value2);
	void Remove(int value);

	///Test Query
	AVLNode<PTNode>* GetRoot();

private:
	void RemoveOnTree(Tree &tA, Tree &tB, int value);

	Tree treeA;
	Tree treeB;
};

E_NS