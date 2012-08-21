
SOURCES += src/main.cpp \
    src/error_recovery.cpp \
    src/context_menu.cpp \
    src/xml_saver.cpp


include(dialogs/dialogs.pri)
include(graphics/graphics.pri)

OTHER_FILES += \
    src/README \
    src/generated/README
