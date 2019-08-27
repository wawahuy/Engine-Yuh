#include "../../include/World/Broadphase.h"

S_NS_PHYSICAL
;

Broadphase::Broadphase()
{
	m_root = BPNode::Null;
}

Broadphase::~Broadphase()
{
}

void Broadphase::ComputeFatAABB(BPNode *node)
{
	node->aabb = node->userdata->getAABB();
	node->aabb.min.x -= MARGIN_PX_AABB;
	node->aabb.min.y -= MARGIN_PX_AABB;
	node->aabb.max.x += MARGIN_PX_AABB;
	node->aabb.max.y += MARGIN_PX_AABB;

	Vec2f vel = node->userdata->m_veclocity*((float)MARGIN_FACTOR_VELOCITY_AABB);

	if (vel.x > 0)
		node->aabb.max.x += vel.x;
	else
		node->aabb.min.x += vel.x;

	if (vel.y > 0)
		node->aabb.max.y += vel.y;
	else
		node->aabb.min.y += vel.y;
}

void Broadphase::Add(ICollider * object)
{
	/// Khởi tạo và gán userdata vào node
	BPNode* node	= CreateNode();
	node->userdata	= object;

	object->m_nodeIndex = node->index;

	/// Tính toán và vỗ béo aabb
	ComputeFatAABB(node);

	m_listMove.push_back(node->index);

	/// Thêm lá vào cây
	if (m_root == BPNode::Null) {
		m_root = node->index;
	}
	else {
		InsertNode(node->index);
	}
}

BPNode * Broadphase::CreateNode()
{
	//Khởi tạo node rỗng
	BPNode* node	= new BPNode();
	node->index		= m_listNode.size();
	node->right		= BPNode::Null;
	node->left		= BPNode::Null;
	node->parent	= BPNode::Null;
	node->height	= 0;

	//Thêm vào danh sách Node
	m_listNode.push_back(node);

	return node;
}

void Broadphase::InsertNode(int indexInsert, int indexBranch)
{
	/// Tìm node cha để insert node lá
	BPNode *nodeInsert = m_listNode[indexInsert];
	BPNode *node = nullptr;
	int index = m_root;

	while (index != BPNode::Null)
	{
		node = m_listNode[index];

		/// Dừng khi đến node lá
		if (node->IsLeaf()) {
			break;
		}
			
		BPNode* nodeLeft = m_listNode[node->left];
		BPNode* nodeRight = m_listNode[node->right];
		float vol1 = nodeInsert->aabb.Combine(nodeLeft->aabb).Area() - nodeLeft->aabb.Area();
		float vol2 = nodeInsert->aabb.Combine(nodeRight->aabb).Area() - nodeRight->aabb.Area();

		/// Lựa chọn nhánh có diện tính nhỏ nhất
		if (vol1 < vol2) {
			index = node->left;
		}
		else {
			index = node->right;
		}
	}

	/// Tiến hành khởi tạo node nhánh
	BPNode* nodeBranch;

	if (indexBranch == BPNode::Null) {
		nodeBranch	= CreateNode();
		indexBranch = nodeBranch->index;
	}
	else {
		nodeBranch  = m_listNode[indexBranch];
	}


	/// Khi cây chỉ có 1 Node
	if (m_root == index) {
		m_root = indexBranch;
		nodeBranch->parent = BPNode::Null;
	}
	else {
		BPNode* oldParent = m_listNode[node->parent];

		if (oldParent->left == node->index) {
			oldParent->left  = indexBranch;
		}
		else {
			oldParent->right = indexBranch;
		}

		nodeBranch->parent = oldParent->index;
	}

	/// Thêm lá cho node nhánh
	nodeBranch->left  = indexInsert;
	nodeBranch->right = index;

	/// Cập nhật node cha trên 2 node lá
	node->parent = indexBranch;
	nodeInsert->parent = indexBranch;


	///// Tính lại chiều cao, và AABB trên nhánh
	RebuildBottomUp(nodeBranch->index);
}

