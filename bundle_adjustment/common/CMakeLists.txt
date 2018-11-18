include_directories(tools flags)

add_library(BALProblem SHARED BALProblem.cpp)
add_library(ParseCmd   SHARED flags/command_args.cpp)
