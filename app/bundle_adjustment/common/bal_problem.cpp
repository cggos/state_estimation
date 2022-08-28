#include "bal_problem.h"

#include <cstdio>
#include <fstream>
#include <string>
#include <vector>
#include <math.h>
#include <stdlib.h>

#include <Eigen/Core>

#include "rotation.h"

typedef Eigen::Map<Eigen::VectorXd> VectorRef;
typedef Eigen::Map<const Eigen::VectorXd> ConstVectorRef;

inline double RandDouble() {
    double r = static_cast<double>(rand());
    return r / RAND_MAX;
}

inline double RandNormal() {
    double x1, x2, w;
    do {
        x1 = 2.0 * RandDouble() - 1.0;
        x2 = 2.0 * RandDouble() - 1.0;
        w = x1 * x1 + x2 * x2;
    } while (w >= 1.0 || w == 0.0);

    w = sqrt((-2.0 * log(w)) / w);
    return x1 * w;
}

template<typename T>
void FscanfOrDie(FILE *fptr, const char *format, T *value) {
    int num_scanned = fscanf(fptr, format, value);
    if (num_scanned != 1)
        std::cerr << "Invalid UW data file. ";
}

// 给一个三维向量加入噪声，很简单xyz依次加入随机值就好了。
// 定义这个的目的是为了后面的Perturb()函数在增加噪声时，是分开对路标点，相机的旋转，相机的平移分别加入噪声的，
// 并且这三个量都是三维的，所以定义一个三维向量添加噪声的函数
void PerturbPoint3(const double sigma, double* point) {
    for (int i = 0; i < 3; ++i)
        point[i] += RandNormal() * sigma;
}

//取一个数组的中位数，主要用在Normalize()函数中
double Median(std::vector<double>* data) {
    int n = data->size();
    std::vector<double>::iterator mid_point = data->begin() + n / 2;
    std::nth_element(data->begin(), mid_point, data->end());
    return *mid_point;
}

//构造函数，主要是把优化数据读进程序
BALProblem::BALProblem(const std::string& filename, bool use_quaternions) {

    FILE *fptr = fopen(filename.c_str(), "r");

    if (fptr == NULL) {
        std::cerr << "Error: unable to open file " << filename;
        return;
    }

    // FscanfOrDie()函数直接读取文件的前三个double类型的值，所以文件格式和数据一定要对，不然很容易崩溃，
    // 这里也就是problem-16-22106-pre.txt文件中的头三个数字，表征了相机数，路标点数，观测数。

    // This wil die horribly on invalid files. Them's the breaks.
    FscanfOrDie(fptr, "%d", &num_cameras_);
    FscanfOrDie(fptr, "%d", &num_points_);
    FscanfOrDie(fptr, "%d", &num_observations_);

    std::cout << "Header: " << num_cameras_ << " " << num_points_ << " " << num_observations_;

    point_index_  = new int[num_observations_];
    camera_index_ = new int[num_observations_];
    observations_ = new double[2 * num_observations_];

    //所有要优化的参数量
    num_parameters_ = 9 * num_cameras_ + 3 * num_points_;
    parameters_ = new double[num_parameters_];

    //这里开始对.txt文件读取了，按文件中数据循序读，第i个相机，第j个路标，然后两个像素坐标。循环读完所有观测。
    //用num_observations控制循环读取所有观测，每次读取一行。
    for (int i = 0; i < num_observations_; ++i) {
        //循环内部这几个FscanfOrDie()其实就是读取了txt里的一行
        FscanfOrDie(fptr, "%d", camera_index_ + i);
        FscanfOrDie(fptr, "%d", point_index_ + i);
        for (int j = 0; j < 2; ++j)
            FscanfOrDie(fptr, "%lf", observations_ + 2 * i + j);
    }

    //这里就读到了txt的后半部分，就是所有优化变量的具体值，当然这里也就是初始值了。发现在txt中是列成了一列
    for (int i = 0; i < num_parameters_; ++i)
        FscanfOrDie(fptr, "%lf", parameters_ + i);

    fclose(fptr);

    use_quaternions_ = use_quaternions;
    if (use_quaternions) {
        // Switch the angle-axis rotations to quaternions.
        num_parameters_ = 10 * num_cameras_ + 3 * num_points_;
        double *quaternion_parameters = new double[num_parameters_];
        double *original_cursor = parameters_;
        double *quaternion_cursor = quaternion_parameters;
        for (int i = 0; i < num_cameras_; ++i) {
            AngleAxisToQuaternion(original_cursor, quaternion_cursor);
            quaternion_cursor += 4;
            original_cursor += 3;
            for (int j = 4; j < 10; ++j)
                *quaternion_cursor++ = *original_cursor++;
        }
        // Copy the rest of the points.
        for (int i = 0; i < 3 * num_points_; ++i)
            *quaternion_cursor++ = *original_cursor++;

        // Swap in the quaternion parameters.
        delete[]parameters_;
        parameters_ = quaternion_parameters;
    }
}

