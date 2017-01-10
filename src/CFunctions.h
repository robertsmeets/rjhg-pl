/*
 * CFunctions.h
 *
 *  Created on: Feb 25, 2015
 *      Author: robert
 */

#ifndef CFUNCTIONS_H_
#define CFUNCTIONS_H_

#include "stdio.h"
#include <vector>
#include "CInterpreter.h"

using namespace std;

unsigned int func_plus_ii(unsigned int, unsigned int);
double func_plus_id(int, double);
double func_plus_di(double, int);
double func_plus_dd(double, double);
unsigned int func_minus_ii(unsigned int, unsigned int);
double func_minus_id(int, double);
double func_minus_di(double, int);
double func_minus_dd(double, double);
unsigned int func_mul_ii(unsigned int, unsigned int);
double func_mul_id(int, double);
double func_mul_di(double, int);
double func_mul_dd(double, double);
bool func_eq_ii(unsigned int, unsigned int);
bool func_eq_id(int, double);
bool func_eq_di(double, int);
bool func_eq_dd(double, double);
bool func_ne_ii(unsigned int, unsigned int);
bool func_ne_id(unsigned int, double);
bool func_ne_di(double, unsigned int);
bool func_ne_dd(double, double);
bool func_lt_ii(unsigned int, unsigned int);
bool func_lt_id(unsigned int, double);
bool func_lt_di(double, int);
bool func_lt_dd(double, double);
bool func_ge_ii(unsigned int, unsigned int);
bool func_ge_id(unsigned int, double);
bool func_ge_di(double, unsigned int);
bool func_ge_dd(double, double);
bool func_gt_ii(unsigned int, unsigned int);
bool func_gt_id(unsigned int, double);
bool func_gt_di(double, unsigned int);
bool func_gt_dd(double, double);
bool func_le_ii(unsigned int, unsigned int);
bool func_le_id(unsigned int, double);
bool func_le_di(double, unsigned int);
bool func_le_dd(double, double);

void array_add(char* ,vector<stack_element>* ,uint16_t* ,bool );
void array_set(char* ,vector<stack_element>* ,uint16_t* ,bool );

template <typename T>
void delete_pointed_to(T* const ptr)
{
    delete ptr;
}

#endif /* CFUNCTIONS_H_ */
