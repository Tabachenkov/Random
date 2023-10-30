#pragma once
typedef struct RandomSource
{
	double x;
	long long linear_x;
	void *ops;
} RandomSource;
typedef struct RandomSourceOperations
{
	RandomSource *(* destroy) (RandomSource *);
	double (* next) (RandomSource *);
} RandomSourceOperations;
