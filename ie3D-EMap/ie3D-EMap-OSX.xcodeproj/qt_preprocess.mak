#############################################################################
# Makefile for building: ie3D-EMap.app/Contents/MacOS/ie3D-EMap
# Generated by qmake (3.0) (Qt 5.4.1)
# Project:  ie3D-EMap.pro
# Template: app
# Command: /usr/bin/qmake -spec macx-xcode -o ie3D-EMap-OSX.xcodeproj/project.pbxproj ie3D-EMap.pro
#############################################################################

MAKEFILE      = project.pbxproj

MOC       = /Volumes/media/Applications/QT/5.4/clang_64/bin/moc
UIC       = /Volumes/media/Applications/QT/5.4/clang_64/bin/uic
LEX       = flex
LEXFLAGS  = 
YACC      = yacc
YACCFLAGS = -d
DEFINES       = -DQT_NO_DEBUG -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_CORE_LIB
INCPATH       = -I. -I/Volumes/media/Applications/QT/5.4/clang_64/lib/QtWidgets.framework/Versions/5/Headers -I/Volumes/media/Applications/QT/5.4/clang_64/lib/QtGui.framework/Versions/5/Headers -I/Volumes/media/Applications/QT/5.4/clang_64/lib/QtCore.framework/Versions/5/Headers -I. -I/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.9.sdk/System/Library/Frameworks/OpenGL.framework/Versions/A/Headers -I/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.9.sdk/System/Library/Frameworks/AGL.framework/Headers -I. -I/Volumes/media/Applications/QT/5.4/clang_64/mkspecs/macx-clang -F/Volumes/media/Applications/QT/5.4/clang_64/lib
DEL_FILE  = rm -f
MOVE      = mv -f

IMAGES = 
PARSERS =
preprocess: $(PARSERS) compilers
clean preprocess_clean: parser_clean compiler_clean

parser_clean:
mocclean: compiler_moc_header_clean compiler_moc_source_clean

mocables: compiler_moc_header_make_all compiler_moc_source_make_all

check: first

compilers: moc_CMainWindow.cpp ui_CMainWindow.h
compiler_objective_c_make_all:
compiler_objective_c_clean:
compiler_rcc_make_all:
compiler_rcc_clean:
compiler_moc_header_make_all: moc_CMainWindow.cpp
compiler_moc_header_clean:
	-$(DEL_FILE) moc_CMainWindow.cpp
moc_CMainWindow.cpp: /Volumes/media/Applications/QT/5.4/clang_64/lib/QtWidgets.framework/Versions/5/Headers/QMainWindow \
		Sources/HMEDeclaration.h \
		Sources/CMainWindow.h
	/Volumes/media/Applications/QT/5.4/clang_64/bin/moc $(DEFINES) -D__APPLE__ -D__GNUC__=4 -I/Volumes/media/Applications/QT/5.4/clang_64/mkspecs/macx-clang -I/Users/codeoneclick/Documents/hobby/ie3D/ie3D-EMap -I/Volumes/media/Applications/QT/5.4/clang_64/lib/QtWidgets.framework/Headers -I/Volumes/media/Applications/QT/5.4/clang_64/lib/QtGui.framework/Headers -I/Volumes/media/Applications/QT/5.4/clang_64/lib/QtCore.framework/Headers -F/Volumes/media/Applications/QT/5.4/clang_64/lib Sources/CMainWindow.h -o moc_CMainWindow.cpp

compiler_moc_source_make_all:
compiler_moc_source_clean:
compiler_uic_make_all: ui_CMainWindow.h
compiler_uic_clean:
	-$(DEL_FILE) ui_CMainWindow.h
ui_CMainWindow.h: Forms/CMainWindow.ui
	/Volumes/media/Applications/QT/5.4/clang_64/bin/uic Forms/CMainWindow.ui -o ui_CMainWindow.h

compiler_rez_source_make_all:
compiler_rez_source_clean:
compiler_yacc_decl_make_all:
compiler_yacc_decl_clean:
compiler_yacc_impl_make_all:
compiler_yacc_impl_clean:
compiler_lex_make_all:
compiler_lex_clean:
compiler_clean: compiler_moc_header_clean compiler_uic_clean 
