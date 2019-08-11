#pragma once
#include "DynamicTree.h"
#include "AABB.h"
#include "Array.h"

S_NS_PHYSICAL
;

/// Interface
/// Các đối tượng có thể va chạm cần được kế thừa ICollider
/// Là một thành phần trong Broadphase
/// Nó bao gồm việc xây dựng getAABB() quanh đối tượng, và getVelocity() là vận tốc đối tượng
class ICollider {
public:
	virtual AABB  getAABB() = 0;
	virtual Vec2f getVelocity() = 0;
};


typedef Pair<int, int>					IndexPair;
typedef Array<IndexPair>				IndexPairArray;
typedef Pair<ICollider *, ICollider *>	ColliderPair;
typedef Array<ColliderPair>				ColliderPairArray;


/// Class
/// Pha rộng sử dụng BVH (cây AABB động)
/// Pha rộng có thể sử dụng trong phạm vi vô hạng với các đối tượng chuyển động không ngừng
class Broadphase {
public:

	/// Thêm đối tượng vào không gian
	void Add(ICollider *object);

	/// Xóa đối tượng khỏi không gian
	void Remove(ICollider *object);

	/// Cập nhật các đối tượng
	void Update();

	/// Tính toán, cập nhật các đối tượng va chạm

private:
	DynamicTree		m_dT;
	ArrayInt		m_listMove;
	IndexPairArray	m_collideCache;
};

E_NS