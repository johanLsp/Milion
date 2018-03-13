
        ##########    Copyright (C) 2017 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #######

cmake_minimum_required (VERSION 3.2.0)

project (Milion)

set ( CMAKE_CXX_STANDARD 11 )

set( PROJECT_ROOT_DIR ${CMAKE_CURRENT_LIST_DIR} )
set( PROJECT_MODULES_DIR ${CMAKE_CURRENT_LIST_DIR}/modules )
set( PROJECT_SOURCE_DIR ${PROJECT_ROOT_DIR}/src )

set( VST_INCLUDE_PATH "${PROJECT_MODULES_DIR}/vst3sdk" )

list( APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake" )

include( cmake/ci.cmake )
include( cmake/JUCE.cmake )

if(${CMAKE_PROJECT_NAME} STREQUAL ${PROJECT_NAME})
  include( cmake/colors.cmake )
endif()

set_directory_properties( PROPERTIES
  COMPILE_DEFINITIONS         NDEBUG
  COMPILE_DEFINITIONS_DEBUG   DEBUG
  COMPILE_DEFINITIONS_RELEASE NDEBUG
)


# Configuration ---------------------------------------------------------------------------------- #

setupCI( 1 0 3 ) # version
string( TIMESTAMP PROJECT_BUILD_DATE "%Y-%m-%d" )

set( JUCE_CONFIG_PLUGIN_NAME "Milion" )
set( JUCE_CONFIG_PLUGIN_DESC "Formant / FM Synthesizer" )
set( JUCE_CONFIG_PLUGIN_MANUFACTURER "Johan Lasperas" )
set( JUCE_CONFIG_PLUGIN_MANUFACTURER_URL "" )
set( JUCE_CONFIG_PLUGIN_MANUFACTURER_EMAIL "johan.lasperas@gmail.com" )
set( JUCE_CONFIG_PLUGIN_MANUFACTURER_CODE_STRING "JL" )
set( JUCE_CONFIG_PLUGIN_MANUFACTURER_CODE 0x4a4c ) # 'JL'
set( JUCE_CONFIG_PLUGIN_CODE_STRING "JLML" )
set( JUCE_CONFIG_PLUGIN_CODE 0x4a4c4d4c ) # 'JLML'

set( JUCE_CONFIG_IS_SYNTH 1 )
set( JUCE_CONFIG_WANTS_MIDI 1 )
set( JUCE_CONFIG_PRODUCES_MIDI 0 )
set( JUCE_CONFIG_IS_MIDI_EFFECT 0 )
set( JUCE_CONFIG_EDITOR_REQUIRES_KEYBOARD_FOCUS 0 )
set( JUCE_CONFIG_MAX_IN_CHANNELS 2 )
set( JUCE_CONFIG_MAX_OUT_CHANNELS 2 )
set( JUCE_CONFIG_PREFERRED_CHANNELS_CFG "{0,1}")

if( JUCE_CONFIG_IS_SYNTH EQUAL 1)
  set( JUCE_CONFIG_PLUGIN_VST_TYPE "kPlugCategSynth" )
  set( JUCE_CONFIG_PLUGIN_AU_TYPE "aumu" ) # Synth: "aumu", FX: "aufx"
else()
  set( JUCE_CONFIG_PLUGIN_VST_TYPE "kPlugCategEffect" )
  set( JUCE_CONFIG_PLUGIN_AU_TYPE "aufx" ) # Synth: "aumu", FX: "aufx"
endif()

configure_file (
  "${PROJECT_ROOT_DIR}/support/JuceHeader.h.in"
  "${PROJECT_BINARY_DIR}/JuceHeader.h"
)

configure_file (
  "${PROJECT_ROOT_DIR}/support/AppConfig.h.in"
  "${PROJECT_BINARY_DIR}/AppConfig.h"
)

MESSAGE( " ")
MESSAGE( " ")
MESSAGE( "${BoldYellow}${JUCE_CONFIG_PLUGIN_NAME} - ${JUCE_CONFIG_PLUGIN_DESC}${ColorReset}")
message( "[${PROJECT_BUILD_DATE}] v. ${PROJECT_VERSION_STRING} / ${PROJECT_BUILD_DESCRIPTION} ")
MESSAGE( " ")
MESSAGE( " ")

# Sources ---------------------------------------------------------------------------------------- #

set( UI_SOURCES

)
set( PLUGIN_SOURCES
  ${PROJECT_SOURCE_DIR}/MilionEditor.cpp
  ${PROJECT_SOURCE_DIR}/MilionEditor.h
  ${PROJECT_SOURCE_DIR}/MilionProcessor.cpp
  ${PROJECT_SOURCE_DIR}/MilionProcessor.h
  ${PROJECT_SOURCE_DIR}/Operator/BaseOperator.cpp
  ${PROJECT_SOURCE_DIR}/Operator/BaseOperator.h
  ${PROJECT_SOURCE_DIR}/Operator/FMOperator.cpp
  ${PROJECT_SOURCE_DIR}/Operator/FMOperator.h
  ${PROJECT_SOURCE_DIR}/Operator/FormantOperator.cpp
  ${PROJECT_SOURCE_DIR}/Operator/FormantOperator.h
  ${PROJECT_SOURCE_DIR}/Operator/OperatorContainer.cpp
  ${PROJECT_SOURCE_DIR}/Operator/OperatorContainer.h
  ${PROJECT_SOURCE_DIR}/Operator/EnvelopeGenerator.cpp
  ${PROJECT_SOURCE_DIR}/Operator/EnvelopeGenerator.h
  ${PROJECT_SOURCE_DIR}/Component/EnvelopeComponent.cpp
  ${PROJECT_SOURCE_DIR}/Component/EnvelopeComponent.h
  ${PROJECT_SOURCE_DIR}/Component/ComponentContainer.cpp
  ${PROJECT_SOURCE_DIR}/Component/ComponentContainer.h
  ${PROJECT_SOURCE_DIR}/Component/FMComponent.cpp
  ${PROJECT_SOURCE_DIR}/Component/FMComponent.h
  ${PROJECT_SOURCE_DIR}/Component/FormantComponent.cpp
  ${PROJECT_SOURCE_DIR}/Component/FormantComponent.h
  ${PROJECT_SOURCE_DIR}/Component/OperatorComponent.cpp
  ${PROJECT_SOURCE_DIR}/Component/OperatorComponent.h
  ${PROJECT_SOURCE_DIR}/Utils/SysexParser.cpp
  ${PROJECT_SOURCE_DIR}/Utils/SysexParser.h
  ${PROJECT_SOURCE_DIR}/Utils/Wavetable.cpp
  ${PROJECT_SOURCE_DIR}/Utils/Wavetable.h
)

source_group( "src\\ui" FILES ${UI_SOURCES} )
source_group( "src" FILES ${PLUGIN_SOURCES} )

add_definitions(-DWIN32_LEAN_AND_MEAN)

include_directories(
  ${PROJECT_BINARY_DIR}
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_ROOT_DIR}/support/JuceLibraryCode
  ${PROJECT_MODULES_DIR}
)

set( ALL_SOURCES ${UI_SOURCES} ${PLUGIN_SOURCES} )


# Targets ---------------------------------------------------------------------------------------- #

addJUCEPlugins( ${JUCE_CONFIG_PLUGIN_NAME} "${ALL_SOURCES}" )
