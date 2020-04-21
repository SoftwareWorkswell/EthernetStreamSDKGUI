# EthernetStreamSDKGUI 0.2.0
EthernetStreamSDKGUI was developed to demonstrate possible usage of EthernetStreamSDK for each of the WIRIS series cameras and MEDICAS thermal camera. Repository is diveded into two parts: ethGui-WIRIS - stream from WIRIS Pro and WIRIS Security cameras and ethGui-MEDICAS.
Project is fully open source, therefore you can access all source files, which can serve as
an example for developing your own application. Note that the most important part of this project is ”ControllerCore” class (with its dependencies like Thread classes and NetworkClient class), which demonstrates basic usage of SDK. User interface was creater using Qt framework and Qt Quick version 5.14.1. Open .pro file in Qt Creator to build or modify provided code. Set the required dependencies in the .pro file, mainly boost headers and opencv headers and library files.

Version: 0.3.0
