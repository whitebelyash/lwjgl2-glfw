LWJGL 2 on GLFW
=====
This is a minimal port of LWJGL 2 to run on GLFW 3. Current status:
- [x] Window creation and basic window events
- [x] Multiple contexts, shared contexts, context hints
- [x] Mouse and keyboard input, mouse grab, unicode input
- [ ] Custom mouse cursors
- [ ] Controller support
- [ ] Additional runtime window settings
- [ ] (Maybe?) Remove the "64" suffix from LWJGL libname

Tested locally on my computer with 1.12.2, Forge 1.12.2 and 1.2.5

AWT support is not planned, but if you want to try to give it a shot, please send a draft pull request.

Build process: (GLFW must be installed)
```shell
ant jars
pushd platform_build
mkdir build && cd build
cmake ..
make -j$(nproc)
popd
```
You will need to manually copy the compiled liblwjgl(64).so library into the correct location in libs/

Old README:

[LEGACY] LWJGL - Lightweight Java Game Library
======

> **WARNING**
> 
> This is the repository of the original LWJGL, which is no longer actively maintained. Unless you have released a product that uses LWJGL 2.x, you should probably be looking at [LWJGL 3](https://github.com/LWJGL/lwjgl3).

The Lightweight Java Game Library (LWJGL) is a solution aimed directly at professional and amateur Java programmers alike to enable commercial quality games to be written in Java. 
LWJGL provides developers access to high performance crossplatform libraries such as OpenGL (Open Graphics Library), OpenCL (Open Computing Language) and OpenAL (Open Audio Library) allowing for state of the art 3D games and 3D sound.
Additionally LWJGL provides access to controllers such as Gamepads, Steering wheel and Joysticks.
All in a simple and straight forward API.

Website: [http://legacy.lwjgl.org](http://legacy.lwjgl.org)
Forum: [http://forum.lwjgl.org](http://forum.lwjgl.org)
Bugs/Suggestions: [https://github.com/LWJGL/lwjgl/issues](https://github.com/LWJGL/lwjgl/issues)

Compilation
-----------

LWJGL requires a JDK and Ant installed to compile, as well as your platforms native compiler to compile the JNI.

* ant generate-all
* ant compile
* ant compile_native
