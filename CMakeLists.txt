cmake_minimum_required(VERSION 3.16)

project(AutoConnectUniVPN VERSION 1.0.8 LANGUAGES CXX)

set(CMAKE_PREFIX_PATH "F:/Programme/Tools/Qt/6.7.2/llvm-mingw_64" CACHE PATH "Prefix path")

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools)

set(TS_FILES AutoConnectUniVPN_de_DE.ts)


####### Personal Stuff ######

# OpenSSL-Pfade angeben
set(OPENSSL_ROOT_DIR "libs/OpenSSL3_Win64/")
find_package(OpenSSL REQUIRED Crypto)

add_library(CCOTP libs/CCOTP/otp_static.cpp libs/CCOTP/otpuri_static.cpp libs/CCOTP/ccotp.cpp libs/CCOTP/totp.cpp libs/CCOTP/totp_exceptions.h)
add_library(CiscoVPN libs/CiscoVPN/CiscoVPN.cpp libs/CiscoVPN/CiscoCommandGenerator.cpp libs/CiscoVPN/LogProgressPhrases.h)
add_library(Encoder libs/Encoder/EncDec.cpp libs/Encoder/EncDecExceptions.h)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp mainwindow.h mainwindow.ui
        src/UserData.cpp src/UserData.h
        src/UiManager.cpp src/UiManager.h
        dialogs/SaveDialog/savedialog.h dialogs/SaveDialog/savedialog.cpp dialogs/SaveDialog/savedialog.ui
        dialogs/InfoDialog/infodialog.h dialogs/InfoDialog/infodialog.cpp dialogs/InfoDialog/infodialog.ui
        dialogs/BugReportDialog/bugreportdialog.h dialogs/BugReportDialog/bugreportdialog.cpp dialogs/BugReportDialog/bugreportdialog.ui
        src/DataLoadingResult.h
        src/MainWindowExecutor.cpp src/MainWindowExecutor.h
        ${TS_FILES}
        res/AutoConnectUniVPN.rc    # should be the last argument
)

####### End Personal Stuff ######


if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(AutoConnectUniVPN
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET AutoConnectUniVPN APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation

    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    if(ANDROID)
        add_library(AutoConnectUniVPN SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(AutoConnectUniVPN
            ${PROJECT_SOURCES}
        )
    endif()

    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()


####### Personal Stuff ######

target_link_libraries(CCOTP PRIVATE OpenSSL::Crypto)  # if cannot link try also link OpenSSL::applink (only Windows)
target_link_libraries(CiscoVPN PRIVATE CCOTP)
target_link_libraries(Encoder PRIVATE OpenSSL::Crypto)
target_link_libraries(AutoConnectUniVPN PRIVATE Qt${QT_VERSION_MAJOR}::Widgets OpenSSL::Crypto CiscoVPN CCOTP Encoder)

# Macro for project version
target_compile_definitions(AutoConnectUniVPN PRIVATE
        PROJECT_VERSION="${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}"
)

####### End Personal Stuff ######


# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.AutoConnectUniVPN)
endif()
set_target_properties(AutoConnectUniVPN PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS AutoConnectUniVPN
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(AutoConnectUniVPN)
endif()
