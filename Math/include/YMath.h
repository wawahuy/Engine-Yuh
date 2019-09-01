#pragma once


namespace yuh {

	const float PI = 3.14159265359;

	template<typename T>
	T max(T a, T b) {
		return a > b ? a : b;
	}

	template<typename T>
	void Swap(T& a, T& b) {
		T t = a;
		a = b;
		b = t;
	}

	template<typename T>
	T abs(T a) {
		return a > 0 ? a : -a;
	}

	template<typename T>
	T min(T a, T b) {
		return a < b ? a : b;
	}

	template<typename T, typename V>
	struct Pair
	{
		T A;
		V B;
	};



}