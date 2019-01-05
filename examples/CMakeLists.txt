add_executable(
    sphereTest
    ${PROJECT_SOURCE_DIR}/examples/sphereTest.cpp
    ${PROJECT_SOURCE_DIR}/src/utils.cpp
)

add_executable(
    ackleyTest
    ${PROJECT_SOURCE_DIR}/examples/ackleyTest.cpp
    ${PROJECT_SOURCE_DIR}/src/utils.cpp
)

add_executable(
    griewankTest
    ${PROJECT_SOURCE_DIR}/examples/griewankTest.cpp
    ${PROJECT_SOURCE_DIR}/src/utils.cpp
)

add_executable(
    rastriginTest
    ${PROJECT_SOURCE_DIR}/examples/rastriginTest.cpp
    ${PROJECT_SOURCE_DIR}/src/utils.cpp
)

add_executable(
    rosenbrokTest
    ${PROJECT_SOURCE_DIR}/examples/rosenbrokTest.cpp
    ${PROJECT_SOURCE_DIR}/src/utils.cpp
)

add_executable(
    graph55
    ${PROJECT_SOURCE_DIR}/examples/graph55.cpp
    ${PROJECT_SOURCE_DIR}/src/mmas.cpp
    ${PROJECT_SOURCE_DIR}/src/utils.cpp
)