cmake_minimum_required (VERSION 3.20)
project(PersonalCpp)
add_executable (PersonalCpp main.cpp "personal.h" "personal.cpp")

set(PostgreSQL_ADDITIONAL_VERSIONS "16")

add_subdirectory ("C:\\Program Files\\libpq++\\libpqxx-7.9.1\\libpqxx-7.9.1" libpqxx-build)

target_link_libraries (PersonalCpp PRIVATE pqxx)

target_compile_features(PersonalCpp PRIVATE cxx_std_17)