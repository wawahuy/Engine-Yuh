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

PSLNodePair * PairList::CreateNodePair(PSLNode * value)
{
	PSLNodePair *node = new PSLNodePair();
	node->value = value;
	node->left = NULL;
	node->right = NULL;
	return node;
}

void PairList::Add(int val1, int val2)
{
	/// Gán min cho val1
	if (val1 > val2) {
		yuh::Swap(val1, val2);
	}

	/// Insert node vào cây
	PSLNode *nodeVal1 = InsertNode(m_rootA, val1);
	PSLNode *nodeVal2 = InsertNode(m_rootB, val2);

	/// Kiểm tra tồn tại pair
	if (!InsertNodePair(nodeVal1, nodeVal2))
		return;

	if (!InsertNodePair(nodeVal2, nodeVal1))
		return;
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
		if (nodeParent->value == value) {
			return nodeParent;
		}

		/// Left
		else if (nodeParent->value > value) {
			node = &nodeParent->left;
		}
		/// Right
		else {
			node = &nodeParent->right;
		}
	}

	/// Tạo Node
	PSLNode* nodeNew = CreateNode(value);
	nodeNew->parent = nodeParent;

	/// Insert Node
	*node = nodeNew;

	return nodeNew;
}

bool PairList::InsertNodePair(PSLNode *& nodeRoot, PSLNode * nodePair)
{
	if (!nodeRoot->rootPair) {
		nodeRoot->rootPair = CreateNodePair(nodePair);
		return true;
	}

	PSLNodePair **node = &nodeRoot->rootPair;
	PSLNodePair  *nodeParent = NULL;

	while (*node)
	{
		nodeParent = *node;

		/// Trả về node đã tồn tại
		if (nodeParent->value == nodePair) {
			return false;
		}

		/// Left
		else if (nodeParent->value->value > nodePair->value) {
			node = &nodeParent->left;
		}
		/// Right
		else {
			node = &nodeParent->right;
		}
	}

	/// Tạo Node
	PSLNodePair* nodeNew = CreateNodePair(nodePair);
	nodeNew->parent = nodeParent;

	/// Insert Node
	*node = nodeNew;

	return nodeNew;
}

PairList::Iterator PairList::Query()
{
	return Iterator();
}

bool PairList::FindNode(PSLNode **root, PSLNode **&node, int value)
{
	node = root;
	PSLNode *cur = *node;

	while (cur) {

		/// Khi node chứa value
		if (cur->value == value) {
			return true;
		}

		/// Left
		if (value < cur->value) {
			node = &cur->left;
		}

		/// Right
		else {
			node = &cur->right;
		}

		/// Value
		cur = *node;
	}
	return false;
}

void PairList::Remove(int value)
{
	RemoveNodeAndPair(&m_rootA, value);
	RemoveNodeAndPair(&m_rootB, value);
}

void PairList::RemoveNodeAndPair(PSLNode **root, int value)
{
	PSLNode **node = NULL;

	/// A Tree
	/// Tìm kiếm node có giá trị value
	if (!FindNode(root, node, value)) return;

	/// Xóa các node pair
	/// Việc này sẽ xóa các node trên B Tree


	/// Xóa node có giá trị value
	RemoveNode(root, node);
}

void PairList::RemoveNode(PSLNode **root, PSLNode **node)
{
	PSLNode *vNode = *node;
	PSLNode *parent = vNode->parent;

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

	/// Có 2 lá
	else {
		/// Node bên phải của node cần xóa
		PSLNode *nodeRight = vNode->right;
		PSLNode *nodeRightLeft = nodeRight->left;

		/// Nếu node bên Trái của node bên phải của node cần xóa là NULL => Dồn Node phải lên
		if (!nodeRightLeft) {
			*node = nodeRight;
			(*node)->left = vNode->left;
			vNode->left->parent = *node;
		}
		else if (!nodeRightLeft->left) {
			PSLNode *temp = nodeRightLeft->right;

			/// Chuyển node phải trái lên đổi chô nút xóa
			*node = nodeRightLeft;
			(*node)->left = vNode->left;
			(*node)->right = vNode->right;
			vNode->left->parent = *node;
			vNode->right->parent = *node;

			nodeRight->left = temp;

			if (temp) {
				/// CHuyển node phải trái phải lên đổi nút di chuyên
				temp->parent = nodeRight;
			}
		}
		else {
			/// Địa chỉ con trỏ giữ node bên trái của node bên phải node cần xóa
			PSLNode **temp = &nodeRight->left;

			/// Tìm địa chỉ của con trỏ giữ node bên phải xâu nhất
			while ((*temp)->right != NULL) {
				temp = &((*temp)->right);
			}

			/// Node cần swap
			*node = *temp;
			
			/// Xóa liên kết với temp với parent
			*temp = NULL;

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