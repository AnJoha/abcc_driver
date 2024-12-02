
# 1. Add your file abcc_system_adaptation.c to your target.
target_sources(<your_target> PRIVATE <"path/to/abcc_system_adaptation.c">)

# 2. Create a variable called ABCC_DRIVER_INCLUDE_DIRS with directories containing
# your personal include (.h) files related to the CompactCom Driver.
set(ABCC_DRIVER_INCLUDE_DIRS
  <"${PROJECT_SOURCE_DIR}/path/to/include_directory">
  <"${PROJECT_SOURCE_DIR}/path/to/another/include_directory">
  < ... >
)

# 3. Create a variable to the directory to where the ABCC Driver repository was cloned.
set(ABCC_DRIVER_DIR <"${PROJECT_SOURCE_DIR}/path/to/abcc_driver">)

# 4. Includes the standard CMake file in the CompactCom Driver.
include(${ABCC_DRIVER_DIR}/abcc_driver/abcc_driver.cmake)

# 5. Add directories containing include (.h) files related to the CompactCom Driver to your targer.
target_include_directories(host_application_exec PRIVATE ${ABCC_DRIVER_INCLUDE_DIRS})

# 6. Add the CompactCom Driver library to the user host application
# executable target.
target_link_libraries(<your_target> abcc_driver)