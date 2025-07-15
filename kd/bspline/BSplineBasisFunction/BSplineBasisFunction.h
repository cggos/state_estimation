/*
*    Copyright (c) 2013 eryar All Rights Reserved.
*
*        File    : BSplineBasisFunction.h
*        Author  : eryar@163.com
*        Date    : 2013-03-23 22:13
*        Version : V1.0
*
*    Description : Use Cox-deBoor formula to implemente the 
*                  B-Spline Basis functions.
*
*/
#ifndef _BSPLINEBASISFUNCTION_H_
#define _BSPLINEBASISFUNCTION_H_

#include <vector>

class BSplineBasisFunction
{
public:
    BSplineBasisFunction(const std::vector<double>& U);
    ~BSplineBasisFunction(void);

public:
    /*
    * @brief Binary search of the knot vector.
    */
    int FindSpan(double u);

    /*
    * @brief 
    * @param [in] i: span of the parameter u;
    *        [in] p: degree;
    *        [in] u: parameter;
    */
    double EvalBasis(int i, int p, double u);

    /*
    * @breif Get knot vector size.
    */
    int GetKnotVectorSize(void) const;

    /*
    * @breif Get the knot value of the given index.
    */
    double GetKnot(int i) const;

private:
    std::vector<double> mKnotVector;
};

#endif // _BSPLINEBASISFUNCTION_H_


