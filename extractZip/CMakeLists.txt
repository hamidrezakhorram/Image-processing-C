cmake_minimum_required(VERSION 3.23)
project(extractZip C)

set(CMAKE_C_STANDARD 99)
# Set vcpkg toolchain file
set(CMAKE_TOOLCHAIN_FILE "C:/Users/DANA-RAYANEH/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")

# Set the prefix path for vcpkg packages
set(CMAKE_PREFIX_PATH "C:/Users/DANA-RAYANEH/vcpkg/installed/x64-windows")

# Find and link minizip
set(minizip_DIR "C:/Users/DANA-RAYANEH/vcpkg/installed/x64-windows/share/minizip")
find_package(unofficial-minizip CONFIG REQUIRED)
add_executable(extractZip main.c)
# Link libraries (minizip and OpenCV)
target_link_libraries(extractZip PRIVATE unofficial::minizip::minizip )

# Include OpenCV headers
target_include_directories(extractZip PRIVATE )
