#include <iostream>
#include <memory>

#include "BsplineSE3.h"
#include "Simulator.h"
#include "utils/dataset_reader.h"

int main() {
  ov_msckf::SimParam params;
  params.sim_traj_path = "../data/tum_corridor1_512_16_okvis.txt";
  std::shared_ptr<ov_msckf::Simulator> sim = std::make_shared<ov_msckf::Simulator>(params);

  // Get initial state
  Eigen::Matrix<double, 17, 1> imustate;
  bool success = sim->get_state(sim->current_timestamp(), imustate);
  if (!success) {
    PRINT_ERROR(RED "[SIM]: Could not initialize the filter to the first state\n" RESET);
    PRINT_ERROR(RED "[SIM]: Did the simulator load properly???\n" RESET);
    std::exit(EXIT_FAILURE);
  }
  // Since the state time is in the camera frame of reference
  // Subtract out the imu to camera time offset
  imustate(0, 0) -= sim->get_true_parameters().calib_camimu_dt;

  while (sim->ok()) {
    // IMU: get the next simulated IMU measurement if we have it
    double time_imu;
    Eigen::Vector3d wm;
    Eigen::Vector3d am;
    bool hasimu = sim->get_next_imu(time_imu, wm, am);

    // CAM: get the next simulated camera uv measurements if we have them
    double time_cam;
    std::vector<int> camids;
    std::vector<std::vector<std::pair<size_t, Eigen::VectorXf>>> feats;
    bool hascam = sim->get_next_cam(time_cam, camids, feats);
  }

  return 0;
}