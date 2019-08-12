#include "../include/PairList.h"

S_NS_PHYSICAL
;

PairList::PairList()
{
	m_rootA = 0;
	m_rootB = 0;
}

PairList::~PairList()
{
}

PSLNode * PairList::GetRootA()
{
	return m_rootA;
}

PSLNode * PairList::GetRootB()
{
	return m_rootB;
}

PSLNode * PairList::CreateNode(int value)
{
	PSLNode *node	= new PSLNode();
	node->value		= value;
	node->left		= NULL;
	node->right		= NULL;
	return node;
}

bool Sort(PSLNode *A, PSLNode *B) {
	return A->value < B->value;
}

void PairList::Add(int val1, int val2)
{
	/// Gán min cho val1
	if (val1 > val2) {
		yuh::Swap(val1, val2);
	}

	/// Insert node vào cây
	bool exist1, exist2;
	PSLNode *nodeVal1 = InsertNode(m_rootA, val1);
	PSLNode *nodeVal2 = InsertNode(m_rootB, val2);

	/// Kiểm tra tồn tại pair
	std::vector<PSLNode *> &lpair1 = nodeVal1->pair;
	std::vector<PSLNode *> &lpair2 = nodeVal2->pair;

	/// Khi đã tồn tại pair
	auto lpend = lpair1.end();
	if (std::find(lpair1.begin(), lpend, nodeVal2) != lpend)
		return;

	/// Khởi tạo pair
	lpair1.push_back(nodeVal2);
	lpair2.push_back(nodeVal1);

	/// Sort pair
	std::sort(lpair1.begin(), lpair1.end(), Sort);
	std::sort(lpair2.begin(), lpair2.end(), Sort);
}

void PairList::Remove(int val1)
{
	PSLNode *node = m_rootA;
	while (node)
	{
		if (node->value == val1) {
			break;
		}
		else if (val1 < node->value)
			node = node->left;
		else
			node = node->right;
	}

}

PairList::Iterator PairList::Query()
{
	return Iterator();
}

PSLNode* PairList::InsertNode(PSLNode *&root, int value)
{
	if (!root) {
		root = CreateNode(value);
		return root;
	}

	PSLNode **node = &root;
	PSLNode  *nodeParent = NULL;

	while (*node)
	{
		nodeParent = *node;

		/// Trả về node đã tồn tại
		if ((*node)->value == value) {
			return *node;
		}

		/// Left
		else if ((*node)->value > value) {
			node = &(*node)->left;
		}
		/// Right
		else {
			node = &(*node)->right;
		}
	}

	/// Tạo Node
	PSLNode* nodeNew = CreateNode(value);
	nodeNew->parent = nodeParent;

	/// Insert Node
	*node = nodeNew;

	return nodeNew;
}



/// ====================================
/// Class Iterator
/// ====================================

yuh::Pair<int, int> PairList::Iterator::Value()
{
	return yuh::Pair<int, int>();
}

PairList::Iterator PairList::Iterator::operator++(int)
{
	return Iterator();
}

PairList::Iterator & PairList::Iterator::operator++()
{
	return *this;
}

bool PairList::Iterator::operator==(void * ptr)
{
	return false;
}

bool PairList::Iterator::operator!=(void * ptr)
{
	return false;
}

E_NS
