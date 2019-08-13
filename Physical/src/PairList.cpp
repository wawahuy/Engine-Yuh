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

bool PairList::FindNodePair(PSLNodePair ** root, PSLNodePair **& node, PSLNode * value)
{
	node = root;
	PSLNodePair *cur = *node;

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
	PSLNode **nodeA = NULL;
	PSLNode **rootB = *root == m_rootA ? &m_rootB : &m_rootA;


	/// A Tree
	/// Tìm kiếm node có giá trị value
	if (!FindNode(root, nodeA, value)) return;

	/// Xóa các node pair
	/// Việc này sẽ xóa các node trên B Tree
	int cstack = 0;
	PSLNodePair* stack[256];
	stack[cstack++] = (*nodeA)->rootPair;

	while (cstack)
	{
		PSLNodePair *pairOnA = stack[--cstack];

		if (pairOnA) {

			/// Thêm vào stack
			stack[cstack++] = pairOnA->left;
			stack[cstack++] = pairOnA->right;

			/// Node trên B
			PSLNode *nodeB = pairOnA->value;

			/// Thực hiện tìm Pair trên B
			PSLNodePair **pairOnB;
			FindNodePair(&nodeB->rootPair, pairOnB, *nodeA);
			RemoveNodePair(&nodeB->rootPair, pairOnB);

			/// Thực hiện xóa node trên B nếu rỗng Pair
			if (!nodeB->rootPair) {
				PSLNode *prB = nodeB->parent;
				if (prB) {
					PSLNode **ndR = prB->left == nodeB ? &prB->left : &prB->right;
					RemoveNode(rootB, ndR);
				}
				else {
					RemoveNode(rootB, &nodeB);
				}
			}

			/// Xóa pair trên A
			delete pairOnA;
		}
	}


	/// Xóa node có giá trị value
	RemoveNode(root, nodeA);
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
		else {
			/// Địa chỉ con trỏ giữ node bên trái của node bên phải node cần xóa
			PSLNode **temp = &nodeRight->left;

			/// Tìm địa chỉ của con trỏ giữ node bên phải xâu nhất
			while ((*temp)->left != NULL) {
				temp = &((*temp)->left);
			}

			/// Node cần swap
			*node = *temp;
			
			/// Xóa liên kết với temp với parent
			*temp = (*node)->left;
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

void PairList::RemoveNodePair(PSLNodePair ** m_root, PSLNodePair ** node)
{
	PSLNodePair *vNode = *node;
	PSLNodePair *parent = vNode->parent;

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
		PSLNodePair *nodeRight = vNode->right;
		PSLNodePair *nodeRightLeft = nodeRight->left;

		/// Nếu node bên Trái của node bên phải của node cần xóa là NULL => Dồn Node phải lên
		if (!nodeRightLeft) {
			*node = nodeRight;
			(*node)->left = vNode->left;
			vNode->left->parent = *node;
		}
		else {
			/// Địa chỉ con trỏ giữ node bên trái của node bên phải node cần xóa
			PSLNodePair **temp = &nodeRight->left;

			/// Tìm địa chỉ của con trỏ giữ node bên phải xâu nhất
			while ((*temp)->left != NULL) {
				temp = &((*temp)->left);
			}

			/// Node cần swap
			*node = *temp;

			/// Xóa liên kết với temp với parent
			*temp = (*node)->left;
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