void Broadphase::RebuildBottomUp(int index)
{
	BPNode *node;

	while (index != BPNode::Null)
	{
		node  = Balance(index);
		index = node->parent;
	}
}

BPNode* Broadphase::Balance(int iA)
{
	///			A
	///		   / \
	///		  B	  C
	BPNode *nodeA = m_listNode[iA];

	int iB = nodeA->left;
	int iC = nodeA->right;

	BPNode *nodeB = m_listNode[iB];
	BPNode *nodeC = m_listNode[iC];
	int u = nodeC->height - nodeB->height;

	if (u > 1) {
		BPNode *nodeCB = m_listNode[nodeC->left];
		BPNode *nodeCC = m_listNode[nodeC->right];
		if (nodeCC->height < nodeCB->height) {
			nodeC = RotateRight(iC);
		}

		return RotateLeft(iA);
	}
	else if (u < -1) {
		BPNode *nodeBB = m_listNode[nodeB->left];
		BPNode *nodeBC = m_listNode[nodeB->right];
		if (nodeBC->height > nodeBB->height) {
			nodeB = RotateLeft(iB);
		}
		
		return RotateRight(iA);;
	}

	/// Rebuild AABB & Height
	nodeA->height = 1 + yuh::max(nodeB->height, nodeC->height);
	nodeA->aabb = nodeB->aabb.Combine(nodeC->aabb);

	return nodeA;
}

BPNode* Broadphase::RotateLeft(int iA)
{
	BPNode *nA = m_listNode[iA];
	
	int iC = nA->right;
	int iP = nA->parent;

	BPNode *nC = m_listNode[iC];
	
	if (iP != BPNode::Null) {
		BPNode *nP = m_listNode[iP];

		if (nP->left == iA)
			nP->left = iC;
		else
			nP->right = iC;

		nC->parent = nP->index;
	}
	else {
		nC->parent = BPNode::Null;
		m_root = iC;
	}

	nA->right = nC->left;

	if (nA->right != BPNode::Null) {
		BPNode *nARight = m_listNode[nA->right];
		nARight->parent = iA;
	}

	nC->left = iA;
	nA->parent = iC;

	/// Rebuild nodeA
	BPNode *nAL = m_listNode[nA->left];
	BPNode *nAR = m_listNode[nA->right];
	nA->height = 1 + yuh::max(nAL->height, nAR->height);
	nA->aabb = nAL->aabb.Combine(nAR->aabb);

	/// Rebuild nodeB
	BPNode *nCR = m_listNode[nC->right];
	nC->height = 1 + yuh::max(nCR->height, nA->height);
	nC->aabb = nCR->aabb.Combine(nA->aabb);

	return nC;
}

BPNode* Broadphase::RotateRight(int iA)
{
	BPNode *nA = m_listNode[iA];

	int iB = nA->left;
	int iP = nA->parent;

	BPNode *nB = m_listNode[iB];
	nB->parent = iP;

	if (iP != BPNode::Null) {
		BPNode *nP = m_listNode[iP];

		if (nP->left == iA)
			nP->left = iB;
		else
			nP->right = iB;
	}
	else {
		m_root = iB;
	}

	nA->left = nB->right;

	if (nA->left != BPNode::Null) {
		BPNode *nALeft = m_listNode[nA->left];
		nALeft->parent = iA;
	}

	nB->right = iA;
	nA->parent = iB;

	/// Rebuild nodeA
	BPNode *nAL = m_listNode[nA->left];
	BPNode *nAR = m_listNode[nA->right];
	nA->height = 1 + yuh::max(nAL->height, nAR->height);
	nA->aabb = nAL->aabb.Combine(nAR->aabb);

	/// Rebuild nodeB
	BPNode *nBL = m_listNode[nB->left];
	nB->height = 1 + yuh::max(nBL->height, nA->height);
	nB->aabb = nBL->aabb.Combine(nA->aabb);

	return nB;
}