void BALProblem::WriteToFile(const std::string& filename)const {

    FILE *fptr = fopen(filename.c_str(), "w");

    if (fptr == NULL) {
        std::cerr << "Error: unable to open file " << filename;
        return;
    }

    fprintf(fptr, "%d %d %d %d\n", num_cameras_, num_cameras_, num_points_, num_observations_);

    for (int i = 0; i < num_observations_; ++i) {
        fprintf(fptr, "%d %d", camera_index_[i], point_index_[i]);
        for (int j = 0; j < 2; ++j) {
            fprintf(fptr, " %g", observations_[2 * i + j]);
        }
        fprintf(fptr, "\n");
    }

    for (int i = 0; i < num_cameras(); ++i) {
        double angleaxis[9];
        if (use_quaternions_) {
            //OutPut in angle-axis format.
            QuaternionToAngleAxis(parameters_ + 10 * i, angleaxis);
            memcpy(angleaxis + 3, parameters_ + 10 * i + 4, 6 * sizeof(double));
        } else
            memcpy(angleaxis, parameters_ + 9 * i, 9 * sizeof(double));

        for (int j = 0; j < 9; ++j)
            fprintf(fptr, "%.16g\n", angleaxis[j]);
    }

    const double *points = parameters_ + camera_block_size() * num_cameras_;
    for (int i = 0; i < num_points(); ++i) {
        const double *point = points + i * point_block_size();
        for (int j = 0; j < point_block_size(); ++j)
            fprintf(fptr, "%.16g\n", point[j]);
    }

    fclose(fptr);
}

// Write the problem to a PLY file for inspection in Meshlab or CloudCompare
void BALProblem::WriteToPLYFile(const std::string& filename)const {

    std::ofstream of(filename.c_str());

    of << "ply"
       << '\n' << "format ascii 1.0"
       << '\n' << "element vertex " << num_cameras_ + num_points_
       << '\n' << "property float x"
       << '\n' << "property float y"
       << '\n' << "property float z"
       << '\n' << "property uchar red"
       << '\n' << "property uchar green"
       << '\n' << "property uchar blue"
       << '\n' << "end_header" << std::endl;

    // Export extrinsic data (i.e. camera centers) as green points.
    double angle_axis[3];
    double center[3];
    for (int i = 0; i < num_cameras(); ++i) {
        const double *camera = cameras() + camera_block_size() * i;
        CameraToAngelAxisAndCenter(camera, angle_axis, center);
        of << center[0] << ' ' << center[1] << ' ' << center[2]
           << "0 255 0" << '\n';
    }

    // Export the structure (i.e. 3D Points) as white points.
    const double *points = parameters_ + camera_block_size() * num_cameras_;
    for (int i = 0; i < num_points(); ++i) {
        const double *point = points + i * point_block_size();
        for (int j = 0; j < point_block_size(); ++j)
            of << point[j] << ' ';
        of << "255 255 255\n";
    }
    of.close();
}

/**
 * 由camera数据中的旋转向量和平移向量解析出相机世界坐标系下的姿态(依旧是旋转向量)和位置(世界坐标系下的xyz)，也是用于生成点云用的
 * @param camera 要解析的相机参数，前三维旋转，接着三维平移，这里指用到这6维
 * @param angle_axis 解析出的相机姿态承接数组，也是旋转向量形式
 * @param center 解析出来的相机原点在世界坐标系下的坐标承接数组，XYZ
 */
