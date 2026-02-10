cmake_minimum_required(VERSION 3.28)
project(rest_api)

include(FetchContent)

# 1. Declare nlohmann_json
FetchContent_Declare(
    json
    URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
)

# 2. FIXED: Added the full repository path
FetchContent_Declare(
    json-schema-validator
    GIT_REPOSITORY https://github.com/pboettch/json-schema-validator
    GIT_TAG 2.3.0
)

# 3. Make available
FetchContent_MakeAvailable(json json-schema-validator)

# 4. Find Crow
find_package(Crow REQUIRED)

# 5. Add executable
add_executable(${PROJECT_NAME}
    src/main.cpp
    src/server.cpp
    src/handlers/base.cpp
    src/handlers/users.cpp
)

# 6. Link libraries
target_link_libraries(${PROJECT_NAME}
    PUBLIC
        Crow::Crow
        nlohmann_json::nlohmann_json
        nlohmann_json_schema_validator
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)