void Broadphase::Remove(ICollider * object)
{
}

void Broadphase::Move(ICollider *object)
{
	BPNode* node = m_listNode[object->m_nodeIndex];
	const AABB& aabbFat = node->aabb;
	const AABB& aabbObj = node->userdata->getAABB();

	if (aabbFat.Contains(aabbObj) == false) {

		if (node->parent == BPNode::Null) {
			ComputeFatAABB(node);
			return;
		}

		BPNode* nodeParent = m_listNode[node->parent];
		BPNode* nodeSibling = m_listNode[nodeParent->left == node->index ? nodeParent->right : nodeParent->left];

		/// Khi node cha là node lá của root
		if (nodeParent->parent == BPNode::Null) {
			m_root = nodeSibling->index;
			nodeSibling->parent = BPNode::Null;
		}
		else {
			BPNode* nodeParent2 = m_listNode[nodeParent->parent];
			nodeSibling->parent = nodeParent2->index;

			if (nodeParent2->left == nodeParent->index)
				nodeParent2->left = nodeSibling->index;
			else
				nodeParent2->right = nodeSibling->index;
		}

		/// Reinsert
		node->height = 0;
		ComputeFatAABB(node);
		InsertNode(node->index, nodeParent->index);

		/// Thêm vào danh sách node cập nhật
		m_listMove.push_back(node->index);

		/// Update height
		RebuildBottomUp(nodeSibling->parent);
	}
}

void Broadphase::QueryAllAABB(const AABB & aabb, std::vector<AABB*>& outListAABB, std::vector<ICollider*>& outListCollide)
{
	int stack[256];
	int cstack = 0;
	stack[cstack++] = m_root;

	while (cstack)
	{
		int index = stack[--cstack];
		if (index == BPNode::Null)
			continue;

		BPNode* node = m_listNode[index];
		if (aabb.Overlap(node->aabb)) {
			outListAABB.push_back(&node->aabb);
			if (node->IsLeaf()) {
				if (aabb.Overlap(node->userdata->getAABB()))
					outListCollide.push_back(node->userdata);
			}
			else {
				stack[cstack++] = node->left;
				stack[cstack++] = node->right;
			}
		}
	}
}

void Broadphase::Query(const AABB & aabb, std::vector<ICollider*>& outListCollide)
{
	int stack[256];
	int cstack = 0;
	stack[cstack++] = m_root;

	while (cstack)
	{
		BPNode* node = m_listNode[stack[--cstack]];
		if (aabb.Overlap(node->aabb)) {
			if (node->IsLeaf()) {
				if(aabb.Overlap(node->userdata->getAABB()))
					outListCollide.push_back(node->userdata);
			}
			else {
				stack[cstack++] = node->left;
				stack[cstack++] = node->right;
			}
		}
	}
}

void Broadphase::Free()
{
	for (BPNode *node : m_listNode) {
		delete node;
	}
}

int Broadphase::GetHeight()
{
	return m_listNode[m_root]->height;
}

int Broadphase::GetBalanceMax()
{
	int balanceMax = 0;
	int stack[256];
	int cstack = 0;
	stack[cstack++] = m_root;

	while (cstack) {
		BPNode *node = m_listNode[stack[--cstack]];
		if (!node->IsLeaf()) {
			int balance = m_listNode[node->left]->height - m_listNode[node->right]->height;
			if (abs(balance) > abs(balanceMax))
				balanceMax = balance;
			stack[cstack++] = node->left;
			stack[cstack++] = node->right;
		}
	}

	return balanceMax;
}

int Broadphase::GetNumMoveObject()
{
	return m_listMove.size();
}

int Broadphase::GetNumNode()
{
	return m_listNode.size();
}

E_NS
