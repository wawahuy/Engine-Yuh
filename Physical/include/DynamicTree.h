#pragma once
#include "Broadphase.h"
#include <algorithm>
#include <functional>
#include <stack>

S_NS_PHYSICAL

#define MARGIN_PX_AABB 10
#define MARGIN_FACTOR_VELOCITY_AABB 2.0f


struct YUH_API_PHYSICAL DTNode {
	static const int Null = -1;

	AABB aabb;

	int parent;
	int index;
	int left;
	int right;
	int height;

	ICollider *userdata;

	bool IsLeaf() {
		return right == Null;
	}
};

typedef std::vector<DTNode *> DTNodeList;

class YUH_API_PHYSICAL DynamicTree : Broadphase
{
public:
	DynamicTree();
	~DynamicTree();

	void Add(ICollider* object);
	void Remove(ICollider* object);
	void Update();
	const IColliderPairList& ComputePair();
	const DTNodeList& GetListNode();
	DTNode* GetRoot();
	int GetHeight();

	void QueryAllNode(IColliderList& list, std::vector<AABB*>& aabbs, const AABB& aabb);


private:
	void InsertNode(DTNode* node, int index, int branchIndex = -1);
	void ComputeAABBObject(DTNode* node);
	void Query(const AABB& aabb);
	bool QueryCallback(int index);
	
	IColliderPairList m_colliderPairList;
	DTNodeList m_listNode;
	DTNode* m_root;

	int m_queryIndex;
	std::vector<unsigned int> m_moveIndex;
	std::vector<std::pair<unsigned int, unsigned int>> m_pairIndex;
};

E_NS