cmake_minimum_required(VERSION 3.13)
project(MachineLearning)

set(CMAKE_CXX_STANDARD 14)

find_package( Boost REQUIRED )

include_directories(
        ${BOOST_INCLUDE_DIRS}
)

include_directories("Machine Learning")

add_executable(MachineLearning
        "Machine Learning/Classifier.h"
        "Machine Learning/Euclidean.cpp"
        "Machine Learning/Euclidean.h"
        "Machine Learning/Instance.cpp"
        "Machine Learning/Instance.h"
        "Machine Learning/KFoldCrossValidation.cpp"
        "Machine Learning/KFoldCrossValidation.h"
        "Machine Learning/Main.cpp"
        "Machine Learning/Metric.h"
        "Machine Learning/Types.h"
        "Machine Learning/Utils.cpp"
        "Machine Learning/Utils.h"
        "Machine Learning/Validation.h" "Machine Learning/KNN.h" "Machine Learning/KNN.cpp" "Machine Learning/HEOM.cpp" "Machine Learning/HEOM.h" "Machine Learning/HVDM.cpp" "Machine Learning/HVDM.h")

