CMAKE_MINIMUM_REQUIRED(VERSION 3.18)

#项目名
project(DesignPatternLearn)

#指定源文件当前目录所有的
aux_source_directory(. SOURCES)

#指定源文件
add_executable(${PROJECT_NAME}
    ${SOURCES}
)

SET(CMAKE_SHARED_LINKER_FLAGS "-fobjc-arc -fobjc-link-runtime ${CMAKE_SHARED_LINKER_FLAGS}")

# 编译时预定义宏
target_compile_definitions(${PROJECT_NAME}
    PRIVATE _UNICODE
    PRIVATE UNICODE
)

# 基础编译配置
target_compile_features(${PROJECT_NAME}
    PUBLIC cxx_std_17
    PUBLIC c_std_11
    )
	
if(MSVC)
    # vs编译时使用utf-8编码，避免有中文时的编译问题ı
    target_compile_options(${PROJECT_NAME}
        PRIVATE "$<$<CXX_COMPILER_ID:MSVC>:/source-charset:utf-8>"
    )
else()
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fms-extensions")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fms-extensions  -fvisibility=hidden -fvisibility-inlines-hidden")
endif()

# 指定预编译头
target_precompile_headers(${PROJECT_NAME}
    PRIVATE "stdafx.h"
)

# 指定输出目录
install(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION "${CMAKE_CURRENT_SOURCE_DIR}/Release")

