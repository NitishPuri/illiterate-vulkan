project(illiterate-vulkan)
cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MT")
endif()

find_package(Vulkan REQUIRED)

include_directories(${Vulkan_INCLUDE_DIRS})

################## Dependencies ##################
# GLFW
set(GLFW_PATH ${PROJECT_SOURCE_DIR}/ext/glfw-3.4.bin.WIN64)
set(GLFW_INCLUDE ${GLFW_PATH}/include)
set(GLFW_LIB_DIR ${GLFW_PATH}/lib-vc2022)

#STB
set(STB_INCLUDE ${PROJECT_SOURCE_DIR}/ext/stb)

set(TINYOBJLOADER_INCLUDE ${PROJECT_SOURCE_DIR}/ext/tinyobjloader)
################## Dependencies ##################



file(GLOB SOURCES
    src/main.cpp
    src/shaders/*.vert
    src/shaders/*.frag
)

# set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /NODEFAULTLIB:MSVCRT")

add_executable(illiterate-vulkan ${SOURCES})


target_include_directories(illiterate-vulkan PUBLIC
    ${GLFW_INCLUDE}
    ${STB_INCLUDE}
    ${TINYOBJLOADER_INCLUDE}
)
target_link_directories(${PROJECT_NAME} PUBLIC ${GLFW_LIB_DIR})
target_link_libraries(illiterate-vulkan
    glfw3
    ${Vulkan_LIBRARIES}
)


#############Compute
add_executable(illiterate-compute src/compute.cpp)
target_include_directories(illiterate-compute PUBLIC
    ${GLFW_INCLUDE}
)
target_link_directories(illiterate-compute PUBLIC ${GLFW_LIB_DIR})
target_link_libraries(illiterate-compute
    glfw3
    ${Vulkan_LIBRARIES}
)
#############Compute


# add_executable(vulkan-tut src/main2.cpp)
# target_include_directories(vulkan-tut PUBLIC
#     ${GLFW_INCLUDE}
#     ${STB_INCLUDE}
# )
# target_link_directories(vulkan-tut PUBLIC ${GLFW_LIB_DIR})
# target_link_libraries(vulkan-tut
#     glfw3
#     ${Vulkan_LIBRARIES}
# )

