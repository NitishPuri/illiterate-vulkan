# Vulkan Tutorial

This project is a basic Vulkan tutorial that demonstrates how to set up a Vulkan application using CMake. It includes a simple rendering loop and basic shaders.

## Project Structure

```
vulkan-tutorial
├── src
│   ├── main.cpp          # Entry point of the application
│   └── shaders
│       ├── shader.vert   # Vertex shader
│       └── shader.frag   # Fragment shader
├── CMakeLists.txt        # CMake configuration file
└── README.md             # Project documentation
```

## Requirements

- CMake (version 3.10 or higher)
- Vulkan SDK
- A C++ compiler that supports C++11 or higher

## Setup Instructions

1. **Install the Vulkan SDK**: Follow the instructions on the [Vulkan SDK website](https://vulkan.lunarg.com/) to download and install the SDK for your platform.

2. **Clone the repository**:
   ```
   git clone <repository-url>
   cd vulkan-tutorial
   ```

3. **Create a build directory**:
   ```
   mkdir build
   cd build
   ```

4. **Run CMake**:
   ```
   cmake ..
   ```

5. **Build the project**:
   ```
   cmake --build .
   ```

6. **Run the application**:
   ```
   ./vulkan-tutorial
   ```

## Overview

This tutorial covers the following topics:

- Initializing Vulkan
- Setting up a rendering loop
- Creating and using shaders
- Handling basic input and output

Feel free to explore the code and modify it to enhance your understanding of Vulkan!