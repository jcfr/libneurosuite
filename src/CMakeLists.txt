project(libklustersshared-src)
include_directories(${CMAKE_SOURCE_DIR}/src/gui/page)
set(libklustersshared_SRCS 
	gui/qrecentfileaction.cpp 
	gui/qcolorbutton.cpp 
	gui/page/qpagedialog.cpp
	gui/page/qpagemodel.cpp
	gui/page/qpageview.cpp
	gui/page/qpageview_p.cpp
	gui/page/qpagewidget.cpp
	gui/page/qpagewidgetmodel.cpp
	gui/page/qextenddialog.cpp
	gui/page/klusterseparator.cpp
        gui/dockarea.cpp
	gui/qextendtabwidget.cpp
	gui/qhelpviewer.cpp
        gui/klusterrubberband.cpp
	shared/channelcolors.cpp
	shared/itemcolors.cpp
	shared/zoomwindow.cpp
	shared/utilities.cpp
	)
if ( NOT Qt5Core_FOUND )
  # Add qstandardpaths and it os specific interface if qt5 is not available
  set(libklustersshared_SRCS ${libklustersshared_SRCS} libinqt5/qstandardpaths.cpp )

  if (UNIX)
    if(APPLE)
      set(libklustersshared_SRCS ${libklustersshared_SRCS} libinqt5/qstandardpaths_mac.cpp )
    else(APPLE)
      set(libklustersshared_SRCS ${libklustersshared_SRCS} libinqt5/qstandardpaths_unix.cpp )
    endif()
  endif()

  if (WIN32)
    set(libklustersshared_SRCS ${libklustersshared_SRCS} libinqt5/qstandardpaths_win.cpp )
  endif()

endif()

set (libklustersshared_HEADERS 
	libklustersshared_export.h 
	gui/qrecentfileaction.h 
	gui/qcolorbutton.h 
	gui/page/qpagedialog.h
       gui/page/qextenddialog.h
       gui/page/qpagewidget.h
       gui/page/qpagemodel.h
       gui/page/qpageview.h
       gui/page/qpagewidgetmodel.h
       gui/dockarea.h
       gui/qextendtabwidget.h
       gui/qhelpviewer.h
       gui/klusterrubberband.h
       shared/channelcolors.h
       shared/itemcolors.h
       shared/types.h
       shared/array.h
       shared/utilities.h
       shared/zoomwindow.h
       #Remove in qt5
       libinqt5/qstandardpaths.h
       )


SET(translate_files
    klustershared.ts
)

if(UPDATE_TRANSLATION)
QT4_CREATE_TRANSLATION(translateSrcs ../translate ${libklustersshared_SRCS} ${translate_files})
endif()

qt4_add_resources(libklustersshared_SRCS gui/libklustersshared-icons.qrc gui/libklustersshared-cursors.qrc)

add_library (klustersshared SHARED ${libklustersshared_SRCS} ${translateSrcs})
if(QT_QTWEBKIT_FOUND)
     target_link_libraries(klustersshared ${QT_QTWEBKIT_LIBRARY})
endif()


set_target_properties(klustersshared PROPERTIES
	        VERSION ${LIBKLUSTERSSHARED_VERSION}
		SOVERSION ${LIBKLUSTERSSHARED_VERSION_MAJOR}
		)

if(Qt5Core_FOUND)
  target_link_libraries( klustersshared Qt5::Widgets ${Qt5WebKitWidgets_LIBRARIES})
else(Qt5Core_FOUND)

  if(APPLE)
    find_library(CF_LIBRARIES NAMES CoreFoundation)
    find_library(CS_LIBRARIES NAMES CoreServices)
    target_link_libraries(klustersshared ${CF_LIBRARIES} ${CS_LIBRARIES})
  endif()

  target_link_libraries( klustersshared ${QT_LIBRARIES})
endif()

install( TARGETS klustersshared EXPORT LibKlustersSharedLibraryTargets ARCHIVE 
    RUNTIME DESTINATION ${BIN_INSTALL_DIR} 
    LIBRARY DESTINATION ${LIB_INSTALL_DIR} 
    ARCHIVE DESTINATION ${LIB_INSTALL_DIR} COMPONENT Devel 
)

install( FILES ${libklustersshared_HEADERS} DESTINATION ${INCLUDE_INSTALL_DIR} COMPONENT Devel )
install( DIRECTORY images DESTINATION ${EXEC_INSTALL_PREFIX}/share/doc/ )

