cmake_minimum_required(VERSION 3.16)
project(Ex6)

set(CMAKE_CXX_STANDARD 11)

SET(CMAKE_CXX_FLAGS -pthread)

add_executable(Ex6 MainTrain.cpp SimpleAnomalyDetector.cpp TimeSeries.cpp anomaly_detection_util.cpp minCircle.cpp HybridAnomalyDetector.cpp CLI.cpp server.cpp)