﻿#pragma once
#include <vector>
#include <algorithm>
#include "Export.h"
#include "PairTree.h"
#include "ICollider.h"


S_NS_PHYSICAL
;



/// Struct
/// Node đại diện cho 1 đối tượng hoặc một nhánh
/// Node chứa các thông tin cơ bản của một cây nhị phân
struct BPNode {
	/// Node Null
	static const int Null = -1;

	/// Chi mục cơ bản
	int index;
	int parent;
	int left;
	int right;
	int height;

	/// Khu vực chứa đối tượng
	AABB aabb;

	/// Thông tin đối tương
	ICollider *userdata;

	/// Kiểm tra lá
	bool IsLeaf() {
		return right == Null;
	}
};





/// Class
/// Pha rộng sử dụng BVH (cây AABB động)
/// Pha rộng có thể sử dụng trong phạm vi vô hạng với các đối tượng chuyển động không ngừng
class YUH_API_PHYSICAL Broadphase {
public:
	Broadphase();
	~Broadphase();

	/// Thêm đối tượng vào không gian
	/// Các đối tượng này không được giải phóng khi hủy Broadphase
	void Add(ICollider *object);


	/// Xóa đối tượng khỏi không gian
	/// Chỉ xóa đối tượng khỏi không gian, nhưng không giải phóng nó khỏi bộ nhớ
	void Remove(ICollider *object);


	/// Cập nhật các đối tượng
	/// Phương pháp cập nhật này bắc buộc phải đi qua tấc cả các bodies để xem nó có vượt khỏi Zone
	void Update();


	/// Truy vấn và lấy AABB thuộc các Node, bao gồm lá và cả nhánh
	void QueryAllAABB(const AABB& aabb, std::vector<AABB *>& outListAABB, std::vector<ICollider *>& outListCollide);
	

	/// Truy vấn các lá va chạm AABB
	void Query(const AABB& aabb, std::vector<ICollider *>& outListCollide);


	/// Tính toán, cập nhật các đối tượng va chạm
	void ComputePair(std::vector<IColliderPair>& outListColliderPair);


	/// Get
	int GetHeight();
	int GetBalanceMax();
	int GetNumMoveObject();
	int GetNumNode();
	int GetNumPairCache();
	PairTree &GetPairCacheTree();

private:
	BPNode*	 CreateNode();
	void	 QueryPair(int queryID, const AABB& aabb);
	void	 InsertNode(int indexInsert, int indexBranch = BPNode::Null);
	void	 Balance(int iA);
	void	 ComputeAABBObject(BPNode *node);
	void	 RebuildBottomUp(int index);

	/// Node gốc
	int						m_root;

	/// Danh sách các đối tượng di chuyển
	/// Danh sách này được dùng để tái kiểm tra va chạm
	std::vector<int>		m_listMove;

	/// Danh sách các đối tượng Reinsert
	std::vector<int>		m_listReinsert;

	/// Danh sách các node lá
	std::vector<int>		m_listLeaf;

	/// Danh sách các cặp va chạm
	PairTree				m_listCachePair;

	/// Danh sách node
	std::vector<BPNode *>	m_listNode;

};

E_NS