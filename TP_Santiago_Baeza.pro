#algo re piola es que aca en el .pro para agregar comentarios se usa el numeral y no //, igual que en python


#Módulos de Qt que se van a usar en el proyecto
QT       += core gui #core: funcionalidades básicas | gui: interfaz gráfica
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets #widgets: botones, barras, etc. (solo si Qt > 4)


#Configuración del compilador
CONFIG += c++17 # Usa el estándar moderno de C++ (C++17)
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


#Archivos fuente (.cpp) que forman parte del proyecto
SOURCES += \
    main.cpp \       #Punto de entrada del programa
    mainwindow.cpp   #Lógica de la ventana principal


#Archivos de cabecera (.h) que definen clases y funciones
HEADERS += \
    mainwindow.h     #Declaración de la clase MainWindow


#Archivos de interfaz (.ui) creados con Qt Designer
FORMS += \
    mainwindow.ui    #Diseño visual de la ventana principal


#Archivos de traducción (para internacionalización)
TRANSLATIONS += \
    TP_Santiago_Baeza_es_AR.ts  #Traducción al español argentino


#Configuración para compilar las traducciones
CONFIG += lrelease    #Compila los archivos .ts a .qm
CONFIG += embed_translations   #Inserta las traducciones en el ejecutable


#Reglas de instalación (para despliegue en sistemas específicos)
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
