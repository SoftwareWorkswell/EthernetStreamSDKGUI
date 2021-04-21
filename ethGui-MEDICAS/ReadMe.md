## Requirements
- Windows 7 or newer

- qmake build system

- minimum Qt version is 5.14

  

## Dependencies

All dependencies for Windows can be downloaded from dropbox https://www.dropbox.com/home/Software/Customer-projects/Active/Libraries/MedicasEthernetStreamSDK

- boost 1.70.0 - can be downloaded at: https://www.boost.org/users/history/version_1_70_0.html
- gstreamer - can be downloaded at https://gstreamer.freedesktop.org/
- opencv 4.1.1 - (has to be compiled with a gstreamer support) can be downloaded at 

#### How to compile opencv with gstreamer:
- follow steps of this tutorial: https://funvision.blogspot.com/2020/03/compile-opencv-4-for-visual-studio-2019.html

  

## Quick project setup
- Clone git repository (branch medicas-version)	`git clone https://workswell-sro.codebasehq.com/sdk-libraries-and-plugins/sdk-medicas-ethernet-stream-gui.git` .

- Download and extract to project folder "img.zip", "libs.zip" and "translations.zip" from dropbox: https://www.dropbox.com/home/Software/Customer-projects/Active/Libraries/MedicasEthernetStreamSDK.  Alternatively You can use your own libraries, but compatibility is not granted.

- It's recommended to use Qt 5.14.2 MSVC2017 64bit compiler to build project

    
## Releasing the application
- for deployment of QT libraries use windeployqt, make sure you use a tool for your version of Qt
- third party runtime libraries need to be copied manually



## Creating application setup

- Download and extract "setup.zip" from Software dropbox: https://www.dropbox.com/home/Software/Customer-projects/Active/Libraries/MedicasEthernetStreamSDK
-  Build project in release
- Copy .exe file from build/release to setup/packages/app/data. If You are using only provided libraries (boost, opencv, gstreamer), there is no need to deploy app or copy third pary libraries as everything is allready in setup template.
-  Run Command Prompt or Windows PowerShell and cd to setup directory
- To create setup run binary creator e.g `binarycreator.exe -c ./config/config.xml -p ./packages setup.exe`