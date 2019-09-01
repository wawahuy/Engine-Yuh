#pragma once
#include "Vector2D.h"

namespace yuh {

	/// Matrix Row
	template<typename T>
	class Mat2 {

	public:
		Mat2();
		Mat2(T i0, T i1, T i2, T i3);

		void Identity();
		void Set(T i0, T i1, T i2, T i3);


	private:
		union
		{
			T m_matrix[4];

			struct {
				Vec2<T> m_rowI, m_rowJ;
			};
		};

	};

	template<typename T>
	inline Mat2<T>::Mat2()
	{
		Identity();
	}

	template<typename T>
	inline Mat2<T>::Mat2(T i0, T i1, T i2, T i3)
	{
		Set(i0, i1, i2, i3);
	}

	template<typename T>
	inline void Mat2<T>::Identity()
	{
		m_rowI.x = 0;
		m_rowI.y = 0;
		m_rowJ.x = 0;
		m_rowJ.y = 0;
	}

	template<typename T>
	inline void Mat2<T>::Set(T i0, T i1, T i2, T i3)
	{
		m_rowI.x = i0;
		m_rowI.y = i1;
		m_rowJ.x = i2;
		m_rowJ.y = i3;
	}

}