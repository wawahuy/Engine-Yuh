#pragma once

namespace yuh {

	template<typename T>
	T max(T a, T b) {
		return a > b ? a : b;
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