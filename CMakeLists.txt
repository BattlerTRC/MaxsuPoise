cmake_minimum_required(VERSION 3.21)
project(MaxsuPoise VERSION 0.3.4 LANGUAGES CXX)

# Set your project version and specify C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Hide useless warnings...
add_definitions(-D_SILENCE_STDEXT_ARR_ITERS_DEPRECATION_WARNING)
set(BUILD_TESTS OFF)

# Include SKSEPlugin.cmake from the same directory
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(SKSEPlugin)

# Configure MSVC-specific settings for C++23
if(MSVC_VERSION GREATER_EQUAL 1936 AND MSVC_IDE) # 17.6+
    file(CONFIGURE OUTPUT "${CMAKE_BINARY_DIR}/Directory.Build.props" CONTENT [==[
    <Project>
      <ItemDefinitionGroup>
        <ClCompile>
          <BuildStlModules>false</BuildStlModules>
        </ClCompile>
      </ItemDefinitionGroup>
    </Project>
    ]==] @ONLY)
endif()
