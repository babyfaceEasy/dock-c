cmake_minimum_required(VERSION 3.28)
project(rest_api)

# find the libs
find_package(Crow REQUIRED)

# link source files
add_executable(${PROJECT_NAME}
    src/main.cpp
)

# link libs
target_link_libraries(${PROJECT_NAME}
    PUBLIC
        Crow::Crow
)

# link header files (include dir)
target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)