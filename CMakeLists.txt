cmake_minimum_required(VERSION 2.8.9)
set(EXTENSION_NAME MultiFrameTests)

#-----------------------------------------------------------------------------
# Prerequisites
#-----------------------------------------------------------------------------
if(NOT Slicer_SOURCE_DIR)
  set(EXTENSION_HOMEPAGE "http://www.slicer.org/slicerWiki/index.php/Slicer4:Developers:Projects:QtSlicer/Tutorials/ExtensionWriting")
  set(EXTENSION_CATEGORY "Examples")
  set(EXTENSION_STATUS "Beta")
  set(EXTENSION_DESCRIPTION "This is an example of Qt loadable module built as an extension")
  set(EXTENSION_DEPENDS CLIExtensionTemplate) # Specified as a space separated list or 'NA' if any

  find_package(Slicer REQUIRED)

  # Additional C/CXX flags - Should be defined before including Slicer_USE_FILE
  set(ADDITIONAL_C_FLAGS "" CACHE STRING "Additional ${EXTENSION_NAME} C Flags")
  set(ADDITIONAL_CXX_FLAGS "" CACHE STRING "Additional ${EXTENSION_NAME} CXX Flags")

  include(${Slicer_USE_FILE})

  set(EXTENSION_LICENSE_FILE ${Slicer_LICENSE_FILE})
  set(EXTENSION_README_FILE ${Slicer_README_FILE})

  include(SlicerEnableExtensionTesting)

endif()

set(MODULE_NAME dcmultiFixup)
SEMMacroBuildCLI(
  NAME ${MODULE_NAME}
	TARGET_LIBRARIES ${MODULE_TARGET_LIBRARIES} ${DCMTK_LIBRARIES}
  INCLUDE_DIRECTORIES ${MODULE_INCLUDE_DIRECTORIES}
  ADDITIONAL_SRCS ${MODULE_SRCS}
  )

set(MODULE_NAME MultiFrameReadWriteTest)
SEMMacroBuildCLI(
  NAME ${MODULE_NAME}
  TARGET_LIBRARIES ${MODULE_TARGET_LIBRARIES} ${DCMTK_LIBRARIES} ${ITK_LIBRARIES}
  INCLUDE_DIRECTORIES ${MODULE_INCLUDE_DIRECTORIES}
  ADDITIONAL_SRCS ${MODULE_SRCS}
  )