void BALProblem::CameraToAngelAxisAndCenter(const double* camera,
                                            double* angle_axis,
                                            double* center) const {
    VectorRef angle_axis_ref(angle_axis, 3);
    if (use_quaternions_)
        QuaternionToAngleAxis(camera, angle_axis);
    else
        angle_axis_ref = ConstVectorRef(camera, 3);

    // c = -R't
    Eigen::VectorXd inverse_rotation = -angle_axis_ref;
    AngleAxisRotatePoint(inverse_rotation.data(),
                         camera + camera_block_size() - 6,
                         center);
    VectorRef(center, 3) *= -1.0;
}

/**
 * 反向过程，由世界坐标系下的相机姿态和原点位置，生成一个camera数据
 * @param angle_axis 旋转向量数据
 * @param center 相机中心在世界坐标系下的位置坐标
 * @param camera 承接数据的camera数组，由于这里只是生成旋转和平移，所以是camera的前6维
 */
void BALProblem::AngleAxisAndCenterToCamera(const double* angle_axis,
                                            const double* center,
                                            double* camera) const {
    ConstVectorRef angle_axis_ref(angle_axis, 3);
    if (use_quaternions_)
        AngleAxisToQuaternion(angle_axis, camera);
    else
        VectorRef(camera, 3) = angle_axis_ref;

    // t = -R * c
    AngleAxisRotatePoint(angle_axis, center, camera + camera_block_size() - 6);
    VectorRef(camera + camera_block_size() - 6, 3) *= -1.0;
}

void BALProblem::Normalize() {
    // Compute the marginal median of the geometry
    std::vector<double> tmp(num_points_);
    Eigen::Vector3d median;
    double *points = mutable_points();
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < num_points_; ++j)
            tmp[j] = points[3 * j + i];
        median(i) = Median(&tmp);
    }

    for (int i = 0; i < num_points_; ++i) {
        VectorRef point(points + 3 * i, 3);
        tmp[i] = (point - median).lpNorm<1>();
    }

    const double median_absolute_deviation = Median(&tmp);

    // Scale so that the median absolute deviation of the resulting
    // reconstruction is 100

    const double scale = 100.0 / median_absolute_deviation;

    // X = scale * (X - median)
    for (int i = 0; i < num_points_; ++i) {
        VectorRef point(points + 3 * i, 3);
        point = scale * (point - median);
    }

    double *cameras = mutable_cameras();
    double angle_axis[3];
    double center[3];
    for (int i = 0; i < num_cameras_; ++i) {
        double *camera = cameras + camera_block_size() * i;
        CameraToAngelAxisAndCenter(camera, angle_axis, center);
        // center = scale * (center - median)
        VectorRef(center, 3) = scale * (VectorRef(center, 3) - median);
        AngleAxisAndCenterToCamera(angle_axis, center, camera);
    }
}

void BALProblem::Perturb(const double rotation_sigma,
                         const double translation_sigma,
                         const double point_sigma) {
    assert(point_sigma >= 0.0);
    assert(rotation_sigma >= 0.0);
    assert(translation_sigma >= 0.0);

    double *points = mutable_points();
    if (point_sigma > 0) {
        for (int i = 0; i < num_points_; ++i)
            PerturbPoint3(point_sigma, points + 3 * i);
    }

    //这里相机是被分成两块，旋转和平移，
    //旋转是考虑到四元数形式，增加了一步用CameraToAngelAxisAndCenter()从camera中取出三维的angle_axis,
    //然后添加噪声，添加完后再用AngleAxisAndCenterToCamera()重构camera参数
    //平移部分就直接用PerturbPoint3()添加了
    for (int i = 0; i < num_cameras_; ++i) {
        double *camera = mutable_cameras() + camera_block_size() * i;
        double angle_axis[3];
        double center[3];
        // Perturb in the rotation of the camera in the angle-axis representation
        CameraToAngelAxisAndCenter(camera, angle_axis, center);
        if (rotation_sigma > 0.0)
            PerturbPoint3(rotation_sigma, angle_axis);
        AngleAxisAndCenterToCamera(angle_axis, center, camera);

        if (translation_sigma > 0.0)
            PerturbPoint3(translation_sigma, camera + camera_block_size() - 6);
    }
}
