/*
	- Vector.h
	- Project	:	System Class
	- Author	:	Nguyen Gia Huy
	- Email		:	Kakahuy99@gmail.com
					Kakahuy104@gmail.com
	- Start		:	05/12/2018
	- Update	:  
*/

#pragma once
#include <iostream>
#include <string>
#include "Vector2D.h"

namespace yuh {

	/* Vector 3D
	 * Header
	 */
	template<typename T>
	class Vec3
	{
	public:
		Vec3<T>();
		Vec3<T>(T x, T y, T z);
		Vec3<T>(const Vec3<T> v2, T z);

		union {
			struct {
				T x, y, z;
			};

			struct {
				T r, g, b;
			};

			struct {
				Vec2<T> xy;
			};

			struct {
				T V[3];
			};

		};


		/* Ep kieu
		 *
		 */
		template <typename U> explicit operator Vec3<U>();
		operator T*();

	};

	/* Type Vec3[x]
	 *
	 */
	typedef Vec3<float>				Vec3f;
	typedef Vec3<char>				Vec3b;
	typedef Vec3<short>				Vec3s;
	typedef Vec3<int>				Vec3i;
	typedef Vec3<long>				Vec3l;
	typedef Vec3<unsigned char>		Vec3ub;
	typedef Vec3<unsigned short>	Vec3us;
	typedef Vec3<unsigned int>		Vec3ui;
	typedef Vec3<unsigned long>		Vec3ul;


#include "Vector3D.inl"
}


