#add_library(pugixml pugixml.hpp pugixml.cpp pugiconfig.hpp)##

add_library(pugixml pugixml.hpp pugixml.cpp pugiconfig.hpp)
#add_executable(${PROJECT_NAME} "pugixml.hpp" "pugixml.cpp" )

install(TARGETS pugixml DESTINATION lib)
install(FILES pugixml.hpp DESTINATION include)
