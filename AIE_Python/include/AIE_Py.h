//\==========================================================================================================
//\ This is a wrapper object for linking the AIE Static Framework to Python
//\ This is not the most graceful of implementations, this is just to give you an idea of how 
//\ Python can be linked into an existing project.
//\ This project is designed to be the base off of with the AI Assessment is built from should you wish to 
//\ modify any of the python linkage or any other aspect of this code then by all means proceed.
//\
//\ Created Shamefully by :Jamie Stewart
//\ Date:					20/06/2013
//\
//\==========================================================================================================

#ifndef __AIE_PY_H__
#define __AIE_PY_H__

#include <Python.h>

#if defined ( _WIN32 )
#define __func__ __FUNCTION__
#endif

	//\===============================================================================
	//\ This is the Python Function look up table, any C function we want to call from 
	//\ Python will go in this table/array
	//\===============================================================================
	extern PyMethodDef AIE_Functions[];
	//\===============================================================================
	//\ - Some Python Linkage Boilerplate code to wrap up calling functions and modules
	//\===============================================================================
	void  ParsePyTupleError( const char* a_pFunction, int a_Line );
	PyObject* ImportPythonModule( const char* a_pyModuleName );
	PyObject* GetHandleToPythonFunction( PyObject* a_pModule, const char* a_pFunctionName );
	PyObject* CallPythonFunction( PyObject* a_pyFunction, PyObject* a_pyFuncArguments);
	//\===============================================================================

	//////////////////////////////////////////////////////////////////////////
	//// This function can be called from Python to force an update to the framework. 
	//////////////////////////////////////////////////////////////////////////
	static PyObject* AIE_Update					(PyObject *self, PyObject *args);
	//////////////////////////////////////////////////////////////////////////
	// Call this function to clear any content that has been rendered to the screen 
	//////////////////////////////////////////////////////////////////////////
	static PyObject* AIE_ClearScreen			(PyObject *self, PyObject *args);
	//////////////////////////////////////////////////////////////////////////
	// Call this function to Set the background colour
	//////////////////////////////////////////////////////////////////////////
	static PyObject* AIE_SetBackgroundColour	(PyObject *self, PyObject *args);
	//////////////////////////////////////////////////////////////////////////
	// Call this function to set the current render colour
	// this will cause any following calls to a draw function to have this colour
	// applied to those objects which are drawn
	//////////////////////////////////////////////////////////////////////////
	static PyObject* AIE_SetRenderColour		(PyObject *self, PyObject *args);
	//////////////////////////////////////////////////////////////////////////
	// Basic Sprite creation and manipulation functionality
	//////////////////////////////////////////////////////////////////////////
	static PyObject* AIE_CreateSprite			(PyObject *self, PyObject *args);
	static PyObject* AIE_DuplicateSprite		(PyObject *self, PyObject *args);
	static PyObject* AIE_SetSpriteColour		(PyObject *self, PyObject *args);
	static PyObject* AIE_GetSpriteColour		(PyObject *self, PyObject *args);
	static PyObject* AIE_MoveSprite				(PyObject *self, PyObject *args);
	static PyObject* AIE_SetSpriteMatrix		(PyObject *self, PyObject *args);
	static PyObject* AIE_RotateSprite			(PyObject *self, PyObject *args);
	static PyObject* AIE_DestroySprite			(PyObject *self, PyObject *args);
	static PyObject* AIE_DrawSprite				(PyObject *self, PyObject *args);
	static PyObject* AIE_SetSpriteUVCoordinates	(PyObject *self, PyObject *args);
	static PyObject* AIE_GetSpriteUVCoordinates	(PyObject *self, PyObject *args);
	static PyObject* AIE_SetSpriteScale			(PyObject *self, PyObject *args);
	static PyObject* AIE_GetSpriteScale			(PyObject *self, PyObject *args);
	//////////////////////////////////////////////////////////////////////////
	// Basic Line Drawing Functionality
	//////////////////////////////////////////////////////////////////////////
	static PyObject* AIE_DrawLine				(PyObject *self, PyObject *args);
	//////////////////////////////////////////////////////////////////////////
	// Draws a String to the screen
	//////////////////////////////////////////////////////////////////////////
	static PyObject* AIE_DrawString				(PyObject *self, PyObject *args);
	//////////////////////////////////////////////////////////////////////////
	// Input Handling Functionality
	//////////////////////////////////////////////////////////////////////////
	static PyObject* AIE_IsKeyDown				(PyObject *self, PyObject *args);
	static PyObject* AIE_GetMouseLocation		(PyObject *self, PyObject *args);
	static PyObject* AIE_GetMouseButtonDown		(PyObject *self, PyObject *args);
	static PyObject* AIE_GetMouseButtonReleased	(PyObject *self, PyObject *args);
	//////////////////////////////////////////////////////////////////////////
	// Basic Camera Control
	//////////////////////////////////////////////////////////////////////////
	static PyObject* AIE_MoveCamera				(PyObject *self, PyObject *args);
	static PyObject* AIE_RotateCamera			(PyObject *self, PyObject *args);
	static PyObject* AIE_GetCameraPosition		(PyObject *self, PyObject *args);
	//////////////////////////////////////////////////////////////////////////
	// Gets the Delta Time
	//////////////////////////////////////////////////////////////////////////
	static PyObject* AIE_GetDeltaTime			(PyObject *self, PyObject *args);

#endif //__AIE_PY_H__