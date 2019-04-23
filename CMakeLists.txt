cmake_minimum_required(VERSION 3.13)
project(MachineLearning)

set(CMAKE_CXX_STANDARD 14)

find_package(Boost REQUIRED)
find_package(PythonLibs REQUIRED)

include_directories(${PYTHON_INCLUDE_DIRS})
include_directories(${BOOST_INCLUDE_DIRS})

include_directories("Machine Learning")

add_executable(MachineLearning
        "Machine Learning/Classify/Classifier.h"
        "Machine Learning/Metrics/Euclidean.cpp"
        "Machine Learning/Metrics/Euclidean.h"
        "Machine Learning/Misc/Instance.cpp"
        "Machine Learning/Misc/Instance.h"
        "Machine Learning/Validation/kFoldCrossValidation.cpp"
        "Machine Learning/Validation/kFoldCrossValidation.h"
        "Machine Learning/Main.cpp"
        "Machine Learning/Metrics/Metric.h"
        "Machine Learning/Misc/Types.h"
        "Machine Learning/Misc/Utils.cpp"
        "Machine Learning/Misc/Utils.h"
        "Machine Learning/Validation/Validation.h" "Machine Learning/Classify/kNN.h" "Machine Learning/Classify/kNN.cpp" "Machine Learning/Metrics/HEOM.cpp" "Machine Learning/Metrics/HEOM.h" "Machine Learning/Metrics/HVDM.cpp" "Machine Learning/Metrics/HVDM.h" "Machine Learning/Clustering/Clustering.h" "Machine Learning/Clustering/kMeans.cpp" "Machine Learning/Clustering/kMeans.h" "Machine Learning/Misc/mathplotlibcpp.h" "Machine Learning/Misc/Plot.cpp" "Machine Learning/Misc/Plot.h" "Machine Learning/InstanceSelection/InstanceSelection.h" "Machine Learning/InstanceSelection/PSR.h" "Machine Learning/InstanceSelection/PSR.cpp" "Machine Learning/InstanceSelection/PSC.cpp" "Machine Learning/InstanceSelection/PSC.h" "Machine Learning/Clustering/PAM.cpp" "Machine Learning/Clustering/PAM.h")

target_link_libraries(MachineLearning ${PYTHON_LIBRARIES})