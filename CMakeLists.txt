# $Id: CMakeLists.txt 68058 2013-03-13 14:47:43Z gcosmo $

#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(B1)

#----------------------------------------------------------------------------
# Find Geant4 package, activating all available UI and Vis drivers by default
# You can set WITH_GEANT4_UIVIS to OFF via the command line or ccmake/cmake-gui
# to build a batch mode only executable
#
option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()

#----------------------------------------------------------------------------
# Setup Geant4 include directories and compile definitions
# Setup include directory for this project
#
include(${Geant4_USE_FILE})
include_directories(${PROJECT_SOURCE_DIR}/include)


#----------------------------------------------------------------------------
# Locate sources and headers for this project
# NB: headers are included so they will show up in IDEs
#
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
#
add_executable(exampleB1 exampleB1.cc ${sources} ${headers})
target_link_libraries(exampleB1 ${Geant4_LIBRARIES})

#----------------------------------------------------------------------------
# Copy all scripts to the build directory, i.e. the directory in which we
# build B1. This is so that we can run the executable directly because it
# relies on these scripts being in the current working directory.
#
set(EXAMPLEB1_SCRIPTS
  exampleB1.in
  exampleB1.out
  init.mac
  init_vis.mac
  run1.mac
  run2.mac
  vis.mac
  vis2.mac
  cmd.file
  cmd2.file
  cmd3.file
  cmdDebug.file
  cosmics_0.data
  cosmics_2100.data
  cosmics_11300.data
  drawGE.C
  drawOSC.C
  drawISC.C
  drawWORLD.C
  drawG-IO.C
  drawG-O.C
  drawG-I.C
  drawALL.C
  drawBigGE.C
  drawBigOSC.C
  drawBigISC.C
  drawBigWORLD.C
  drawBigG-IO.C
  drawBigG-O.C
  drawBigG-I.C
  drawBigALL.C
  drawPart.C
  drawBigIorO.C
  drawIorO.C
  drawNeutronGE.C
  drawBigNeutronGE.C
  drawProtonGE.C
  drawBigProtonGE.C
  drawGammaGE.C
  drawBigGammaGE.C
  drawElectronGE.C
  drawBigElectronGE.C
  drawMuonGE.C
  drawBigMuonGE.C
  drawPionGE.C
  drawBigPionGE.C
  drawPositronGE.C
  drawBigPositronGE.C
  drawGeAll.C
  drawZPos.C
  drawInitENeutron.C
  drawInitEElectron.C
  drawInitEPositron.C
  drawInitEGamma.C
  drawInitEMuon.C
  drawInitEPion.C
  drawInitEProton.C
  drawInitEKaon.C
  drawInitDirNeutron.C
  drawInitDirElectron.C
  drawInitDirPositron.C
  drawInitDirGamma.C
  drawInitDirMuon.C
  drawInitDirPion.C
  drawInitDirProton.C
  drawInitDirKaon.C
  drawNeutron-IO.C
  drawProton-IO.C
  drawElectron-IO.C
  drawPositron-IO.C
  drawGamma-IO.C
  drawPion-IO.C
  drawKaon-IO.C
  drawMuon-IO.C
  drawUncaughtPart.C
  drawUncaughtInitENeutron.C
  drawPartGe.C
  drawUncaughtPartGe.C
)

foreach(_script ${EXAMPLEB1_SCRIPTS})
  configure_file(
    ${PROJECT_SOURCE_DIR}/${_script}
    ${PROJECT_BINARY_DIR}/${_script}
    COPYONLY
    )
endforeach()

#----------------------------------------------------------------------------
# For internal Geant4 use - but has no effect if you build this
# example standalone
#
add_custom_target(B1 DEPENDS exampleB1)

#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
install(TARGETS exampleB1 DESTINATION bin)


