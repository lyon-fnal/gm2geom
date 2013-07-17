#  This file is the main CMakeLists.txt file for the @gm2geom@ package (an art module package)

# h2. Introduction 

#  Before running cmake, you must ensure that all external
#  packages are set up already (e.g. source all necessary setup
#  files, including the appropriate @setup_for_development@ script).

#  If you are copying this project to use as a template for a new project, you will need to make some minor changes. The 
#  areas where you would have to do so are marked with -->

#  This most important place where you would have to add to this file
#  is at the bottom.

# h2. Main code

# Ensure we are using a moden version of CMake
CMAKE_MINIMUM_REQUIRED (VERSION 2.8)

# h3. Project definition

# --> Define the name of this project (replace @gm2artexamples@ with your 
# project name - use all lowercase)
PROJECT (gm2geom)

# h3. Load build system modules

# cetbuildtools contains our cmake modules
set( CETBUILDTOOLS_VERSION $ENV{CETBUILDTOOLS_VERSION} )
if( NOT CETBUILDTOOLS_VERSION )
  message( FATAL_ERROR "ERROR: setup cetbuildtools to get the cmake modules" )
endif()
set( CMAKE_MODULE_PATH $ENV{CETBUILDTOOLS_DIR}/Modules ${CMAKE_MODULE_PATH} )

# Add any modules from this source necessary for building
set(CMAKE_MODULE_PATH $ENV{GM2SOFTWARETOOLS_DIR}/Modules ${CMAKE_MODULE_PATH} )

# Include the necessary macros
include(CetCMakeEnv)

# h3. Configure this product

# Configure
cet_cmake_env()

# Do the same compiler flag stuff as Art
cet_set_compiler_flags(DIAGS VIGILANT WERROR EXTRA_FLAGS -pedantic)
cet_have_qual( e2 )
if ( ${CET_HAVE_QUAL} )
  cet_add_compiler_flags(CXX -std=c++0x;-Wno-deprecated-declarations)
endif()
if ( ${APPLE} )
  cet_add_compiler_flags(CXX -undefined dynamic_lookup)
endif()
cet_report_compiler_flags()


# Locate some UPS areas (the versions are just minimum required) and setup include paths
# These are needed for Art stuff
find_ups_product(gm2softwaretools v201209_02)
find_ups_product(cetbuildtools v2_03_03)
find_ups_product(cpp0x v1_03_14)
find_ups_product(libsigcpp v2_2_10)
find_ups_product(cppunit v1_12_1)
find_ups_product(clhep v2_1_2_4)
find_ups_product(cetlib v1_03_14)
find_ups_product(gccxml v0_9_20120702)
find_ups_product(art v1_02_02)
find_ups_product(fhiclcpp v2_17_01)
find_ups_product(messagefacility v1_10_15)
find_ups_root( v5_34_01 )
find_ups_boost( v1_50_0 filesystem
                        program_options
                        regex
                        thread
                        unit_test_framework
 )


# Make sure we have gcc
cet_check_gcc()

# Configure the include search path - explicitly add these sigcpp includes
include_directories ( $ENV{LIBSIGCPP_INC}/sigc++-2.0 )
include_directories ( $ENV{LIBSIGCPP_LIB}/sigc++-2.0/include )

# Add certain libraries not covered by the @find_ups_product@ commands
find_library( FHICLCPP NAMES fhiclcpp PATHS $ENV{FHICLCPP_LIB} )

find_library( MF_MESSAGELOGGER NAMES MF_MessageLogger
                              PATHS $ENV{MESSAGEFACILITY_LIB} )
find_library( MF_MESSAGESERVICE NAMES MF_MessageService
                              PATHS $ENV{MESSAGEFACILITY_LIB} )
find_library( MF_UTILITIES NAMES MF_Utilities
                              PATHS $ENV{MESSAGEFACILITY_LIB} )
find_library( MF_EXTENSIONS NAMES MF_ExtModules
                              PATHS $ENV{MESSAGEFACILITY_LIB} )

# Ensure system libraries are not used for the following

find_library( SIGC    NAMES sigc-2.0  PATHS $ENV{LIBSIGCPP_LIB} NO_DEFAULT_PATH )

find_library( CPPUNIT NAMES cppunit   PATHS $ENV{CPPUNIT_LIB}   NO_DEFAULT_PATH )

# Macros for art_make and simple plugins
include(ArtDictionary)
include(BuildPlugins)
include(ArtMake)

# Build these directories
add_subdirectory( strawtracker )
add_subdirectory( Core )
add_subdirectory( ups )
add_subdirectory( test )


# Packaging facility
include(UseCPack)

