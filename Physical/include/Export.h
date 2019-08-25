#pragma once

#ifdef IS_EXPORT
	#define YUH_API_PHYSICAL __declspec(dllexport)
#else
	#define YUH_API_PHYSICAL __declspec(dllimport)
#endif

/// Namespace
#define S_NS_PHYSICAL namespace yuh { \
						namespace physical {

/// End namspace
#define E_NS } }


/// Class: Broadphase
/// Lề 'px' của node so với object
#define MARGIN_PX_AABB 1

/// Class: Broadphase
/// Hệ số vận tốc để mở rộng độ lớn node
#define MARGIN_FACTOR_VELOCITY_AABB 2.0f


/// File: Manifold.h
/// Số điểm giao cắt tối đã giữa 2 Collider
#define MANIFOLD_NUM_MAX 2
