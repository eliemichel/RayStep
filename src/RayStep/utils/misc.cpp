// **************************************************
// Author : �lie Michel <elie.michel@telecom-paristech.fr>
// UNPUBLISHED CODE.
// Copyright (C) 2017 �lie Michel.
// **************************************************

#include "utils/misc.h"

int log2(int i) {
	int l = 0;
	while (i >>= 1) { ++l; }
	return l;
}
