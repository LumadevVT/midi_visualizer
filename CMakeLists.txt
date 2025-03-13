cmake_minimum_required(VERSION 3.22)
project(midi_visualizer)

add_executable(midi_visualizer main.cpp)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)