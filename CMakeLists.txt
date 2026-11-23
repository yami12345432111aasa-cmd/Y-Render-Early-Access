cmake_minimum_required(VERSION 3.20)
project(YRender VERSION 0.1.0 LANGUAGES CXX)

option(YRENDER_BUILD_TESTS "Build basic tests" ON)

add_library(yrender_core
    src/hardware_intelligence.cpp
    src/memory_cache.cpp
    src/resource_predictor.cpp
    src/async_loader.cpp
    src/diagnostics.cpp
    src/evolution_engine.cpp
    src/media_analyzer.cpp
    src/service_manager.cpp
)
target_include_directories(yrender_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_features(yrender_core PUBLIC cxx_std_20)
if(MSVC)
    target_compile_options(yrender_core PRIVATE /W4 /permissive-)
else()
    target_compile_options(yrender_core PRIVATE -Wall -Wextra -Wpedantic)
endif()

# === DXGI para detección de GPU en Windows ===
if(WIN32)
    target_link_libraries(yrender_core PRIVATE dxgi)
endif()

add_executable(yrender src/main.cpp)
target_link_libraries(yrender PRIVATE yrender_core)

if(YRENDER_BUILD_TESTS)
    enable_testing()
    add_executable(yrender_tests tests/basic_tests.cpp)
    target_link_libraries(yrender_tests PRIVATE yrender_core)
    add_test(NAME yrender_basic_tests COMMAND yrender_tests)
endif()
