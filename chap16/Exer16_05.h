#ifndef EXER16_05_H
#define EXER16_05_H
#include <iostream>
template <typename V, unsigned N>
void print(const V (&arr)[N])
{
	for(auto elem : arr) {
		std::cout << elem << " ";
	}
	std::cout << std::endl;
}
#endif