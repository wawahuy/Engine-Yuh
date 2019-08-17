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

void Broadphase::ComputeAABBObject(BPNode *node)
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

	/// Gán ID node cho userdata
	object->nodeIndex = node->index;

	/// Tính toán và vỗ béo aabb
	ComputeAABBObject(node);

	/// Gán node vào danh sách lá
	m_listLeaf.push_back(node->index);

	/// Thêm vào danh sách xữ lí bộ đệm va chạm
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

		/// Next
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

void Broadphase::Remove(ICollider * object)
{
}

void Broadphase::Update()
{
	if (!m_root) return;
	BPNode* nodeRoot = m_listNode[m_root];

	/// Khi chỉ có 1 node
	if (nodeRoot->IsLeaf()) {
		nodeRoot->aabb = nodeRoot->userdata->getAABB();
	}
	else {

		/// Danh sách các node 'lá' thay đổi
		m_listReinsert.clear();

		for (int nodeIndex : m_listLeaf) {
			BPNode* node = m_listNode[nodeIndex];

			/// Khểm tra object aabb có nằm trong node không
			if (!node->aabb.Contains(node->userdata->getAABB())) {
				m_listReinsert.push_back(nodeIndex);
			}
		}

		/// Cập nhật lại các AABB
		for (int nodeIndex : m_listReinsert) {
			BPNode* node = m_listNode[nodeIndex];
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
			ComputeAABBObject(node);
			InsertNode(node->index, nodeParent->index);

			/// Thêm vào danh sách node cập nhật
			m_listMove.push_back(node->index);

			/// Update height
			RebuildBottomUp(nodeSibling->parent);

			/// Xóa các bộ đệm va chạm
			m_listCachePair.Remove(node->index);
		}
	}


}



void Broadphase::QueryPair(int queryID, const AABB & aabb)
{
	int stack[256];
	int cstack = 0;
	stack[cstack++] = m_root;

	while (cstack)
	{
		BPNode* node = m_listNode[stack[--cstack]];
		if (aabb.Overlap(node->aabb)) {
			if (node->IsLeaf()) {

				/// Pair cùng một đối tượng
				if (queryID == node->index) continue;

				/// Thêm vào danh sách va chạm
				m_listCachePair.Add(queryID, node->index);
			}
			else {
				stack[cstack++] = node->left;
				stack[cstack++] = node->right;
			}
		}
	}
}

void Broadphase::ComputePair(std::vector<IColliderPair>& outListColliderPair)
{
	/// Truy vấn va chạm
	for (int i : m_listMove) {
		QueryPair(i, m_listNode[i]->aabb);
	}


	//AVLNode<PTNode> *stackA[2560];
	//AVLNode<AVLNode<PTNode>*> *stackB[2560];

	//int cstackA = 0;
	//stackA[cstackA++] = m_listCachePair.GetRoot();

	//while (cstackA)
	//{
	//	AVLNode<PTNode>  *node = stackA[--cstackA];

	//	if (node) {
	//		stackA[cstackA++] = node->left;
	//		stackA[cstackA++] = node->right;

	//		int cstackB = 0;
	//		stackB[cstackB++] = node->data.pair.GetRoot();
	//		while (cstackB)
	//		{
	//			AVLNode<AVLNode<PTNode>*> *nodePair = stackB[--cstackB];

	//			if (nodePair) {
	//				stackB[cstackB++] = nodePair->left;
	//				stackB[cstackB++] = nodePair->right;

	//				outListColliderPair.push_back({
	//					m_listNode[node->data.value]->userdata,
	//					m_listNode[nodePair->data->data.value]->userdata
	//				});
	//			}
	//		}
	//	}
	//}

	AVLNode<physicalTest::PTNode> *stackA[2560];
	AVLNode<physicalTest::CPTNode> *stackB[2560];

	int cstackA = 0;
	stackA[cstackA++] = m_listCachePair.GetRoot();

	while (cstackA)
	{
		AVLNode<physicalTest::PTNode> *node = stackA[--cstackA];

		if (node) {
			stackA[cstackA++] = node->left;
			stackA[cstackA++] = node->right;

			int cstackB = 0;
			stackB[cstackB++] = node->data.pair.GetRoot();
			while (cstackB)
			{
				AVLNode<physicalTest::CPTNode> *nodePair = stackB[--cstackB];

				if (nodePair) {
					stackB[cstackB++] = nodePair->left;
					stackB[cstackB++] = nodePair->right;

					outListColliderPair.push_back({
						m_listNode[node->data.value]->userdata,
						m_listNode[nodePair->data.value]->userdata
					});
				}
			}
		}
	}

	m_listMove.clear();
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

int Broadphase::GetNumPairCache()
{
	return 0;
}

physicalTest::PairTree & Broadphase::GetPairCacheTree()
{
	return m_listCachePair;
}


E_NS
