/* Vector 2D
* Inline
*/
template<typename T>
inline Vec2<T>::Vec2()
{
	x = 0;
	y = 0;
}

template<typename T>
inline Vec2<T>::Vec2(T x, T y)
{
	this->x = x;
	this->y = y;
}

template<typename T>
template<typename U>
inline Vec2<T>::operator Vec2<U>()
{
	return Vec2<U>(x, y);
}


template<typename T>
inline Vec2<T>::operator T*()
{
	return &x;
}


// a+b
template<typename T>
Vec2<T> operator+(Vec2<T> a, Vec2<T> b) {
	return Vec2<T>(a.x + b.x, a.y + b.y);
}

// a-b
template<typename T>
Vec2<T> operator-(Vec2<T> a, Vec2<T> b) {
	return Vec2<T>(a.x - b.x, a.y - b.y);
}

// a*b
template<typename T>
T operator*(Vec2<T> a, Vec2<T> b) {
	return a.x*b.x + a.y*b.y;
}

// vector*number
template<typename T>
Vec2<T> operator*(Vec2<T> a, T b) {
	return Vec2<T>(a.x * b, a.y * b);
}

// number*vector
template<typename T>
Vec2<T> operator*(T a, Vec2<T> b) {
	return Vec2<T>(a * b.b.x, a * b.y);
}


// a==b
template<typename T>
bool operator==(Vec2<T> a, Vec2<T> b) {
	return (a.x == b.x) && (a.y == b.y);
}


// a!=b
template<typename T>
bool operator!=(Vec2<T> a, Vec2<T> b) {
	return (a.x != b.x) || (a.y != b.y);
}


/* Tính độ dài Vector
*
*/
template<typename T>
float VectorLength(Vec2<T> v) {
	return sqrt(v.x*v.x + v.y*v.y);
}


template<typename T>
Vec2f VectorNormalize(Vec2<T> v) {
	float l = VectorLength(v);
	return Vec2f(
		v.x / l,
		v.y / l
	);
}




/* Print Vector
 *
 */
template<typename T>
void Print(const Vec2<T> &v, std::ostream &out = std::cout) {
	out << "(" << v.x << ":" << v.y << ")";
}


template<typename T>
std::ostream& operator<<(std::ostream& out, const Vec2<T> &v) {
	Print(v, out);
	return out;
}