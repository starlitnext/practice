/*
	Author: VipXXQ
	2015/8/19
	This is a template class of complex, Using the class as follows:
	'''
	Complex<double> t1(1.5, 2.0);
	Complex<int> t2(1.2, 3.6);
	Complexf t3(1.2, 2.3);
	Complexd t4(1.5, 2.3);
	'''
	The manipulation between two Complex is illegal is the type parameter is different, such as t1 + t2 or t1 += t2 is illegal.
	But if you manipulate between a Complex and a int/float/double real number, then the number will automatic convert to the Complex's type argument.
*/
#pragma once
#include <complex>
#include <iostream>

#undef Complex

template<typename T>
class Complex
{
public:

	//! constructors
	Complex();
	Complex(T _re, T _im=0);
	Complex(const std::complex<T> & c);

	//! conversion to another data type
	template<typename T2> operator Complex<T2>() const;
	//! conjugation 共轭
	Complex conj() const;
	//! conversion to std::complex
	operator std::complex<T>() const;

	T re, im; // the real and the imaginary parts
};

template<typename T> inline Complex<T>::Complex() : re(0), im(0) {}
template<typename T> inline Complex<T>::Complex(T _re, T _im) : re(_re), im(_im) {}
template<typename T> inline Complex<T>::Complex(const std::complex<T> & c) : re(c.real()), im(c.imag()) {}
template<typename T> template<typename T2> inline Complex<T>::operator Complex<T2>() const{
	return Complex<T2>(static_cast<T2>(re), static_cast<T2>(im));
}
template<typename T> inline Complex<T> Complex<T>::conj() const {
	return Complex<T>(re, -im);
}

template<typename T> static
std::ostream& operator << (std::ostream &os, const Complex<T> &a) {
	if (a.im >= 0)
		os << a.re << " + " << a.im << "i";
	else
		os << a.re << " - " << -a.im << "i";
	return os;
}

template<typename T> static inline
bool operator == (const Complex<T> &a, const Complex<T> &b) {
	return a.re == b.re && a.im == b.im;
}

template<typename T> static inline
bool operator != (const Complex<T> &a, const Complex<T> &b) {
	return a.re != b.re || a.im != b.im;
}

/*
两个Complex<T>对象之间的操作: + - * / += -= *= /=
*/
template<typename T> static inline
Complex<T> operator + (const Complex<T> &a, const Complex<T> &b) {
	return Complex<T>(a.re + b.re, a.im + b.im);
}

template<typename T> static inline
Complex<T> operator - (const Complex<T> &a, const Complex<T> &b) {
	return Complex<T>(a.re - b.re, a.im - b.im);
}

template<typename T> static inline
Complex<T> operator * (const Complex<T> &a, const Complex<T> &b) {
	return Complex<T>(a.re * b.re - a.im * b.im, a.re * b.im + a.im * b.re);
}

template<typename T> static inline
Complex<T> operator / (const Complex<T> &a, const Complex<T> &b) {
	double t = 1. / ((double)b.re*b.re + (double)b.im*b.im);
	return Complex<T>((T)((a.re*b.re + a.im*b.im)*t),
					(T)((-a.re*b.im + a.im*b.re)*t));
}

template<typename T> static inline
Complex<T>& operator += (Complex<T> &a, const Complex<T> &b) {
	a.re += b.re; a.im += b.im; return a;
}

template<typename T> static inline
Complex<T>& operator -= (Complex<T> &a, const Complex<T> &b) {
	a.re -= b.re; a.im -= b.im; return a;
}

template<typename T> static inline
Complex<T>& operator *= (Complex<T> &a, const Complex<T> &b) {
	return (a = a * b);
}

template<typename T> static inline
Complex<T>& operator /= (Complex<T> &a, const Complex<T> &b) {
	return (a = a / b);
}

template<typename T> static inline
Complex<T> operator - (const Complex<T> &a) {
	return Complex<T>(-a.re, -a.im);
}

/*
Complex<T>对象和T之间的操作: + - * / += -= *= /=
*/
template<typename T, typename T2> static inline
Complex<T> operator + (const Complex<T> &a, T2 b) {
	T tb = static_cast<T>(b);
	return Complex<T>(a.re + tb, a.im);
}

template<typename T, typename T2> static inline
Complex<T> operator + (T2 b, const Complex<T> &a) {
	T tb = static_cast<T>(b);
	return Complex<T>(a.re + tb, a.im);
}

template<typename T, typename T2> static inline
Complex<T> operator - (const Complex<T> &a, T2 b) {
	T tb = static_cast<T>(b);
	return Complex<T>(a.re - tb, a.im);
}

template<typename T, typename T2> static inline
Complex<T> operator - (T2 b, const Complex<T> &a) {
	return Complex<T>(static_cast<T>(b) - a.re, -a.im);
}

template<typename T, typename T2> static inline
Complex<T> operator * (const Complex<T> &a, T2 b) {
	return Complex<T>(a.re * static_cast<T>(b), a.im * b);
}

template<typename T, typename T2> static inline
Complex<T> operator * (T2 b, const Complex<T> &a) {
	return Complex<T>(a.re * static_cast<T>(b), a.im * static_cast<T>(b));
}

template<typename T, typename T2> static inline
Complex<T> operator / (const Complex<T> &a, T2 b) {
	T t = (T)1 / static_cast<T>(b);
	return Complex<T>(a.re * t, a.im * t);
}

template<typename T, typename T2> static inline
Complex<T> operator / (T2 b, const Complex<T> &a) {
	return Complex<T>(static_cast<T>(b)) / a;
}

template<typename T, typename T2> static inline
Complex<T>& operator += (Complex<T> &a, T2 b) {
	a.re += static_cast<T>(b); return a;
}

template<typename T, typename T2> static inline
Complex<T>& operator -= (Complex<T> &a, T2 b) {
	a.re -= static_cast<T>(b); return a;
}

template<typename T, typename T2> static inline
Complex<T>& operator *= (Complex<T> &a, T2 b) {
	a.re *= static_cast<T>(b); a.im *= static_cast<T>(b); return a;
}

template<typename T, typename T2> static inline
Complex<T>& operator /= (Complex<T> &a, T2 b) {
	T t = (T)1 / static_cast<T>(b);
	a.re *= t; a.im *= t; return a;
}

template<typename T> static inline
double abs(const Complex<T> &a) {
	return std::sqrt((double)a.re * a.re + (double)a.im * a.im);
}

typedef Complex<float> Complexf;
typedef Complex<double> Complexd;