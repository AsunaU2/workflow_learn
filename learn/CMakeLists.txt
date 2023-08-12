cmake_minimum_required(VERSION 3.6)

project(learn)

set(CMAKE_CXX_STANDARD 11)

list(APPEND CMAKE_INSTALL_RPATH "$ORIGIN/../../workflow/_lib")
set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin)

set(LEARN_LIST
        http_request
        http_request_timer_delay
)

set(LEARN_LIB
        dl
        pthread
        ssl
        crypto
        workflow
)

set(LEARN_INCLUDE_LIST
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/../workflow/_include
)

link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../workflow/_lib)

foreach(src ${LEARN_LIST})
	add_executable(${src} ${CMAKE_CURRENT_SOURCE_DIR}/src/${src}.cpp)
        target_include_directories(${src} PRIVATE ${LEARN_INCLUDE_LIST})
	target_link_libraries(${src} PUBLIC ${LEARN_LIB})
endforeach()

