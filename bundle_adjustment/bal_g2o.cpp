#include <iostream>
#include <stdint.h>
#include <unordered_set>
#include <memory>
#include <vector>
#include <stdlib.h>

#include <Eigen/StdVector>
#include <Eigen/Core>

#include "common/bal_problem.h"
#include "common/bundle_params.h"
#include "common/g2o_imp.h"

using namespace Eigen;
using namespace std;

typedef Eigen::Map<Eigen::VectorXd> VectorRef;
typedef Eigen::Map<const Eigen::VectorXd> ConstVectorRef;

// set up the vertexs and edges for the bundle adjustment.
void BuildProblem(const BALProblem* bal_problem, g2o::SparseOptimizer* optimizer, const BundleParams& params) {
    const int num_points        = bal_problem->num_points();
    const int num_cameras       = bal_problem->num_cameras();
    const int camera_block_size = bal_problem->camera_block_size();
    const int point_block_size  = bal_problem->point_block_size();
    const int num_observations  = bal_problem->num_observations();

    // Set camera vertex with initial value in the dataset.
    const double* raw_cameras = bal_problem->cameras();
    for(int i = 0; i < num_cameras; ++i) {
        ConstVectorRef temVecCamera(raw_cameras + camera_block_size * i, camera_block_size);
        VertexCamera *pCamera = new VertexCamera();
        pCamera->setEstimate(temVecCamera);   // initial value for the camera i..
        pCamera->setId(i);                    // set id for each camera vertex
        optimizer->addVertex(pCamera);
    }

    // Set point vertex with initial value in the dataset.
    const double* raw_points = bal_problem->points();
    for(int j = 0; j < num_points; ++j) {
        ConstVectorRef temVecPoint(raw_points + point_block_size * j, point_block_size);
        VertexPoint *pPoint = new VertexPoint();
        pPoint->setEstimate(temVecPoint);   // initial value for the point i..
        // each vertex should have an unique id, no matter it is a camera vertex, or a point vertex
        pPoint->setId(j + num_cameras);
        pPoint->setMarginalized(true);
        optimizer->addVertex(pPoint);
    }

    // Set edges for graph..
    const double* observations = bal_problem->observations();   // pointer for the first observation..
    for(int i = 0; i < num_observations; ++i) {
        EdgeObservation *bal_edge = new EdgeObservation();

        if (params.robustify) {
            g2o::RobustKernelHuber *rk = new g2o::RobustKernelHuber;
            rk->setDelta(1.0);
            bal_edge->setRobustKernel(rk);
        }

        const int camera_id = bal_problem->camera_index()[i];              // get id for the camera;
        const int point_id  = bal_problem->point_index()[i] + num_cameras; // get id for the point

        // set the vertex by the ids for an edge observation
        bal_edge->setVertex(0, dynamic_cast<VertexCamera *>(optimizer->vertex(camera_id)));
        bal_edge->setVertex(1, dynamic_cast<VertexPoint *>(optimizer->vertex(point_id)));
        bal_edge->setInformation(Eigen::Matrix2d::Identity());
        bal_edge->setMeasurement(Eigen::Vector2d(observations[2 * i + 0], observations[2 * i + 1]));

        optimizer->addEdge(bal_edge);
    }
}

void WriteToBALProblem(BALProblem* bal_problem, g2o::SparseOptimizer* optimizer) {
    const int num_points  = bal_problem->num_points();
    const int num_cameras = bal_problem->num_cameras();
    const int camera_block_size = bal_problem->camera_block_size();
    const int point_block_size  = bal_problem->point_block_size();

    double *raw_cameras = bal_problem->mutable_cameras();
    for (int i = 0; i < num_cameras; ++i) {
        VertexCamera *pCamera = dynamic_cast<VertexCamera *>(optimizer->vertex(i));
        Eigen::VectorXd NewCameraVec = pCamera->estimate();
        memcpy(raw_cameras + i * camera_block_size, NewCameraVec.data(), sizeof(double) * camera_block_size);
    }

    double *raw_points = bal_problem->mutable_points();
    for (int j = 0; j < num_points; ++j) {
        VertexPoint *pPoint = dynamic_cast<VertexPoint *>(optimizer->vertex(j + num_cameras));
        Eigen::Vector3d NewPointVec = pPoint->estimate();
        memcpy(raw_points + j * point_block_size, NewPointVec.data(), sizeof(double) * point_block_size);
    }
}

int main(int argc, char** argv)
{
    BundleParams params(argc, argv);

    if(params.input.empty()){
        std::cout << "Usage: ba_g2o -input <path to bal_dataset: problem-*.txt>\n";
        return 1;
    }

    BALProblem bal_problem(params.input.c_str());

    std::cout << "\n\n"
              << "bal problem file loaded..." << std::endl
              << "bal problem have " << bal_problem.num_cameras() << " cameras and "
              << bal_problem.num_points() << " points. " << std::endl
              << "Forming " << bal_problem.num_observations() << " observatoins. " << std::endl;

    // store the initial 3D cloud points and camera pose..
    if (!params.initial_ply.empty()) {
        bal_problem.WriteToPLYFile(params.initial_ply);
    }

    // add some noise for the intial value
    srand(params.random_seed);
    bal_problem.Normalize();
    bal_problem.Perturb(params.rotation_sigma, params.translation_sigma, params.point_sigma);

    {
        g2o::SparseOptimizer optimizer;
        SetSolverOptions(&optimizer, params.linear_solver, params.trust_region_strategy);
        BuildProblem(&bal_problem, &optimizer, params);
        optimizer.initializeOptimization();
        optimizer.setVerbose(true);
        optimizer.optimize(params.num_iterations);
        // write the optimized data into BALProblem class
        WriteToBALProblem(&bal_problem, &optimizer);
    }

    // write the result into a .ply file.
    if (!params.final_ply.empty()) {
        bal_problem.WriteToPLYFile(params.final_ply);
    }

    return 0;
}
