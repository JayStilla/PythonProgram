
@echo off

goto :CreatePythonEnvironmentVariables
==========================================================================================
CreatePythonEnvironmentVariables
  Usage:
    This script sets up the environment variables for Python 2.6.x 

  Notes:
   setx -m the -m flag sets environment variables for all users

==========================================================================================
:CreatePythonEnvironmentVariables
setx PYTHONHOME "C:\PYTHON27"
setx -m PYTHONPATH "C:\PYTHON27;C:\PYTHON27\INCLUDE;C:\PYTHON27\DLLS;C:\PYTHON27\LIB;C:\PYTHON27\LIB\LIB-TK"
setx -m PYTHONLIBS "C:\PYTHON27\LIBS;"
pause
@echo off