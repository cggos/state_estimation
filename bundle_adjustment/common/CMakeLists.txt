include_directories(flags)

add_library(bal_problem SHARED bal_problem.cpp)
add_library(parse_cmd   SHARED flags/command_args.cpp)
