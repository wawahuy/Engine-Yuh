#include "../include/PairTree2.h"


namespace yuh {
	namespace physicalTest {




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
	AVLNode<CPTNode> *nPair1 = nodeVal1->data.pair.AddValue({value2});
	if (!nPair1) return;

	AVLNode<CPTNode> *nPair2 = nodeVal2->data.pair.AddValue({ value1 });
	if (!nPair2) return;

	nPair1->data.pair = nPair2;
	nPair1->data.nodeP = nodeVal2;

	nPair2->data.pair = nPair1;
	nPair2->data.nodeP = nodeVal1;
}

void PairTree::Remove(int value)
{
	RemoveOnTree(treeA, treeB, value);
	RemoveOnTree(treeB, treeA, value);
}

PairTree::Tree & PairTree::GetTreeA()
{
	return treeA;
}

PairTree::Tree & PairTree::GetTreeB()
{
	return treeB;
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
	AVLNode<CPTNode> *stack[256];
	int cstack = 0;
	stack[cstack++] = (*nodeA)->data.pair.GetRoot();

	while (cstack)
	{
		AVLNode<CPTNode>  *node = stack[--cstack];
		if (node) {
			stack[cstack++] = node->left;
			stack[cstack++] = node->right;

			auto &treePair = node->data.nodeP->data.pair;

			/// Xóa node trên cây Pair ở Node thuộc B
			treePair.RemoveNodeC(node->data.pair);

			/// Xóa node trên cây B
			/// Nếu nó không chứa cặp pair nào khác
			if (treePair.GetRoot() == NodeNull) {
				///treePair.Free();
				tB.RemoveNodeC(node->data.nodeP);
			}

			delete node;
		}
	}


	/// Xóa node có giá trị value
	/// (*nodeA)->data.pair.Free();
	tA.RemoveNode(nodeA);
}

E_NS
