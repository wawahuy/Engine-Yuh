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

namespace yuh {

	/* Vector 2D
	 * Header
	 * Dont use namespace math
	 *
	 */
	template<typename T>
	class Vec2
	{
	public:
		Vec2<T>();
		Vec2<T>(T x, T y);

		union {
			struct {
				T x, y;
			};

			struct {
				T u, v;
			};
		};

		/* Ep kieu
		 *
		 */
		template <typename U> explicit operator Vec2<U>();
		operator T*();

	};

	/* Type Vec2[x]
	 *
	 */
	typedef Vec2<float>				Vec2f;
	typedef Vec2<char>				Vec2b;
	typedef Vec2<short>				Vec2s;
	typedef Vec2<int>				Vec2i;
	typedef Vec2<long>				Vec2l;
	typedef Vec2<unsigned char>		Vec2ub;
	typedef Vec2<unsigned short>	Vec2us;
	typedef Vec2<unsigned int>		Vec2ui;
	typedef Vec2<unsigned long>		Vec2ul;

#include "Vector2D.inl"
}


