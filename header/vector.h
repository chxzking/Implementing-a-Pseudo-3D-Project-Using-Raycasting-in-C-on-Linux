#ifndef __VECTOR_H__
#define __VECTOR_H__


//向量结构体
typedef struct vector{
    double x, y;
} vector;

//创建一个向量体
vector vector_create(double x, double y);

//计算两个向量的和
vector vector_add(const vector* v1, const vector* v2);

//计算两个向量的差
vector vector_sub(const vector* v1, const vector* v2);

//计算两个向量的点积
double vector_dot(const vector* v1, const vector* v2);

//计算一个向量和一个标量的积
vector vector_mul_scalar(const vector* v, double k);

//计算向量与标量的商
vector vector_div_scalar(const vector* v, double k);

//计算向量模长的平方
double vector_magnitude2(const vector* v);

//计算向量的模长
double vector_magnitude(const vector* v) ;

//归一化向量
vector vector_normalize(vector* v);

//计算向量取反
vector vector_negate(const vector* v);

#endif