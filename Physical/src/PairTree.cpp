#include "../include/PairTree.h"

S_NS_PHYSICAL
;

bool PTNode_PredEqual(const PTNode & a, const PTNode & b)
{
	return a.value == b.value;
}

bool PTNode_PredMin(const PTNode & a, const PTNode & b)
{
	return a.value < b.value;
}

bool AVL_PTNode_PredEqual(AVLNode<PTNode>* const & a, AVLNode<PTNode>* const & b)
{
	return a->data.value == b->data.value;
}

bool AVL_PTNode_PredMin(AVLNode<PTNode>* const & a, AVLNode<PTNode>* const & b)
{
	return a->data.value < b->data.value;
}



/// ===========================================================


void PairTree::Add(int value1, int value2)
{
	/// Gán min cho val1
	if (value1 > value2) {
		yuh::Swap(value1, value2);
	}

	/// Insert node vào cây
	AVLNode<PTNode> *nodeVal1 = treeA.AddValue({ value1 });
	AVLNode<PTNode> *nodeVal2 = treeB.AddValue({ value2 });

	/// Add pair
	if (!nodeVal1->data.pair.AddValue(nodeVal2))
		return;

	if (!nodeVal2->data.pair.AddValue(nodeVal1))
		return;
}

void PairTree::Remove(int value)
{
	RemoveOnTree(treeA, treeB, value);
	RemoveOnTree(treeB, treeA, value);
}

AVLNode<PTNode>* PairTree::GetRoot()
{
	return treeA.GetRoot();
}

void PairTree::RemoveOnTree(Tree & tA, Tree &tB, int value)
{
	AVLNode<PTNode> **nodeA = NodeNull;

	/// A Tree
	/// Tìm kiếm node có giá trị value
	nodeA = tA.FindNode({ value });
	if (!nodeA) return;

	/// Xóa các node pair
	/// Việc này sẽ xóa các node trên B Tree
	AVLNode<AVLNode<PTNode>*> *stack[256];
	int cstack = 0;
	stack[cstack++] = (*nodeA)->data.pair.GetRoot();

	while (cstack)
	{
		AVLNode<AVLNode<PTNode>*> *node = stack[--cstack];
		if (node) {
			stack[cstack++] = node->left;
			stack[cstack++] = node->right;
			
			auto &treePair = node->data->data.pair;
			treePair.RemoveValue(*nodeA);
			
			delete node;
		}
	}


	/// Xóa node có giá trị value
	tA.RemoveNode(nodeA);
}

E_NS
