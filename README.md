# Vulkan Tutorial

https://docs.vulkan.org/tutorial/latest/

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
