#include <Python.h>
#include "AIE_Py.h"
#include "AIE.h"
#include <iostream>

char* g_pWindowTitle;

namespace AIE
{
	bool InitialiseFramework( PyObject* a_pModule );
	void Load				( PyObject* a_pModule );
	void UpdatePython		( PyObject* a_pModule, float a_fDeltaTime );
	void ShutdownFramework	( PyObject* a_pModule );
}

int main(int argc, char *argv[])
{
	
    
	//\================================================================================================
	//\ Initialise Python
	//\================================================================================================
	Py_Initialize();
	//\================================================================================================
	//\Add our current directory to the path lookup for Python imports
	//\================================================================================================
	PySys_SetArgv(argc, argv);
	//PyRun_SimpleString( "from sys import path\nfrom os import getcwd\nprint \"Current Dir\" \, getcwd()\npath.append( getcwd() + \"/scripts\" )\n" );
	PyObject* sysPath = PySys_GetObject((char*)"path");
	PyList_Append(sysPath, PyString_FromString("./scripts"));
	//\================================================================================================
	//\Import the AIE C Functions into Python so that we can call them from there
	//\ we will need to add "import AIE" to any Python files that we wish to use these functions in
	//\================================================================================================
	Py_InitModule("AIE", AIE_Functions);
	//\================================================================================================
	//\ Here we are loading our Python Entry point this is the name of the game.py file that we will be 
	//\ using for this project.
	//\ ** feel free to change this to anything you would like to, "game" is purely intended as a 
	//\    suitable example.
	//\================================================================================================
	PyObject* pModule = ImportPythonModule( "game" );
	
	if (pModule != NULL) 
	{
		//\============================================================================================
		//\ Here we are attempting to resolve a function to call inside our python file
		//\ In this scenario we are looking for the main function inside our game.py file
		//\============================================================================================
		if( AIE::InitialiseFramework( pModule ) )
		{
			AIE::Load(pModule);
			do 
			{

				ClearScreen();
				float fDeltaTime = GetDeltaTime();
				AIE::UpdatePython( pModule, fDeltaTime );

			}while( !FrameworkUpdate() );

			AIE::ShutdownFramework( pModule );
		}
		Py_DECREF(pModule);
    }
    Py_Finalize();

	if( g_pWindowTitle )
	{
		delete[] g_pWindowTitle;
	}

    return 0;
}

namespace AIE
{
	bool InitialiseFramework( PyObject* a_pModule )
	{
		bool bSuccess = false;
		PyObject* pInitialise = GetHandleToPythonFunction( a_pModule, "PyInitialise" );
		if( pInitialise )
		{
			PyObject* pReturnValue = CallPythonFunction( pInitialise, nullptr );
			if( pReturnValue )
			{
				int iWidth; int iHeight; bool bFullscreen; char* windowTitle;

				if (!PyArg_ParseTuple(pReturnValue, "iibs", &iWidth, &iHeight, &bFullscreen, &windowTitle)) 
				{
					ParsePyTupleError( __func__, __LINE__ );
				}
				Py_DECREF(pReturnValue);

				int titleLength = strlen(windowTitle);
				g_pWindowTitle = new char[titleLength+1];
				memcpy(g_pWindowTitle, windowTitle, titleLength);
				g_pWindowTitle[titleLength] = '\0';
				bSuccess = ( Initialise( iWidth, iHeight, bFullscreen, g_pWindowTitle) == 0 );
			}
			Py_XDECREF(pInitialise);
			
		}
		return bSuccess;
	}

	void Load( PyObject* a_pModule )
	{
		//"./images/crate_sideup.png", 64.0, 64.0, 0.5, 0.5, 0.0, 0.0, 1.0, 1.0, 255, 255, 255, 255
		float fv2Size[2] = { 64.f, 64.f };
		float fv2Origin[2] = { 0.5f, 0.5f };
		float fv4UVCoords[4] = { 0.f, 0.f, 1.f, 1.f };
		float vColour[4] = { 1.f, 1.f, 1.f, 1.f };

		unsigned int uiSpriteID = CreateSprite( "./images/crate_sideup.png", fv2Size, fv2Origin, fv4UVCoords, SColour(vColour[0]/255, vColour[1]/255, vColour[2]/255, vColour[3]/255) );
	
		PyObject* pLoad = GetHandleToPythonFunction( a_pModule, "PyLoad" );
		if( pLoad )
		{
			PyObject* pReturnValue = CallPythonFunction( pLoad, nullptr );
			if( pReturnValue )
			{
				Py_DECREF(pReturnValue);
			}
			Py_XDECREF(pLoad);
			
		}
	}

	void UpdatePython( PyObject* a_pModule, float a_fDeltaTime )
	{
		PyObject* pUpdateFunc = GetHandleToPythonFunction( a_pModule, "PyUpdate" );
		if (pUpdateFunc) 
		{
			PyObject* pDeltaTime = PyFloat_FromDouble( a_fDeltaTime );
			PyObject* pArgs = PyTuple_New(1);
			PyTuple_SetItem( pArgs, 0, pDeltaTime );
			PyObject* pReturnValue = CallPythonFunction( pUpdateFunc, pArgs );
			if( pReturnValue )
			{
				//std::cout << "Updating Python Game Loop" << std::endl;
				Py_DECREF(pReturnValue);
			}
			Py_XDECREF(pUpdateFunc);
		}  
	}

	void ShutdownFramework( PyObject* a_pModule )
	{
		PyObject* pShutdownFunc = GetHandleToPythonFunction( a_pModule, "PyShutdown" );
		if (pShutdownFunc) 
		{
			PyObject* pReturnValue = CallPythonFunction( pShutdownFunc, nullptr );
			if( pReturnValue )
			{
				printf("We have reached the end of the Game! Arguments returned from call: %ld\n", PyInt_AsLong(pReturnValue));
				Py_DECREF(pReturnValue);
			}
			Py_XDECREF(pShutdownFunc);
		}
		Shutdown();
	}
}