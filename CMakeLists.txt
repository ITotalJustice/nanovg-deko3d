cmake_minimum_required(VERSION 3.13)

project(nanovg LANGUAGES C CXX)

option(NANOVG_DEBUG OFF "link against deko3dd")
option(NANOVG_NO_JPEG OFF "toggle build option for stbi")
option(NANOVG_NO_PNG OFF "toggle build option for stbi")
option(NANOVG_NO_BMP OFF "toggle build option for stbi")
option(NANOVG_NO_PSD OFF "toggle build option for stbi")
option(NANOVG_NO_TGA OFF "toggle build option for stbi")
option(NANOVG_NO_GIF OFF "toggle build option for stbi")
option(NANOVG_NO_HDR OFF "toggle build option for stbi")
option(NANOVG_NO_PIC OFF "toggle build option for stbi")
option(NANOVG_NO_PNM OFF "toggle build option for stbi")
option(NANOVG_STBI_STATIC ON "make stbi static")
option(NANOVG_STBTT_STATIC ON "make stbtt static")

add_library(nanovg
    source/framework/CIntrusiveTree.cpp
    source/framework/CMemPool.cpp
    source/framework/CShader.cpp
    source/dk_renderer.cpp
    source/nanovg.c
)

target_include_directories(nanovg PUBLIC include)

if (NANOVG_NO_JPEG)
    target_compile_definitions(nanovg PRIVATE STBI_NO_JPEG)
endif()

if (NANOVG_NO_PNG)
    target_compile_definitions(nanovg PRIVATE STBI_NO_PNG)
endif()

if (NANOVG_NO_BMP)
    target_compile_definitions(nanovg PRIVATE STBI_NO_BMP)
endif()

if (NANOVG_NO_PSD)
    target_compile_definitions(nanovg PRIVATE STBI_NO_PSD)
endif()

if (NANOVG_NO_TGA)
    target_compile_definitions(nanovg PRIVATE STBI_NO_TGA)
endif()

if (NANOVG_NO_GIF)
    target_compile_definitions(nanovg PRIVATE STBI_NO_GIF)
endif()

if (NANOVG_NO_HDR)
    target_compile_definitions(nanovg PRIVATE STBI_NO_HDR)
endif()

if (NANOVG_NO_PIC)
    target_compile_definitions(nanovg PRIVATE STBI_NO_PIC)
endif()

if (NANOVG_NO_PNM)
    target_compile_definitions(nanovg PRIVATE STBI_NO_PNM)
endif()

if (NANOVG_STBI_STATIC)
    target_compile_definitions(nanovg PRIVATE NANOVG_STBI_STATIC)
endif()

if (NANOVG_STBTT_STATIC)
    target_compile_definitions(nanovg PRIVATE NANOVG_STBTT_STATIC)
endif()

set_target_properties(nanovg PROPERTIES
    C_STANDARD 99
    C_EXTENSIONS ON
    CXX_STANDARD 20
    CXX_EXTENSIONS ON
)

find_path(glm_inc glm REQUIRED)

if (NANOVG_DEBUG)
    find_library(deko3d_lib deko3dd REQUIRED)
    message(STATUS "linking against debug deko3dd")
else()
    find_library(deko3d_lib deko3d REQUIRED)
    message(STATUS "linking against release deko3d")
endif()

target_link_libraries(nanovg PRIVATE
    ${deko3d_lib}
)

target_include_directories(nanovg PRIVATE
    include
    include/nanovg
    include/nanovg/framework
    ${glm_inc}
)

nx_add_shader_program(fill_aa_fsh shaders/fill_aa_fsh.glsl frag)
nx_add_shader_program(fill_fsh shaders/fill_fsh.glsl frag)
nx_add_shader_program(fill_vsh shaders/fill_vsh.glsl vert)
