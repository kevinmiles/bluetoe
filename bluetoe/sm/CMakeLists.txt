add_library(bluetoe_security_manager INTERFACE)
add_library(bluetoe::sm ALIAS bluetoe_security_manager)

target_include_directories(bluetoe_security_manager INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
