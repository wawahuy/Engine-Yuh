/* Vector 3D
* Inline
*/
template<typename T>
inline Vec3<T>::Vec3()
{
	x = 0;
	y = 0;
	z = 0;
}

template<typename T>
inline Vec3<T>::Vec3(T x, T y, T z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

template<typename T>
inline yuh::Vec3<T>::Vec3(const Vec3<T> v2, T z)
{
	xy = v2;
	z = z;
}


template<typename T>
template<typename U>
inline Vec3<T>::operator Vec3<U>()
{
	return Vec3<U>(x, y, z);
}


template<typename T>
inline Vec3<T>::operator T*()
{
	return &x;
}


// a+b
template<typename T>
Vec3<T> operator+(const Vec3<T>& a, const Vec3<T>& b) {
	return Vec3<T>(a.x + b.x, a.y + b.y, a.z + b.z);
}


// a+=b
template<typename T>
Vec3<T>& operator+=(Vec3<T>& a, const Vec3<T>& b) {
	a.x += b.x;
	a.y += b.y;
	a.z += b.z;
	return a;
}


// a-=b
template<typename T>
Vec3<T>& operator-=(Vec3<T>& a, const Vec3<T>& b) {
	a.x -= b.x;
	a.y -= b.y;
	a.z -= b.z;
	return a;
}

// a-b
template<typename T>
Vec3<T> operator-(const Vec3<T>& a, const Vec3<T>& b) {
	return Vec3<T>(a.x - b.x, a.y - b.y, a.z - b.z);
}

// a*b
template<typename T>
T operator*(const Vec3<T>& a, const Vec3<T>& b) {
	return a.x*b.x + a.y*b.y + a.z*b.z;
}

// vector*number
template<typename T>
Vec3<T> operator*(const Vec3<T>& a, T b) {
	return Vec3<T>(a.x * b, a.y * b, a.z * b);
}

// number*vector
template<typename T>
Vec3<T> operator*(T a, const Vec3<T>& b) {
	return Vec3<T>(a * b.b.x, a * b.y, a * b.z);
}



/* Tính độ dài Vector
 *
 */
template<typename T>
float VectorLength(const Vec3<T>& v) {
	return sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
}


template<typename T>
Vec3f VectorNormalize(const Vec3<T>& v) {
	float leng = VectorLength(v);
	return Vec3<T>(
			v.x /leng,
			v.y /leng,
			v.z /leng
		);
}

template<typename T>
Vec3<T> VectorCross(const Vec3<T>& v1, const Vec3<T>& v2) {
	return Vec3<T>(
		v1.y*v2.z - v1.z*v2.y,
		v1.z*v2.x - v1.x*v2.z,
		v1.x*v2.y - v1.y*v2.x
		);
}


template<typename T>
float VectorCorner(const Vec3<T>& v1, const Vec3<T>& v2) {
	return (v1*v2) / (VectorLength(v1)*VectorLength(v2));
}


template<typename T>
std::string VectorToString(const Vec3<T> &v) {
	return "(" + std::to_string(v.x) + "," + std::to_string(v.y) + "," + std::to_string(v.z) + ")";
}


/* Print Vector
*
*/
template<typename T>
void Print(const Vec3<T> &v, std::ostream &out = std::cout) {
	out << "(" << v.x << "," << v.y << "," << v.z << ")";
}


template<typename T>
std::ostream& operator<<(std::ostream& out, const Vec3<T> &v) {
	Print(v, out);
	return out;
}