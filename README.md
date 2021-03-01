# Glass Opus

Glass Opus is an open source rotoscoping software for students and artists. The software will provide a variety of features to allow the users to see their work from start to finish. Drawing with a variety brushes and vectors, image manipuilation and object tracking via an included AI package, and filtering are among the many features than one can employ to create their vision.

The focus of Glass Opus, is to provide a free software that students and artists can use to further their work and portfolio. This is often a difficult endeavor for artists due to the restrictive cost of major softwares. Since Glass Opus is open source, users can tweak features or add their own to suit specific needs. It will also serve as a foundation for those who seek to improve their knowledge in image processing and manipulation, as well as basic graphics programming.

Microsoft coding guidelines are used. Found [here](https://www.cise.ufl.edu/~mschneid/Research/C++%20Programming%20Style%20Guidelines.htm) and implemented via [this](https://docs.microsoft.com/en-us/cpp/code-quality/using-the-cpp-core-guidelines-checkers?view=vs-2019https://docs.microsoft.com/en-us/cpp/code-quality/using-the-cpp-core-guidelines-checkers?view=vs-2019).

The app can be run via the information below in the [running](https://github.com/Geist-of-the-Automaton/GlassOpus/blob/main/README.md#running) section

## Dev Setup

1. Install C++ libraries, specifically <!--either -->[MSVC](https://visualstudio.microsoft.com/vs/features/cplusplus/)<!-- or [MinGW](http://www.mingw.org/)-->.
2. Install [Qt and Qt Creator](https://www.qt.io/download-open-source?hsCtaTracking=9f6a2170-a938-42df-a8e2-a9f0b1d6cdce%7C6cb0de4f-9bb5-4778-ab02-bfb62735f3e5)
3. Install [OpenCV](https://opencv.org/) libraries. [Tutorial](https://wiki.qt.io/How_to_setup_Qt_and_openCV_on_Windows) if needed.
4. Make sure system path aligns with current installation of OpenCV and MSVC. Verify that the includes and libs within the project file are correct.
5. Copy the .libs from the OpenCV build folder to the highest/top directory of the build folder.
6. Copy the mainMenubar.txt into same location as the .libs mentioned above.

## Running

There are two options to running the software:  
1. Once Qt and the OpenCV libs have been correctly installed, one must open the project in Qt and press the run button in the bottom left of the Qt gui. (This option requires the Qt libraries to be on your path.)
2. Open the bin folder and run the Roto.exe file. Found [here](https://github.com/Geist-of-the-Automaton/GlassOpus/releases/tag/v0.1)

## Videos of program running

https://youtu.be/YO4rZiHFvdQ

https://youtu.be/wA5ZYv1swbU

https://youtu.be/p1WU3VXfM3Y

https://youtu.be/9Q7ZjGNGi_Y

https://youtu.be/XUTLOKrGKds

## Images of program running

![Icon_View](https://user-images.githubusercontent.com/44931507/109450387-c3571500-7a18-11eb-9ebe-5ec7456ffaed.png)

![Spline_Example_1](https://user-images.githubusercontent.com/44931507/109450396-cb16b980-7a18-11eb-8ec9-137f5bc2f685.png)

![Spline_Brush_Example_1](https://user-images.githubusercontent.com/44931507/109450419-d669e500-7a18-11eb-9f5e-c339581bfaa5.png)

![Spline_Brush_Example_2](https://user-images.githubusercontent.com/44931507/109450432-dbc72f80-7a18-11eb-84c6-f2a0b3ed0945.png)

![Spline_Brush_Example_3](https://user-images.githubusercontent.com/44931507/109450440-dec22000-7a18-11eb-9de4-7f6569bb464c.png)

![Spline_Example_2](https://user-images.githubusercontent.com/44931507/109450447-e5509780-7a18-11eb-8910-055c4971a558.png)


