cmake_minimum_required(VERSION 3.6)

project(learn)

set(CMAKE_CXX_STANDARD 11)

list(APPEND CMAKE_INSTALL_RPATH "$ORIGIN/../../workflow/_lib")
set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin)

add_executable(${PROJECT_NAME}
        ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
        )

target_include_directories(${PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/../workflow/_include
)

target_link_directories(${PROJECT_NAME} PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/../workflow/_lib
)

target_link_libraries(${PROJECT_NAME} PRIVATE
        -ldl
        -lpthread
        -lssl
        -lcrypto
        -lworkflow
)
