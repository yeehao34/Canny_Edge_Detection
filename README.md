# Canny_Edge_Detection

*Note: This setup is required as need to include opencv
# ------- Environment Setup --------
1. Download opencv from https://opencv.org/releases/ by selecting Windows
2. Extract the zip file
3. Open Visual Studio and create a cpp file
4. Setup project properties
5. Choose "All Configurations" as the configuration and "x64" as the Platform
6. Open "C/C++" tab and click "General"
7. Remove the original one and add Additional Include Directories, copy the path of "opencv\build\include" and paste it, click "Ok". 
8. Open "Linker" tab and click "General"
9. Remove the original one and add Additional Library Directories, copy the path of "opencv\build\x64\vc{based on your version downloaded}\lib" and paste it
10. Open "Debugging" tab 
11. Add Environment, type "PATH={your opencv directory}\build\x64\vc{based on your version downloaded}\bin;%PATH%", click "OK"
12. Click on the "Configuration Manager" at the top right corner
13. Make sure the both platform (Active solution platform and Platform) is "x64", then close it
14. Change configuration to "Debug" as the configuration and Open "Linker" tab and click "Input"
15. Remove the original one and add Additional Dependencies, copy and paste the "opencv_world{version}d.lib", verify the filename at "opencv\build\x64\vc{based on your version downloaded}\lib", click "OK" 
16. Choose "Release" as the configuration and Open "Linker" tab and click "Input"
17. same as Step 15 but the filename is without the 'd'
18. Click "Apply" and "OK"
19. Select "Release" as the "Active solution configuration" instead of "Debug"

![2023-08-24 23 55 42](https://github.com/yeehao34/Canny_Edge_Detection/assets/88917687/f828ab6a-8b1f-4267-9101-5bf98efa60c2)

