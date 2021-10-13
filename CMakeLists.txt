cmake_minimum_required (VERSION 3.1)

project (Glover C)

# Check the OS -----------------------------------------------------------------
if(NOT UNIX)
	message(FATAL_ERROR " Only Unix based systems are supported")
endif()

# Set the compiler flags ------------------------------------------------------
set(CMAKE_C_FLAGS "-O2 -Wall")

# Build the example -----------------------------------------------------------
add_executable(Glover
	Glover/main.c
)

# Link the math library --------------------------------------------------------
target_link_libraries(Glover m)

# Copy the config file ---------------------------------------------------------
file(COPY "Glover/hand.config" DESTINATION ".")

