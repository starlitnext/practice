/*
	Author: VipXXQ
	2015/8/19
	This is a template class of Size
*/
#pragma once

using std::ostream;

template<typename T>
class Size_ {
public:

	//! various constructors
	Size_();
	Size_(T _width, T _height);
	Size_(const Size_ &sz);

	Size_& operator = (const Size_ &sz);
	//! the area (width * height)
	T area() const;

	//! conversion of another data type.
	template<typename T2> operator Size_<T2>() const;

	T width, height; // the width and the height
};

//! various constructors
template<typename T> inline Size_<T>::Size_() : width(0), height(0) {}
template<typename T> inline Size_<T>::Size_(T _width, T _height) : width(_width), height(_height) {}
template<typename T> inline Size_<T>::Size_(const Size_ &sz) : width(sz.width), height(sz.height) {}

template<typename T> inline Size_<T>& Size_<T>::operator= (const Size_<T> &sz)
{
	width = sz.width; height = sz.height; return *this;
}

template<typename T> inline T Size_<T>::area() const { return width * height; }

template<typename T> template<typename T2> inline Size_<T>::operator Size_<T2>() const
{
	return Size_<T2>(static_cast<T2>(width), static_cast<T2>(height));
}

template<typename T> static inline Size_<T> operator + (const Size_<T> &a, const Size_<T> &b)
{
	return Size_<T>(a.width + b.width, a.height + b.height);
}

template<typename T> static inline Size_<T> operator - (const Size_<T> &a, const Size_<T> &b)
{
	return Size_<T>(a.width - b.width, a.height - b.height);
}

template<typename T> static inline Size_<T> operator * (const Size_<T> &a, T b)
{
	return Size_<T>(a.width * b, a.height * b);
}

template<typename T> static inline Size_<T>& operator += (Size_<T> &a, const Size_<T> &b)
{
	a.width += b.width; a.height += b.height; return a;
}

template<typename T> static inline Size_<T>& operator -= (Size_<T> &a, const Size_<T> &b)
{
	a.width -= b.width; a.height -= b.height; return a;
}

template<typename T> static inline bool operator == (const Size_<T> &a, const Size_<T> &b)
{
	return a.width == b.width && a.height == b.height;
}

template<typename T> static inline bool operator != (const Size_<T> &a, const Size_<T> &b)
{
	return a.width != b.width || a.height != b.height;
}

template<typename T> static inline std::ostream& operator << (std::ostream &os, const Size_<T> &a) {
	os << "widht = " << a.width << " ; height = " << a.height;
	return os;
}

typedef Size_<int> Size2i;
typedef Size2i Size;
typedef Size_<double> Size2d;
