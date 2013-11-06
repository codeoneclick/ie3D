#############################################################################
# Makefile for building: indieEngineShader.app/Contents/MacOS/indieEngineShader
# Generated by qmake (3.0) (Qt 5.1.1) on: Wed Nov 6 15:47:57 2013
# Project:  indieEngineShader.pro
# Template: app
# Command: /Applications/Qt/5.1.1/clang_64/bin/qmake -spec macx-xcode -o indieEngineShader.xcodeproj/project.pbxproj indieEngineShader.pro
#############################################################################

MAKEFILE      = project.pbxproj

MOC       = /Applications/Qt/5.1.1/clang_64/bin/moc
UIC       = /Applications/Qt/5.1.1/clang_64/bin/uic
LEX       = flex
LEXFLAGS  = 
YACC      = yacc
YACCFLAGS = -d
DEFINES       = -DQT_NO_DEBUG -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_CORE_LIB
INCPATH       = -I/Applications/Qt/5.1.1/clang_64/mkspecs/macx-xcode -I. -I/Applications/Qt/5.1.1/clang_64/include -I/Applications/Qt/5.1.1/clang_64/include/QtWidgets -I/Applications/Qt/5.1.1/clang_64/lib/QtWidgets.framework/Versions/5/Headers -I/Applications/Qt/5.1.1/clang_64/include/QtGui -I/Applications/Qt/5.1.1/clang_64/lib/QtGui.framework/Versions/5/Headers -I/Applications/Qt/5.1.1/clang_64/include/QtCore -I/Applications/Qt/5.1.1/clang_64/lib/QtCore.framework/Versions/5/Headers -I. -I/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.8.sdk/System/Library/Frameworks/OpenGL.framework/Versions/A/Headers -I/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.8.sdk/System/Library/Frameworks/AGL.framework/Headers -I. -F/Applications/Qt/5.1.1/clang_64/lib
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

compilers: ./moc_CMainWindowGUI.cpp ./moc_CShaderCompileGUI.cpp ./moc_CMaterialSettingsGUI.cpp\
	 ./moc_CMaterialExportGUI.cpp ./ui_CMainWindowGUI.h ./ui_CShaderCompileGUI.h ./ui_CMaterialSettingsGUI.h\
	 ./ui_CMaterialExportGUI.h
compiler_objective_c_make_all:
compiler_objective_c_clean:
compiler_rcc_make_all:
compiler_rcc_clean:
compiler_moc_header_make_all: moc_CMainWindowGUI.cpp moc_CShaderCompileGUI.cpp moc_CMaterialSettingsGUI.cpp moc_CMaterialExportGUI.cpp
compiler_moc_header_clean:
	-$(DEL_FILE) moc_CMainWindowGUI.cpp moc_CShaderCompileGUI.cpp moc_CMaterialSettingsGUI.cpp moc_CMaterialExportGUI.cpp
moc_CMainWindowGUI.cpp: /Applications/Qt/5.1.1/clang_64/include/QtWidgets/QMainWindow \
		CShaderCompileGUI.h \
		/Applications/Qt/5.1.1/clang_64/include/QtWidgets/QDialog \
		CMaterialSettingsGUI.h \
		CMaterialExportGUI.h \
		CMainWindowGUI.h
	/Applications/Qt/5.1.1/clang_64/bin/moc $(DEFINES) -D__APPLE__ -D__GNUC__=4 $(INCPATH) CMainWindowGUI.h -o moc_CMainWindowGUI.cpp

moc_CShaderCompileGUI.cpp: /Applications/Qt/5.1.1/clang_64/include/QtWidgets/QDialog \
		CShaderCompileGUI.h
	/Applications/Qt/5.1.1/clang_64/bin/moc $(DEFINES) -D__APPLE__ -D__GNUC__=4 $(INCPATH) CShaderCompileGUI.h -o moc_CShaderCompileGUI.cpp

moc_CMaterialSettingsGUI.cpp: /Applications/Qt/5.1.1/clang_64/include/QtWidgets/QDialog \
		CMaterialSettingsGUI.h
	/Applications/Qt/5.1.1/clang_64/bin/moc $(DEFINES) -D__APPLE__ -D__GNUC__=4 $(INCPATH) CMaterialSettingsGUI.h -o moc_CMaterialSettingsGUI.cpp

moc_CMaterialExportGUI.cpp: /Applications/Qt/5.1.1/clang_64/include/QtWidgets/QDialog \
		CMaterialExportGUI.h
	/Applications/Qt/5.1.1/clang_64/bin/moc $(DEFINES) -D__APPLE__ -D__GNUC__=4 $(INCPATH) CMaterialExportGUI.h -o moc_CMaterialExportGUI.cpp

compiler_moc_source_make_all:
compiler_moc_source_clean:
compiler_uic_make_all: ui_CMainWindowGUI.h ui_CShaderCompileGUI.h ui_CMaterialSettingsGUI.h ui_CMaterialExportGUI.h
compiler_uic_clean:
	-$(DEL_FILE) ui_CMainWindowGUI.h ui_CShaderCompileGUI.h ui_CMaterialSettingsGUI.h ui_CMaterialExportGUI.h
ui_CMainWindowGUI.h: CMainWindowGUI.ui
	/Applications/Qt/5.1.1/clang_64/bin/uic CMainWindowGUI.ui -o ui_CMainWindowGUI.h

ui_CShaderCompileGUI.h: CShaderCompileGUI.ui
	/Applications/Qt/5.1.1/clang_64/bin/uic CShaderCompileGUI.ui -o ui_CShaderCompileGUI.h

ui_CMaterialSettingsGUI.h: CMaterialSettingsGUI.ui
	/Applications/Qt/5.1.1/clang_64/bin/uic CMaterialSettingsGUI.ui -o ui_CMaterialSettingsGUI.h

ui_CMaterialExportGUI.h: CMaterialExportGUI.ui
	/Applications/Qt/5.1.1/clang_64/bin/uic CMaterialExportGUI.ui -o ui_CMaterialExportGUI.h

compiler_rez_source_make_all:
compiler_rez_source_clean:
compiler_yacc_decl_make_all:
compiler_yacc_decl_clean:
compiler_yacc_impl_make_all:
compiler_yacc_impl_clean:
compiler_lex_make_all:
compiler_lex_clean:
compiler_clean: compiler_moc_header_clean compiler_uic_clean 

