#include "../include/DynamicTree.h"


S_NS_PHYSICAL

;


DynamicTree::DynamicTree()
{
	m_root = nullptr;
}

DynamicTree::~DynamicTree()
{
}

const DTNodeList & DynamicTree::GetListNode()
{
	return m_listNode;
}

DTNode * DynamicTree::GetRoot()
{
	return m_root;
}

int DynamicTree::GetHeight()
{
	return m_root->height;
}

int DynamicTree::GetBalanceMax()
{
	int balanceMax = 0;
	int stack[256];
	int cstack = 0;
	stack[cstack++] = m_root->index;

	while (cstack) {
		DTNode *node = m_listNode[stack[--cstack]];
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

int DynamicTree::GetNumMoveObject()
{
	return m_moveIndex.size();
}



void DynamicTree::ComputeAABBObject(DTNode * node)
{
	node->userdata->getAABB(&node->aabb);
	node->aabb.min.x -= MARGIN_PX_AABB;
	node->aabb.min.y -= MARGIN_PX_AABB;
	node->aabb.max.x += MARGIN_PX_AABB;
	node->aabb.max.y += MARGIN_PX_AABB;

	//Increase Velocity
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

void DynamicTree::Add(ICollider * object)
{
	DTNode* node = new DTNode();
	node->index = m_listNode.size();
	node->right = DTNode::Null;
	node->left = DTNode::Null;
	node->parent = DTNode::Null;
	node->height = 0;
	node->userdata = object;
	ComputeAABBObject(node);
	m_listNode.push_back(node);

	if (m_root == nullptr) {
		m_root = node;
	}
	else {
		InsertNode(node, m_root->index);
	}
}

void DynamicTree::Remove(ICollider * object)
{
}

void DynamicTree::Update()
{
	if (m_root) {
		if (m_root->IsLeaf()) {
			m_root->userdata->getAABB(&m_root->aabb);
		}
		else {
			DTNodeList nodeupdate;
			AABB aabb;
			for (DTNode* node : m_listNode) {
				if (node->IsLeaf()) {
					node->userdata->getAABB(&aabb);
					if (!ContainsAABBAABB(node->aabb, aabb)) {
						nodeupdate.push_back(node);
					}
				}
			}


			for (DTNode* node : nodeupdate) {
				DTNode* nodeParent = m_listNode[node->parent];
				DTNode* nodeSibling = m_listNode[nodeParent->left == node->index ? nodeParent->right : nodeParent->left];

				if (nodeParent == m_root) {
					m_root = nodeSibling;
					ComputeAABBObject(node);
					InsertNode(node, m_root->index, nodeParent->index);
				}
				else {
					DTNode* nodeParent2 = m_listNode[nodeParent->parent];
					nodeSibling->parent = nodeParent2->index;

					if (nodeParent2->left == nodeParent->index)
						nodeParent2->left = nodeSibling->index;
					else
						nodeParent2->right = nodeSibling->index;

					ComputeAABBObject(node);
					node->height = 0;
					InsertNode(node, m_root->index, nodeParent->index);
				}

				m_moveIndex.push_back(node->index);


				//update height
				if (nodeSibling->parent != DTNode::Null) {
					nodeParent = m_listNode[nodeSibling->parent];
					while (nodeParent->parent != DTNode::Null)
					{
						nodeParent->height = 1 + std::max(m_listNode[nodeParent->left]->height, m_listNode[nodeParent->right]->height);
						nodeParent = m_listNode[nodeParent->parent];
					}
				}


				//remove pair index qualify
				int i = 0;
				int size = m_pairIndex.size();
				
				while (i < size) {
					const std::pair<unsigned int, unsigned int> &p1 = m_pairIndex[i];
					if (p1.first == node->index || p1.second == node->index) {

						int j = i;
						while (++j < size) {
							const std::pair<unsigned int, unsigned int> &p2 = m_pairIndex[j];
							if (p2.first != node->index && p2.second != node->index)
								break;
						}

						auto &begin = m_pairIndex.begin();
						m_pairIndex.erase(begin + i, begin + j);
						size = m_pairIndex.size();
					}
					else
						i++;
				}

			}
		}
	}
}

void DynamicTree::Query(const AABB & aabb)
{
	int stack[256];
	int cstack = 0;
	stack[cstack++] = m_root->index;

	while (cstack)
	{
		DTNode* node = m_listNode[stack[--cstack]];
		if (OverlapAABBAABB(aabb, node->aabb)) {
			if (node->IsLeaf()) {
				if (!QueryCallback(node->index)) {
					return;
				}
			}
			else {
				stack[cstack++] = node->left;
				stack[cstack++] = node->right;
			}
		}
	}
}

bool DynamicTree::QueryCallback(int index)
{
	if (m_queryIndex == index) {
		return true;
	}
	
	
	m_pairIndex.push_back(std::make_pair(
		std::min(m_queryIndex, index),
		std::max(m_queryIndex, index)
	));

	return true;
}


bool PairLessThan(const std::pair<unsigned int, unsigned int>& A, const std::pair<unsigned int, unsigned int>& B)
{
	if (A.first < B.first)
	{
		return true;
	}

	if (A.first == B.first)
	{
		return A.second < B.second;
	}

	return false;
}

const IColliderPairList & DynamicTree::ComputePair()
{
	m_colliderPairList.clear();

	if (!m_root || m_root->IsLeaf())
		return m_colliderPairList;

	int hasUpdate = m_pairIndex.size();
	for (int i : m_moveIndex) {
		m_queryIndex = i;
		Query(m_listNode[i]->aabb);
	}
	
	if(hasUpdate != m_pairIndex.size())
		std::sort(m_pairIndex.begin(), m_pairIndex.end(), PairLessThan);

	//add pair
	int i = 0;
	int size = m_pairIndex.size();
	while (i < size) {
		const std::pair<unsigned int, unsigned int> &p1 = m_pairIndex[i];
		m_colliderPairList.push_back(std::make_pair(
			m_listNode[p1.first]->userdata,
			m_listNode[p1.second]->userdata
		));

		++i;
		while (i < size) {
			const std::pair<unsigned int, unsigned int> &p2 = m_pairIndex[i];
			if (p1.first != p2.first || p1.second != p2.second)
				break;
			m_pairIndex.erase(m_pairIndex.begin() + i);
			size = m_pairIndex.size();
		}
	}

	m_moveIndex.clear();
	return m_colliderPairList;
}


void DynamicTree::InsertNode(DTNode * node, int index, int branchIndex)
{
	DTNode *p = m_listNode[index];

	if (p->IsLeaf()) {
		DTNode *newNode;
		if (branchIndex == -1) {
			newNode = new DTNode();             
			newNode->index = m_listNode.size();
			m_listNode.push_back(newNode);
		}
		else {
			newNode = m_listNode[branchIndex];
			newNode->index = branchIndex;
		}

		newNode->left = p->index;
		newNode->right = node->index;

		//Update Height
		p->height = 0;
		node->height = 0;

		if (m_listNode.size() == 3) {
			newNode->parent = DTNode::Null;
			m_root = newNode;
		}
		else {
			newNode->parent = p->parent;
			DTNode* pp = m_listNode[newNode->parent];
			if (pp->left == p->index)
				pp->left = newNode->index;
			else
				pp->right = newNode->index;
		}

		p->parent = node->parent = newNode->index;
		p->right = DTNode::Null;
		p->left = DTNode::Null;
		p = newNode;


	}
	else {
		DTNode* nodeLeft = m_listNode[p->left];
		DTNode* nodeRight = m_listNode[p->right];
		AABB nAABB1, nAABB2;
		CombineAABB(&nAABB1, nodeLeft->aabb, node->aabb);
		CombineAABB(&nAABB2, nodeRight->aabb, node->aabb);
		float vol1 = yuh::physical::AreaAABB(nAABB1) - yuh::physical::AreaAABB(nodeLeft->aabb);
		float vol2 = yuh::physical::AreaAABB(nAABB2) - yuh::physical::AreaAABB(nodeRight->aabb);

		if (vol1 < vol2) {
			InsertNode(node, nodeLeft->index, branchIndex);
		}
		else {
			InsertNode(node, nodeRight->index, branchIndex);
		}

	}

	//Update Height
	p->height = 1 + std::max(m_listNode[p->left]->height, m_listNode[p->right]->height);


	//Update AABB
	CombineAABB(&p->aabb, m_listNode[p->left]->aabb, m_listNode[p->right]->aabb);

}


void DynamicTree::Balance(int index)
{
	//Rot Left

	//Rot Right
}


void DynamicTree::QueryAllNode(IColliderList & list, std::vector<AABB*>& aabbs, const AABB & aabb)
{
	int stack[256];
	int cstack = 0;
	stack[cstack++] = m_root->index;

	while (cstack)
	{
		DTNode* node = m_listNode[stack[--cstack]];
		if (OverlapAABBAABB(aabb, node->aabb)) {
			aabbs.push_back(&node->aabb);
			if (node->IsLeaf()) {
				list.push_back(node->userdata);
			}
			else {
				stack[cstack++] = node->left;
				stack[cstack++] = node->right;
			}
		}
	}
}

E_NS