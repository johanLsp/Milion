/*


*/

#pragma once

//==============================================================================
// [BEGIN_USER_CODE_SECTION]@user_code_section@// [END_USER_CODE_SECTION]

/*
  ==============================================================================

   In accordance with the terms of the JUCE 5 End-Use License Agreement, the
   JUCE Code in SECTION A cannot be removed, changed or otherwise rendered
   ineffective unless you have a JUCE Indie or Pro license, or are using JUCE
   under the GPL v3 license.

   End User License Agreement: www.juce.com/juce-5-licence
  ==============================================================================
*/

// BEGIN SECTION A

#ifndef JUCE_DISPLAY_SPLASH_SCREEN
 #define JUCE_DISPLAY_SPLASH_SCREEN @display_splash_screen@
#endif

#ifndef JUCE_REPORT_APP_USAGE
 #define JUCE_REPORT_APP_USAGE @report_app_usage@
#endif

// END SECTION A

#define JUCE_USE_DARK_SPLASH_SCREEN @use_dark_splash_screen@

//==============================================================================
@module_available_defines@
//==============================================================================
#ifndef    JUCE_STANDALONE_APPLICATION
 #ifdef JucePlugin_Build_Standalone
  #define  JUCE_STANDALONE_APPLICATION JucePlugin_Build_Standalone
 #else
  #define  JUCE_STANDALONE_APPLICATION @is_standalone_application@
 #endif
#endif

#define JUCE_GLOBAL_MODULE_SETTINGS_INCLUDED 1

#if JucePlugin_Build_VST3 && (_WIN32 || _WIN64)
 #define JUCE_VST3_CAN_REPLACE_VST2 0
#endif

//#define JUCE_CORE_INCLUDE_NATIVE_HEADERS 1
//#define NOGDI

@config_flags_defines@@audio_plugin_settings_defines@
