# CMakeList.txt: CMake-Projekt für "CMakeProject1". Schließen Sie die Quelle ein, und definieren Sie
# projektspezifische Logik hier.
#
cmake_minimum_required (VERSION 3.8)

# Aktivieren Sie Hot Reload für MSVC-Compiler, sofern unterstützt.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("Dumbo")

# Fügen Sie der ausführbaren Datei dieses Projekts eine Quelle hinzu.
add_executable (Dumbo "main.cpp" "position.h" "uci.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Dumbo PROPERTY CXX_STANDARD 17)
endif()

# TODO: Fügen Sie bei Bedarf Tests hinzu, und installieren Sie Ziele.
