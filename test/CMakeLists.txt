set( test_scrollview_SRCS main.cpp  )

add_executable( test_scrollview ${test_scrollview_SRCS} )

target_link_libraries( test_scrollview ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY})
