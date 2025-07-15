/*
*    Copyright (c) 2013 eryar All Rights Reserved.
*
*        File    : BSplineBasisFunction.cpp
*        Author  : eryar@163.com
*        Date    : 2013-03-23 22:14
*        Version : V1.0
*
*    Description : Use Cox-deBoor formula to implemente the 
*                  B-Spline Basis functions.
*
*/

#include "BSplineBasisFunction.h"

BSplineBasisFunction::BSplineBasisFunction( const std::vector<double>& U )
    :mKnotVector(U)
{

}


BSplineBasisFunction::~BSplineBasisFunction(void)
{
}

int BSplineBasisFunction::GetKnotVectorSize( void ) const
{
    return static_cast<int> (mKnotVector.size());
}

double BSplineBasisFunction::GetKnot( int i ) const
{
    return mKnotVector[i];
}

/*
* @brief Binary search of the knot vector.
*/
int BSplineBasisFunction::FindSpan( double u )
{
    int iSize = static_cast<int> (mKnotVector.size());

    if (u >= mKnotVector[iSize-1])
    {
        return iSize;
    }

    int iLow = 0;
    int iHigh = iSize;
    int iMiddle = (iLow + iHigh) / 2;

    while (u < mKnotVector[iMiddle] || u > mKnotVector[iMiddle+1])
    {
        if (u < mKnotVector[iMiddle])
        {
            iHigh = iMiddle;
        }
        else
        {
            iLow = iMiddle;
        }

        iMiddle = (iLow + iHigh) / 2;
    }

    return iMiddle;
}

double BSplineBasisFunction::EvalBasis( int i, int p, double u )
{
    if ((i+p+1) >= GetKnotVectorSize())
    {
        return 0;
    }

    if (0 == p)
    {
        if (u >= mKnotVector[i] && u < mKnotVector[i+1])
        {
            return 1;
        } 
        else
        {
            return 0;
        }
    }

    double dLeftUpper = u - mKnotVector[i];
    double dLeftLower = mKnotVector[i+p] - mKnotVector[i];
    double dLeftValue = 0;

    double dRightUpper = mKnotVector[i+p+1] - u;
    double dRightLower = mKnotVector[i+p+1] - mKnotVector[i+1];
    double dRightValue = 0;

    if (dLeftUpper != 0 && dLeftLower != 0)
    {
        dLeftValue = (dLeftUpper / dLeftLower) * EvalBasis(i, p-1, u);
    }

    if (dRightUpper != 0 && dRightLower != 0)
    {
        dRightValue = (dRightUpper / dRightLower) * EvalBasis(i+1, p-1, u);
    }

    return (dLeftValue + dRightValue);
}
