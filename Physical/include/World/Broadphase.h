#pragma once
#include <vector>
#include <algorithm>
#include "../Export.h"
#include "../Collision/ICollider.h"


S_NS_PHYSICAL
;
class ContactManager;


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


	/// Truy vấn và lấy AABB thuộc các Node, bao gồm lá và cả nhánh
	void QueryAllAABB(const AABB& aabb, std::vector<AABB *>& outListAABB, std::vector<ICollider *>& outListCollide);
	

	/// Kiểm tra 2 node
	bool TestOverlap(int iA, int iB);


	/// Truy vấn các lá va chạm AABB
	void Query(const AABB& aabb, std::vector<ICollider *>& outListCollide);


	/// Thêm vào danh sách đối tượng di chuyển
	void Move(ICollider *object);


	/// Tìm kiếm các vụ va chạm qua các đối tượng di chuyển
	template<class T> void UpdatePair(T* contactManager);

	/// Giải phóng
	void Free();

	/// Get
	int GetHeight();
	int GetBalanceMax();
	int GetNumMoveObject();
	int GetNumNode();

private:
	BPNode*	 CreateNode();
	void	 InsertNode(int indexInsert, int indexBranch = BPNode::Null);
	void	 ComputeFatAABB(BPNode *node);
	void	 RebuildBottomUp(int index);
	
	BPNode*	 Balance(int iA);
	BPNode*	 RotateLeft(int iA);
	BPNode*	 RotateRight(int iA);

	/// Node gốc
	int						m_root;

	/// Danh sách các đối tượng di chuyển
	/// Danh sách này được dùng để tái kiểm tra va chạm
	std::vector<int>		m_listMove;

	/// Danh sách node
	std::vector<BPNode *>	m_listNode;
};


inline bool Broadphase::TestOverlap(int iA, int iB) {
	const AABB& aabb1 = m_listNode[iA]->aabb;
	const AABB& aabb2 = m_listNode[iB]->aabb;
	return aabb1.Overlap(aabb2);
}




/// -------------- Phần sau có sử dụng mã của Box 2D -----------------
/// https://github.com/erincatto/Box2D/blob/master/Box2D/Collision/b2BroadPhase.h
/// Đoạn code tham khảo và được sửa đổi để phù hơp với dự án

inline bool b2PairLessThan(const Pair<int, int>& pair1, const Pair<int, int>& pair2)
{
	if (pair1.A < pair2.A)
	{
		return true;
	}

	if (pair1.A == pair2.A)
	{
		return pair1.B < pair2.B;
	}

	return false;
}


template<class T>
inline void Broadphase::UpdatePair(T * contactManager)
{
	int stack[256];
	int stack_count;
	std::vector<Pair<int, int>> list_pair;

	for (int nodeQrID : m_listMove) {

		/// Reset stack
		stack_count = 0;
		stack[stack_count++] = m_root;

		/// AABB Fat cần kiểm tra
		const AABB& aabb = m_listNode[nodeQrID]->aabb;
		
		while (stack_count)
		{
			BPNode* node = m_listNode[stack[--stack_count]];
			
			if (aabb.Overlap(node->aabb)) {
				
				if (node->IsLeaf()) {

					if (nodeQrID == node->index) continue;

					list_pair.push_back(
					{
						yuh::min(nodeQrID, node->index),
						yuh::max(nodeQrID, node->index)
					}
					);
				}
				else {
					stack[stack_count++] = node->left;
					stack[stack_count++] = node->right;
				}
			}
		}
	}


	std::sort(list_pair.begin(), list_pair.end(), b2PairLessThan);


	int i = 0;
	int m_pairCount = list_pair.size();
	while (i < m_pairCount)
	{
		auto primaryPair = list_pair[i];
		ICollider* userDataA = m_listNode[primaryPair.A]->userdata;
		ICollider* userDataB = m_listNode[primaryPair.B]->userdata;

		contactManager->Add(userDataA, userDataB);
		++i;

		// Skip any duplicate pairs.
		while (i < m_pairCount)
		{
			auto pair = list_pair[i];;
			if (pair.A != primaryPair.A || pair.B != primaryPair.B)
			{
				break;
			}
			++i;
		}
	}

	m_listMove.clear();
}

E_NS

