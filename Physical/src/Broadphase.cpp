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

	/// Tính toán và vỗ béo aabb
	ComputeAABBObject(node);

	/// Gán node vào danh sách lá
	m_listLeaf.push_back(node->index);

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


	/// Tính lại chiều cao, và AABB trên nhánh
	index = nodeBranch->index;
	while (index != BPNode::Null)
	{
		node  = m_listNode[index];
		BPNode *left  = m_listNode[node->left];
		BPNode *right = m_listNode[node->right];

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
}

void Broadphase::Remove(ICollider * object)
{
}

void Broadphase::Update()
{
	if (m_root) {
		BPNode* nodeRoot = m_listNode[m_root];

		/// Khi chỉ có 1 node
		if (nodeRoot->IsLeaf()) {
			nodeRoot->aabb = nodeRoot->userdata->getAABB();
		}
		else {

			/// Danh sách các node 'lá' thay đổi
			std::vector<BPNode*> listNodeUpdate;

			for (int nodeIndex : m_listLeaf) {
				BPNode* node = m_listNode[nodeIndex];
				
				/// Khểm tra object aabb có nằm trong node không
				if (!node->aabb.Contains(node->userdata->getAABB())) {
					listNodeUpdate.push_back(node);
				}
			}

			/// Cập nhật lại các AABB
			for (BPNode* node : listNodeUpdate) {
				BPNode* nodeParent = m_listNode[node->parent];
				BPNode* nodeSibling = m_listNode[nodeParent->left == node->index ? nodeParent->right : nodeParent->left];

				/// Khi chí có 2 lá
				if (nodeParent->parent == BPNode::Null) {
					m_root = nodeSibling->index;
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
				if (nodeSibling->parent != BPNode::Null) {

					int nodeIndex = nodeSibling->parent;
					while (nodeIndex != BPNode::Null)
					{
						BPNode *node  = m_listNode[nodeIndex];;
						BPNode *left  = m_listNode[node->left];
						BPNode *right = m_listNode[node->right];

						/// Height
						node->height = 1 + yuh::max(left->height, right->height);

						/// AABB
						node->aabb = left->aabb.Combine(right->aabb);

						/// Next
						nodeIndex = node->parent;
					}
				}


				/// Xóa các bộ đệm va chạm
				int i = 0;
				int size = m_listCachePair.size();

				while (i < size) {

					const IndexPair &p1 = m_listCachePair[i];
					if (p1.A == node->index || p1.B == node->index) {
						
						int j = i;
						while (++j < size) {
							const IndexPair &p2 = m_listCachePair[j];
							if (p2.A != node->index && p2.B != node->index)
								break;
						}

						auto &begin = m_listCachePair.begin();
						m_listCachePair.erase(begin + i, begin + j);
						size = m_listCachePair.size();

						/// Mảng có A < B và được sắp xếp, nên nếu nodeIndex == A thì nó đã được xóa hết
						if (node->index == p1.A) {
							break;
						}
					}
					else
						i++;
				}

			}
		}
	}
}

void Broadphase::Query(int queryID, const AABB & aabb)
{
	int stack[256];
	int cstack = 0;
	stack[cstack++] = m_root;

	while (cstack)
	{
		BPNode* node = m_listNode[stack[--cstack]];
		if (aabb.Overlap(node->aabb)) {
			if (node->IsLeaf()) {
				if (queryID == node->index) continue;
				m_listCachePair.push_back(IndexPair{
					yuh::min(queryID, node->index),
					yuh::max(queryID, node->index)
				});
			}
			else {
				stack[cstack++] = node->left;
				stack[cstack++] = node->right;
			}
		}
	}
}


bool PairLessThan(const IndexPair& A, const IndexPair& B)
{
	if (A.A < B.A)
	{
		return true;
	}

	if (A.B == B.B)
	{
		return A.B < B.B;
	}

	return false;
}

void Broadphase::ComputePair(std::vector<IColliderPair>& outListColliderPair)
{
	int hasUpdate = m_listCachePair.size();
	for (int i : m_listMove) {
		Query(i, m_listNode[i]->aabb);
	}

	if (hasUpdate != m_listCachePair.size())
		std::sort(m_listCachePair.begin(), m_listCachePair.end(), PairLessThan);

	//add pair
	int i = 0;
	int size = m_listCachePair.size();
	while (i < size) {
		const IndexPair &p1 = m_listCachePair[i];
		outListColliderPair.push_back({
			m_listNode[p1.A]->userdata,
			m_listNode[p1.B]->userdata
		});

		++i;
		while (i < size) {
			const IndexPair &p2 = m_listCachePair[i];
			if (p1.A != p2.A || p1.B != p2.B)
				break;
			m_listCachePair.erase(m_listCachePair.begin() + i);
			size = m_listCachePair.size();
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
	return 0;
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
