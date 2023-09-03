# Canny_Edge_Detection - MPI Implementation

# ------- Environment Setup --------
1. Download Microsoft MPI from https://www.microsoft.com/en-US/download/details.aspx?id=57467, install both files
    Install msmpisetup.exe for MPI executable file
    Install msmpisdk.msi for MPI library
2. Configure project properties → C/C++ → Additional Include Directories (The path iswhere mpi.h is stored, normally is installed at “C:\Program Files (x86)\Microsoft SDKs\MPI\Include”)
3. Configure project properties Linker → Additional library Directories (The path is where msmpi.lib is stored, normally is installed at “C:\Program Files (x86)\Microsoft SDKs\MPI\Lib\x86”)
4. Configure project properties Linker → Input → Additional Dependencies, add in "msmpi.lib";
