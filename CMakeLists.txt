cmake_minimum_required(VERSION 3.19)
project(observer_sample LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)

include(FetchContent)
FetchContent_Declare(
  sigslot
  GIT_REPOSITORY https://github.com/palacaze/sigslot
  GIT_TAG        19a6f0f5ea11fc121fe67f81fd5e491f2d7a4637 # v1.2.0
)
FetchContent_Declare(
  Boost
  GIT_REPOSITORY https://github.com/boostorg/boost.git
  GIT_TAG boost-1.80.0
  GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(sigslot Boost)

set(CMAKE_CXX_STANDARD 20)

qt_standard_project_setup()

qt_add_executable(observer_sample
    WIN32 MACOSX_BUNDLE
    src/main.cpp
    src/EventSender.hpp
    src/gui/MainWindow.cpp
    src/gui/MainWindow.hpp
    src/gui/MainWindow.ui
    src/core/BookmarkNode.hpp src/core/BookmarkNode.cpp
    src/core/BookmarkNode_Root.hpp src/core/BookmarkNode_Root.cpp
    src/core/BookmarkNode_Folder.hpp src/core/BookmarkNode_Folder.cpp
    src/core/BookmarkNode_Url.hpp src/core/BookmarkNode_Url.cpp
    src/core/BookmarkNodeEvent.hpp
    src/gui/BookmarkTreeWidget.hpp src/gui/BookmarkTreeWidget.cpp src/gui/BookmarkTreeWidget.ui
    src/ctrl/BookmarkItemModelTree.hpp src/ctrl/BookmarkItemModelTree.cpp
    src/ctrl/System.hpp src/ctrl/System.cpp
    src/ctrl/BookmarkManager.hpp src/ctrl/BookmarkManager.cpp
    src/ctrl/BookmarkManagerEvent.hpp
    src/gui/BookmarkToolBar.hpp src/gui/BookmarkToolBar.cpp
    src/core/BookmarkKind.hpp
    src/core/BookmarkNode_UrlEvent.hpp
    src/core/BookmarkNodeVisitor.hpp src/core/BookmarkNodeVisitor.cpp
    src/EventMacro.hpp
    src/gui/BookmarkUrlFactoryDialog.hpp src/gui/BookmarkUrlFactoryDialog.cpp src/gui/BookmarkUrlFactoryDialog.ui
    src/gui/BookmarkEditor.hpp src/gui/BookmarkEditor.cpp src/gui/BookmarkEditor.ui
    src/EventBase.hpp
    src/EventBase.cpp
    src/EventConnection.hpp
)

target_link_libraries(observer_sample
    PRIVATE
        Qt::Core
        Qt::Widgets
        Pal::Sigslot
        Boost::signals2
)
target_include_directories(observer_sample
    PUBLIC src
)

include(GNUInstallDirs)

install(TARGETS observer_sample
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET observer_sample
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
