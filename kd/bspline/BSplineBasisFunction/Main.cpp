/*
*    Copyright (c) 2013 eryar All Rights Reserved.
*
*        File    : Main.cpp
*        Author  : eryar@163.com
*        Date    : 2013-03-23 22:11
*        Version : V1.0
*
*    Description : Use Cox-deBoor formula to implemente the 
*                  B-Spline Basis functions.
*
*/

#include <osgDB/ReadFile>
#include <osgViewer/Viewer>
#include <osgGA/StateSetManipulator>
#include <osgViewer/ViewerEventHandlers>

#include "BSplineBasisFunction.h"

// #pragma comment(lib, "osgd.lib")
// #pragma comment(lib, "osgDBd.lib")
// #pragma comment(lib, "osgGAd.lib")
// #pragma comment(lib, "osgViewerd.lib")

osg::Node* MakeBasisFuncLine(BSplineBasisFunction& bf, int i, int p)
{
    // The property basis functions.
    int iLen = bf.GetKnotVectorSize();
    int iStep = 800;
    double dStart = bf.GetKnot(0);
    double dEnd = bf.GetKnot(iLen-1);
    double dDelta = (dEnd - dStart) / iStep;
    double u = 0;
    double v = 0;

    // Create the Geode (Geometry Node) to contain all our osg::Geometry objects.
    osg::Geode* geode = new osg::Geode;

    // Create Geometry object to store all the vertices and lines primitive.
    osg::ref_ptr<osg::Geometry> linesGeom = new osg::Geometry;

    // Set the vertex array to the points geometry object.
    osg::ref_ptr<osg::Vec3Array> pointsVec = new osg::Vec3Array;

    for (int s = 0; s <= iStep; s++)
    {
        u = s * dDelta;
        v = bf.EvalBasis(i, p, u);
        if (v != 0)
        {
            pointsVec->push_back(osg::Vec3(u, 0, v));
        }
    }
    linesGeom->setVertexArray(pointsVec);

    // Set the colors.
    osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;
    colors->push_back(osg::Vec4(1.0f, 1.0f, 0.0f, 0.0f));
    linesGeom->setColorArray(colors.get());
    linesGeom->setColorBinding(osg::Geometry::BIND_OVERALL);

    // Set the normal in the same way of color.
    osg::ref_ptr<osg::Vec3Array> normals = new osg::Vec3Array;
    normals->push_back(osg::Vec3(0.0f, -1.0f, 0.0f));
    linesGeom->setNormalArray(normals.get());
    linesGeom->setNormalBinding(osg::Geometry::BIND_OVERALL);

    // Add the points geometry to the geode.
    linesGeom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINE_STRIP, 0, pointsVec->size()));
    geode->addDrawable(linesGeom.get());

    return geode;
}

osg::Node* CreateScene(void)
{
    osg::Group* root = new osg::Group;

    // Knot vector: U={0,0,0,1,2,3,4,4,5,5,5}.
    std::vector<double> knotVector;
    knotVector.push_back(0);
    knotVector.push_back(0);
    knotVector.push_back(0);
    knotVector.push_back(1);
    knotVector.push_back(2);
    knotVector.push_back(3);
    knotVector.push_back(4);
    knotVector.push_back(4);
    knotVector.push_back(5);
    knotVector.push_back(5);
    knotVector.push_back(5);

    BSplineBasisFunction basisFunc(knotVector);
 
    for (int i = 0; i < basisFunc.GetKnotVectorSize(); i++)
    {
        root->addChild(MakeBasisFuncLine(basisFunc, i, 3));
    }
    
    return root;
}

int main(int argc, char* argv[])
{
    osgViewer::Viewer viewer;
    viewer.setSceneData(CreateScene());

    viewer.addEventHandler(new osgGA::StateSetManipulator(viewer.getCamera()->getOrCreateStateSet()));
    viewer.addEventHandler(new osgViewer::StatsHandler);
    viewer.addEventHandler(new osgViewer::WindowSizeHandler);

    return viewer.run();
}
