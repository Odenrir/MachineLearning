cmake_minimum_required(VERSION 3.13)
project(MachineLearning)

set(CMAKE_CXX_STANDARD 14)

set(Boost_USE_MULTITHREADED ON)
if (APPLE)
    #set(Boost_USE_STATIC_LIBS       ON)
    set(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} "/usr/local/Cellar/boost/1.72.0")
    set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "/usr/local/Cellar/boost/1.72.0/lib")
    set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} "-I /opt/anaconda3/lib/python3.7/site-packages/numpy/core/include")
    set(PYTHON_INCLUDE_DIR /opt/anaconda3/include/python3.7m)
    set(PYTHON_LIBRARY /opt/anaconda3/lib/libpython3.7m.dylib)
    #set(PYTHON_INCLUDE_DIR /usr/local/Cellar/python/3.7.6_1/Frameworks/Python.framework/Versions/3.7/Headers)
    #set(PYTHON_LIBRARY /usr/local/Cellar/python/3.7.6_1/Frameworks/Python.framework/Versions/3.7/lib/libpython3.7m.dylib)
    #set(Boost_ROOT /usr/local/Cellar/boost/1.72.0)
    #set(Boost_INCLUDE_DIR /usr/local/Cellar/boost/1.72.0/include)
    #set(Boost_INCLUDE_DIRS /usr/local/Cellar/boost/1.72.0/include)
    #set(Boost_LIBRARY_DIRS /usr/local/Cellar/boost/1.72.0/lib)
    #set(Boost_LIBRARIES /usr/local/Cellar/boost/1.72.0/lib)
    #set(Boost_LIBRARY_DIRS /usr/local/lib)
    #set(Boost_LIBRARIES /usr/local/lib)
    find_package(Boost REQUIRED)
else ()
    find_package(Boost REQUIRED)
endif ()

if (Boost_FOUND)
    message("Boost config:")
    message("   BOOST INCLUDE DIR = ${Boost_INCLUDE_DIR}")
    message("   BOOST INCLUDE DIRS = ${Boost_INCLUDE_DIRS}")
    message("   BOOST LIB DIR = ${Boost_LIBRARY_DIRS}")
    message("   BOOST LIBRARIES = ${Boost_LIBRARIES}")
    message("   BOOST FOUND = ${Boost_FOUND}")
    message("   BOOST VERSION = ${Boost_VERSION}")
    include_directories(${Boost_INCLUDE_DIR})
    add_definitions("-DHAS_BOOST")
endif ()

find_package(PythonLibs REQUIRED)
if (PYTHONLIBS_FOUND)
    message("PythonLibs config:")
    message("   PYTHONLIBS INCLUDE DIRS = ${PYTHON_INCLUDE_DIRS}")
    message("   PYTHONLIBS LIBRARIES = ${PYTHON_LIBRARIES}")
    message("   PYTHONLIBS FOUND = ${PYTHONLIBS_FOUND}")
    message("   PYTHONLIBS VERSION = ${PYTHONLIBS_VERSION_STRING}")
    include_directories(${PYTHON_INCLUDE_DIRS})
endif ()

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
        "Machine Learning/Validation/Validation.h" "Machine Learning/Classify/kNN.h" "Machine Learning/Classify/kNN.cpp" "Machine Learning/Metrics/HEOM.cpp" "Machine Learning/Metrics/HEOM.h" "Machine Learning/Metrics/HVDM.cpp" "Machine Learning/Metrics/HVDM.h" "Machine Learning/Clustering/Clustering.h" "Machine Learning/Clustering/kMeans.cpp" "Machine Learning/Clustering/kMeans.h" "Machine Learning/Misc/mathplotlibcpp.h" "Machine Learning/Misc/Plot.cpp" "Machine Learning/Misc/Plot.h" "Machine Learning/InstanceSelection/InstanceSelection.h" "Machine Learning/InstanceSelection/PSR.h" "Machine Learning/InstanceSelection/PSR.cpp" "Machine Learning/InstanceSelection/PSC.cpp" "Machine Learning/InstanceSelection/PSC.h" "Machine Learning/Clustering/PAM.cpp" "Machine Learning/Clustering/PAM.h" "Machine Learning/InstanceSelection/ISRB.cpp" "Machine Learning/InstanceSelection/ISRB.h" "Machine Learning/InstanceSelection/LSSm.cpp" "Machine Learning/InstanceSelection/LSSm.h" "Machine Learning/InstanceSelection/LSCo.cpp" "Machine Learning/InstanceSelection/LSCo.h" "Machine Learning/InstanceSelection/LSBo.cpp" "Machine Learning/InstanceSelection/LSBo.h" "Machine Learning/InstanceSelection/IBR.cpp" "Machine Learning/InstanceSelection/IBR.h" "Machine Learning/InstanceSelection/InstanceSelection.cpp" "Machine Learning/InstanceSelection/ISBM.cpp" "Machine Learning/InstanceSelection/ISBM.h" "Machine Learning/InstanceSelection/ENN.cpp" "Machine Learning/InstanceSelection/ENN.h" "Machine Learning/Classify/Classifier.cpp" "Machine Learning/Clustering/Clustering.cpp" "Machine Learning/InstanceSelection/ISRT.cpp" "Machine Learning/InstanceSelection/ISRT.h")

target_link_libraries(MachineLearning ${PYTHON_LIBRARIES})