#include "../include/Broadphase.h"

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

	Vec2f vel = node->userdata->getVelocity()*((float)MARGIN_FACTOR_VELOCITY_AABB);

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
	BPNode *node, *left, *right;

	while (index != BPNode::Null)
	{
		node	= m_listNode[index];
		left	= m_listNode[node->left];
		right	= m_listNode[node->right];

		/// Height
		node->height = 1 + yuh::max(left->height, right->height);

		/// AABB
		node->aabb = left->aabb.Combine(right->aabb);

		index = node->parent;
	}
}

void Broadphase::Balance(int iA)
{
	///			A
	///		   / \
	///		  B	  C
	BPNode *nodeA = m_listNode[iA];

	int iB = nodeA->left;
	int iC = nodeA->right;

	BPNode *nodeB = m_listNode[iB];
	BPNode *nodeC = m_listNode[iC];


}

int Broadphase::RotateLeft(int iA)
{
	return 0;
}

int Broadphase::RotateRight(int iA)
{
	return 0;
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
		BPNode* node = m_listNode[stack[--cstack]];
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
