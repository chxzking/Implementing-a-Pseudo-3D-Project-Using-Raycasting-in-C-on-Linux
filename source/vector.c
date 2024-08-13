#include "../header/vector.h"
#include <math.h>
#include <stdio.h>
//创建一个向量体
vector vector_create(double x, double y) {
    vector v;
    v.x = x;
    v.y = y;
    return v;
}

/////////////////////注意，减法都是前减后的操作


//计算两个向量的和
vector vector_add(const vector* v1, const vector* v2) {
    return vector_create(v1->x + v2->x, v1->y + v2->y);
}

//计算两个向量的差
vector vector_sub(const vector* v1, const vector* v2) {
    return vector_create(v1->x - v2->x, v1->y - v2->y);
}

//计算两个向量的点积
double vector_dot(const vector* v1, const vector* v2) {
    return v1->x * v2->x + v1->y * v2->y;
}

//计算一个向量和一个标量的积
vector vector_mul_scalar(const vector* v, double k) {
    return vector_create(k * v->x, k * v->y);
}

//计算向量与标量的商
vector vector_div_scalar(const vector* v, double k) {
    if(k == 0){
        printf("出现错误：向量(%lf,%lf)出现除0情况\n",v->x,v->y);
        return vector_create(0, 0);
    }
    return vector_create(v->x / k, v->y / k);
}

//计算向量模长的平方
double vector_magnitude2(const vector* v) {
    return v->x * v->x + v->y * v->y;
}

//计算向量的模长
double vector_magnitude(const vector* v) {
    return sqrt(vector_magnitude2(v));
}

//归一化向量
vector vector_normalize(vector* v) {
    double mag = vector_magnitude(v);
    return vector_div_scalar(v, mag);
}

//计算向量取反
vector vector_negate(const vector* v) {
    return vector_create(-v->x, -v->y);
}
