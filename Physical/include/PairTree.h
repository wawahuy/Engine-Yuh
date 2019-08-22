#pragma once
#include "Export.h"
#include <AVLTree.h>
#include <YMath.h>

S_NS_PHYSICAL

struct PTNode;

struct CPTNode {
	int value;
	AVLNode<CPTNode> *pair;
	AVLNode<PTNode> *nodeP;
};


/// Node Pair Tree
struct PTNode {
	/// Giá trị 1 phần tử trong pair
	int value;

	/// Danh sách các mối liện kết đến phần tử khác
	AVLTree<CPTNode> pair;
};

inline bool operator==(const PTNode& a, const PTNode& b) {
	return a.value == b.value;
}

inline bool operator<(const PTNode& a, const PTNode& b) {
	return a.value < b.value;
}

inline bool operator==(const CPTNode& a, const CPTNode& b) {
	return a.value == b.value;
}

inline bool operator<(const CPTNode& a, const CPTNode& b) {
	return a.value < b.value;
}

/// Pair Tree
/// Chứa các cặp giá trị
class YUH_API_PHYSICAL PairTree {
	typedef AVLTree<PTNode> Tree;

public:
	void Add(int value1, int value2);
	void Remove(int value);

	Tree& GetTreeA();
	Tree& GetTreeB();

	///Test Query
	AVLNode<PTNode>* GetRoot();

	int m_numPair = 0;

private:
	void RemoveOnTree(Tree &tA, Tree &tB, int value);

	Tree treeA;
	Tree treeB;

};

E_NS
