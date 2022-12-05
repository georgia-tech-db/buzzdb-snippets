# ---------------------------------------------------------------------------
# SOURCE
# ---------------------------------------------------------------------------

file(GLOB_RECURSE SRC_CC 
	 ${CMAKE_SOURCE_DIR}/src/*.cc 
	 ${CMAKE_SOURCE_DIR}/src/*/*.cc 
	 ${CMAKE_SOURCE_DIR}/src/*/*/*.cc
     ${CMAKE_SOURCE_DIR}/src/*/*/*/*.cc
)

file(GLOB_RECURSE INCLUDE_H 
	 ${CMAKE_SOURCE_DIR}/src/include/*.h 
	 ${CMAKE_SOURCE_DIR}/src/include/*/*.h 
	 ${CMAKE_SOURCE_DIR}/src/include/*/*/*.h 
)

# ---------------------------------------------------------------------------
# Includes
# ---------------------------------------------------------------------------

set(
    INCLUDE_H
    ${CMAKE_SOURCE_DIR}/src/include
    ${CMAKE_SOURCE_DIR}/test/unit/include
    ${CMAKE_SOURCE_DIR}    
)

set(
	SYSTEM_INCLUDE_H
    ${LLVM_INCLUDE_DIRS}
    ${GTEST_INCLUDE_DIR}
    ${GFLAGS_INCLUDE_DIR}
    ${BENCHMARK_INCLUDE_DIR}
    ${FLEX_INCLUDE_DIRS}
    ${CMAKE_CURRENT_BINARY_DIR}
)

include_directories(
	"${INCLUDE_H}"
)

include_directories(
	SYSTEM "${SYSTEM_INCLUDE_H}"
)

# ---------------------------------------------------------------------------
# Library
# ---------------------------------------------------------------------------

set(SHARED_LIBRARY "buzzdb")

add_library(${SHARED_LIBRARY} STATIC 
            ${SRC_CC} 
            mc_driver.cpp
            ${INCLUDE_H}
			${FLEX_mc_lexer_OUTPUTS}
    		${BISON_mc_parser_OUTPUTS}
)

target_link_libraries(
    ${SHARED_LIBRARY}
    gflags
    Threads::Threads
    ${LLVM_LIBS}
    ${LLVM_LDFLAGS}
)
