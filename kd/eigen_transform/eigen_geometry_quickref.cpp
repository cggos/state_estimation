#include <iostream>
#include <Eigen/Dense>

using namespace std;
using namespace Eigen;

int main(int argc, char **argv) {

    AngleAxisd t_V(M_PI / 4, Vector3d(0, 0, 1));
    Matrix3d t_R = t_V.matrix();
    Quaterniond t_Q(t_V);

    //对旋转向量（轴角）赋值的三大种方法

    //1.使用旋转的角度和旋转轴向量（此向量为单位向量）来初始化角轴
    AngleAxisd V1(M_PI / 4, Vector3d(0, 0, 1));
    cout << "Rotation_vector1" << endl << V1.matrix() << endl;

    //2.使用旋转矩阵转旋转向量的方式

    //2.1 使用旋转向量的fromRotationMatrix()函数来对旋转向量赋值（注意此方法为旋转向量独有,四元数没有）
    AngleAxisd V2;
    V2.fromRotationMatrix(t_R);
    cout << "Rotation_vector2" << endl << V2.matrix() << endl;

    //2.2 直接使用旋转矩阵来对旋转向量赋值
    AngleAxisd V3;
    V3 = t_R;
    cout << "Rotation_vector3" << endl << V3.matrix() << endl;

    //2.3 使用旋转矩阵来对旋转向量进行初始化
    AngleAxisd V4(t_R);
    cout << "Rotation_vector4" << endl << V4.matrix() << endl;

    //3. 使用四元数来对旋转向量进行赋值

    //3.1 直接使用四元数来对旋转向量赋值
    AngleAxisd V5;
    V5 = t_Q;
    cout << "Rotation_vector5" << endl << V5.matrix() << endl;

    //3.2 使用四元数来对旋转向量进行初始化
    AngleAxisd V6(t_Q);
    cout << "Rotation_vector6" << endl << V6.matrix() << endl;


    //------------------------------------------------------

    //对四元数赋值的三大种方法（注意Eigen库中的四元数前三维是虚部,最后一维是实部）

    //1.使用旋转的角度和旋转轴向量（此向量为单位向量）来初始化四元数,即使用q=[cos(A/2),n_x*sin(A/2),n_y*sin(A/2),n_z*sin(A/2)]
    Quaterniond Q1(
            cos((M_PI / 4) / 2),
            0 * sin((M_PI / 4) / 2),
            0 * sin((M_PI / 4) / 2),
            1 * sin((M_PI / 4) / 2));//以（0,0,1）为旋转轴，旋转45度
    //第一种输出四元数的方式
    cout << "Quaternion1" << endl << Q1.coeffs() << endl;

    //第二种输出四元数的方式
    cout << Q1.x() << endl << endl;
    cout << Q1.y() << endl << endl;
    cout << Q1.z() << endl << endl;
    cout << Q1.w() << endl << endl;

    //2. 使用旋转矩阵转四元數的方式

    //2.1 直接使用旋转矩阵来对旋转向量赋值
    Quaterniond Q2;
    Q2 = t_R;
    cout << "Quaternion2" << endl << Q2.coeffs() << endl;


    //2.2 使用旋转矩阵来对四元數进行初始化
    Quaterniond Q3(t_R);
    cout << "Quaternion3" << endl << Q3.coeffs() << endl;

    //3. 使用旋转向量对四元数来进行赋值

    //3.1 直接使用旋转向量对四元数来赋值
    Quaterniond Q4;
    Q4 = t_V;
    cout << "Quaternion4" << endl << Q4.coeffs() << endl;

    //3.2 使用旋转向量来对四元数进行初始化
    Quaterniond Q5(t_V);
    cout << "Quaternion5" << endl << Q5.coeffs() << endl;



    //----------------------------------------------------

    //对旋转矩阵赋值的三大种方法

    //1.使用旋转矩阵的函数来初始化旋转矩阵
    Matrix3d R1=Matrix3d::Identity();
    cout << "Rotation_matrix1" << endl << R1 << endl;

    //2. 使用旋转向量转旋转矩阵来对旋转矩阵赋值

    //2.1 使用旋转向量的成员函数matrix()来对旋转矩阵赋值
    Matrix3d R2;
    R2 = t_V.matrix();
    cout << "Rotation_matrix2" << endl << R2 << endl;

    //2.2 使用旋转向量的成员函数toRotationMatrix()来对旋转矩阵赋值
    Matrix3d R3;
    R3 = t_V.toRotationMatrix();
    cout << "Rotation_matrix3" << endl << R3 << endl;

    //3. 使用四元数转旋转矩阵来对旋转矩阵赋值

    //3.1 使用四元数的成员函数matrix()来对旋转矩阵赋值
    Matrix3d R4;
    R4 = t_Q.matrix();
    cout << "Rotation_matrix4" << endl << R4 << endl;

    //3.2 使用四元数的成员函数toRotationMatrix()来对旋转矩阵赋值
    Matrix3d R5;
    R5 = t_Q.toRotationMatrix();
    cout << "Rotation_matrix5" << endl << R5 << endl;

    return 0;
}
