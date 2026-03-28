cmake_minimum_required(VERSION 3.10)
project(app)

# 1. Set C++ Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 2. Local Headers
# This ensures #include <glad/glad.h> and #include <KHR/khrplatform.h> work
include_directories(include)

# 3. Find System Packages (Homebrew)
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm CONFIG REQUIRED)

# 4. macOS Specific Configuration
if(APPLE)
    # Silence "OpenGL is deprecated" warnings
    add_definitions(-DGL_SILENCE_DEPRECATION)
    
    # Define the necessary Apple Frameworks
    set(MACOS_FRAMEWORKS 
        "-framework CoreVideo" 
        "-framework IOKit" 
        "-framework Cocoa" 
        "-framework OpenGL"
    )
    
    # Hint for Homebrew paths on Apple Silicon (M1/M2/M3)
    # This helps CMake find packages in /opt/homebrew
    list(APPEND CMAKE_PREFIX_PATH "/opt/homebrew")
endif()

# 5. Define the Executable
# We include both your main code and the glad implementation
add_executable(${PROJECT_NAME} 
    # src/main.cpp # change according to what file should be compiled
    src/shadertriangle.cpp
    src/glad.c
)

# 6. Link Libraries
# This connects your code to the actual binary logic of the libraries
target_link_libraries(${PROJECT_NAME} PRIVATE 
    glfw 
    glm::glm
    OpenGL::GL 
    ${MACOS_FRAMEWORKS}
)

# 7. Print status (Optional: helpful for debugging)
message(STATUS "Opengl found: ${OPENGL_LIBRARIES}")
message(STATUS "GLFW3 found: ${GLFW3_LIBRARIES}")

# # Use this to copy the FOLDER 'shaders' into the build directory
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/shaders" 
     DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")