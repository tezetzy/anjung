# AML-AIDE-Template
Template for making AndroidModLoader mods on AIDE app

# How to use
## You need:
- AIDE(obviously)
- NDK installed
- basic knowledge on what you are doing
- a working brain with atleast 2 braincells
## Setting up
1. Clone this repository by using an app that can clone this repository 
2. Copy this project's folder to /storage/emulated/0/AppProjects (aka the folder where AIDE stores projects)
3. Open this project on AIDE
4. Change `modtemplate` to whatever you want on `jni/Android.mk`
5. Change the libname.so path on `src/MainActivity.java:55` to your .so file
6. Change the target mod folder at line 56 on the same java file
7. Change the modded app's package name on line 100
8. i think you're good to go, code some mods and compile the app

This app will automatically install your mod to the mods folder of your AML modded app

Tested with Xiaomi Redmi 5 Plus(android 8.1) and Oppo a17k(android 11)


