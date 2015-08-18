#pragma once

#define DLL_EXPORT __declspec(dllexport)

class DynamicMath
{
public:
	DLL_EXPORT DynamicMath() = default;
	DLL_EXPORT ~DynamicMath() = default;

	static DLL_EXPORT double add(double a, double b);
	static DLL_EXPORT double sub(double a, double b);
	static DLL_EXPORT double mul(double a, double b);
	static DLL_EXPORT double div(double a, double b);

	DLL_EXPORT void print();
};

