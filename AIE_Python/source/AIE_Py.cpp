#include "AIE_Py.h"
#include "AIE.h"
#include <iostream>


PyMethodDef AIE_Functions[] = 
{
	{"Update",					AIE_Update,						METH_VARARGS,		"Update the AIE Framework."								},
	{"ClearScreen",				AIE_ClearScreen,				METH_VARARGS,		"Clear the OpenGL Render scene."						},
	{"SetBackgroundColour",		AIE_SetBackgroundColour,		METH_VARARGS,		"Sets the background Colour of the Scene"				},
	{"CreateSprite",			AIE_CreateSprite,				METH_VARARGS,		"Create a sprite object"								},
	{"DestroySprite",			AIE_DestroySprite,				METH_VARARGS,		"Destroy a sprite object"								},
	{"MoveSprite",				AIE_MoveSprite,					METH_VARARGS,		"Move Sprite"											},
	{"DrawSprite",				AIE_DrawSprite,					METH_VARARGS,		"Draw Sprite"											},
	{"GetMouseLocation",		AIE_GetMouseLocation,			METH_VARARGS,		"Where is the Mouse?"									},
	{"GetMouseButton",			AIE_GetMouseButtonDown,			METH_VARARGS,		"Mouse Button Pressed?"									},
	{"GetMouseButtonRelease",	AIE_GetMouseButtonReleased,		METH_VARARGS,		"Mouse Button Let Go?"									},
	{NULL, NULL, 0, NULL}
};

void  ParsePyTupleError(const char* a_pFunction, int a_Line )
{
	std::cout << "___Error Parsing Tuple___\nFunction: " <<  a_pFunction << "\nLine#   : " << a_Line << std::endl;
	PyErr_Print();
}

PyObject* ImportPythonModule( const char* a_pyModuleName )
{
	PyObject* pObjName= PyString_FromString(a_pyModuleName);
	PyObject* pModule = PyImport_Import(pObjName);
	if (!pModule)
    {
        PyErr_Print();
        std::cout << stderr << "Failed to load \" " << a_pyModuleName << "\"" << std::endl;
    }
    Py_DECREF(pObjName);
	return pModule;
}

PyObject* GetHandleToPythonFunction( PyObject* a_pModule, const char* a_pFunctionName )
{
	 PyObject* pFunction = PyObject_GetAttrString(a_pModule, a_pFunctionName);
    /* pFunc is a new reference */
    if( !(pFunction && PyCallable_Check(pFunction)) ) 
	{
        if (PyErr_Occurred())
        {
			PyErr_Print();
		}
        std::cout << stderr << "Cannot find function \"" << a_pFunctionName << "\"" << std::endl;
    }
	return pFunction;
}

PyObject* CallPythonFunction( PyObject* a_pyFunction, PyObject* a_pyFuncArguments)
{
	PyObject* pReturnValue = PyObject_CallObject( a_pyFunction, a_pyFuncArguments );
    if (pReturnValue == nullptr)
	{
		PyErr_Print();
		fprintf(stderr,"Call failed\n");
    }
	return pReturnValue;
}
//////////////////////////////////////////////////////////////////////////
// This function can be called from Python to force an update to the framework. 
//////////////////////////////////////////////////////////////////////////
PyObject* AIE_Update(PyObject *self, PyObject *args)
{

	FrameworkUpdate();
	Py_RETURN_NONE;
}
//////////////////////////////////////////////////////////////////////////
// Call this function to clear any content that has been rendered to the screen 
//////////////////////////////////////////////////////////////////////////
PyObject* AIE_ClearScreen(PyObject *self, PyObject *args)
{
	ClearScreen();
	Py_RETURN_NONE;
}
//////////////////////////////////////////////////////////////////////////
// Call this function to Set the background colour
//////////////////////////////////////////////////////////////////////////
PyObject* AIE_SetBackgroundColour(PyObject *self, PyObject *args)
{
	int iRed; int iBlue; int iGreen; int iAlpha;
	if (!PyArg_ParseTuple(args, "iiii", &iRed, &iBlue, &iGreen, &iAlpha)) 
	{
		ParsePyTupleError( __func__, __LINE__ );
		return nullptr;
	}
	SetBackgroundColour(SColour( iRed, iBlue, iGreen, iAlpha ));
	Py_RETURN_NONE;
}
//////////////////////////////////////////////////////////////////////////
// Call this function to set the current render colour
// this will cause any following calls to a draw function to have this colour
// applied to those objects which are drawn
//////////////////////////////////////////////////////////////////////////
PyObject* AIE_SetRenderColour(PyObject *self, PyObject *args)
{
	Py_RETURN_NONE;
}
//////////////////////////////////////////////////////////////////////////
// Basic Sprite creation and manipulation functionality
//////////////////////////////////////////////////////////////////////////
PyObject* AIE_CreateSprite(PyObject *self, PyObject *args)
{
	const char* pTextureName; float fv2Size[2]; float fv2Origin[2]; float fv4UVCoords[4]; unsigned int vColour[4];
	if (!PyArg_ParseTuple(args, "sffffffffiiii", &pTextureName, 
												 &fv2Size[0], &fv2Size[1], 
												 &fv2Origin[0], &fv2Origin[1], 
												 &fv4UVCoords[0], &fv4UVCoords[1], &fv4UVCoords[2], &fv4UVCoords[3],
												 &vColour[0], &vColour[1], &vColour[2], &vColour[3]) ) 
	{
		ParsePyTupleError( __func__, __LINE__ );
		return nullptr;
	}
	//fv4UVCoords[0] = 0.f, fv4UVCoords[1] = 0.f, fv4UVCoords[2] = 1.f, fv4UVCoords[3] = 1.f;
	unsigned int uiSpriteID = CreateSprite( pTextureName, fv2Size, fv2Origin, fv4UVCoords, SColour(vColour[0], vColour[1], vColour[2], vColour[3]));
	//unsigned int uiSpriteID = CreateSprite( pTextureName, fv2Size[0], fv2Size[1], false );
	return Py_BuildValue("i", uiSpriteID);
}

PyObject* AIE_DuplicateSprite(PyObject *self, PyObject *args)
{
	Py_RETURN_NONE;
}

PyObject* AIE_SetSpriteColour(PyObject *self, PyObject *args)
{
	Py_RETURN_NONE;
}

PyObject* AIE_GetSpriteColour(PyObject *self, PyObject *args)
{
	Py_RETURN_NONE;
}

PyObject* AIE_MoveSprite(PyObject *self, PyObject *args)
{
	unsigned int iSpriteID; float v2fPos[2];
	if (!PyArg_ParseTuple(args, "iff", &iSpriteID, 
												 &v2fPos[0], &v2fPos[1]) ) 
	{
		ParsePyTupleError( __func__, __LINE__ );
		return nullptr;
	}
	MoveSprite( iSpriteID, v2fPos );
	Py_RETURN_NONE;
}

PyObject* AIE_SetSpriteMatrix(PyObject *self, PyObject *args)
{
	Py_RETURN_NONE;
}

PyObject* AIE_RotateSprite(PyObject *self, PyObject *args)
{
	Py_RETURN_NONE;
}

PyObject* AIE_DestroySprite(PyObject *self, PyObject *args)
{
	unsigned int iSpriteID;
	if (!PyArg_ParseTuple( args, "i", &iSpriteID ) ) 
	{
		ParsePyTupleError( __func__, __LINE__ );
		return nullptr;
	}
	DestroySprite( iSpriteID );
	Py_RETURN_NONE;
}

PyObject* AIE_DrawSprite(PyObject *self, PyObject *args)
{
	unsigned int iSpriteID;
	if (!PyArg_ParseTuple( args, "i", &iSpriteID ) ) 
	{
		ParsePyTupleError( __func__, __LINE__ );
		return nullptr;
	}
	DrawSprite( iSpriteID );
	Py_RETURN_NONE;
}

PyObject* AIE_SetSpriteUVCoordinates(PyObject *self, PyObject *args)
{
	Py_RETURN_NONE;
}

PyObject* AIE_GetSpriteUVCoordinates(PyObject *self, PyObject *args)
{
	Py_RETURN_NONE;
}

PyObject* AIE_SetSpriteScale(PyObject *self, PyObject *args)
{
	Py_RETURN_NONE;
}

PyObject* AIE_GetSpriteScale(PyObject *self, PyObject *args)
{
	Py_RETURN_NONE;
}


//////////////////////////////////////////////////////////////////////////
// Basic Line Drawing Functionality
//////////////////////////////////////////////////////////////////////////
PyObject* AIE_DrawLine(PyObject *self, PyObject *args)
{
	Py_RETURN_NONE;
}
//////////////////////////////////////////////////////////////////////////
// Draws a String to the screen
//////////////////////////////////////////////////////////////////////////
PyObject* AIE_DrawString(PyObject *self, PyObject *args)
{
	Py_RETURN_NONE;
}

//////////////////////////////////////////////////////////////////////////
// Input Handling Functionality
//////////////////////////////////////////////////////////////////////////
PyObject* AIE_IsKeyDown(PyObject *self, PyObject *args)
{
	Py_RETURN_NONE;
}
PyObject* AIE_GetMouseLocation(PyObject *self, PyObject *args)
{
	int iMouseX, iMouseY;
	GetMouseLocation( iMouseX, iMouseY );
	return Py_BuildValue("ii", iMouseX, iMouseY );
}

PyObject* AIE_GetMouseButtonDown(PyObject *self, PyObject *args)
{
	unsigned int iMouseButton;
	if (!PyArg_ParseTuple( args, "i", &iMouseButton ) ) 
	{
		ParsePyTupleError( __func__, __LINE__ );
		return nullptr;
	}
	bool bIsDown = GetMouseButtonDown(iMouseButton);
	if ( bIsDown )
		Py_RETURN_TRUE;
	else
		Py_RETURN_FALSE;
}

PyObject* AIE_GetMouseButtonReleased(PyObject *self, PyObject *args)
{
	unsigned int iMouseButton;
	if (!PyArg_ParseTuple( args, "i", &iMouseButton ) ) 
	{
		ParsePyTupleError( __func__, __LINE__ );
		return nullptr;
	}
	bool bIsReleased = GetMouseButtonReleased(iMouseButton);
	if ( bIsReleased )
		Py_RETURN_TRUE;
	else
		Py_RETURN_FALSE;
}
//////////////////////////////////////////////////////////////////////////
// Basic Camera Control
//////////////////////////////////////////////////////////////////////////
PyObject* AIE_MoveCamera(PyObject *self, PyObject *args)
{
	Py_RETURN_NONE;
}
PyObject* AIE_RotateCamera(PyObject *self, PyObject *args)
{
	Py_RETURN_NONE;
}
PyObject* AIE_GetCameraPosition(PyObject *self, PyObject *args)
{
	Py_RETURN_NONE;
}
//////////////////////////////////////////////////////////////////////////
// Gets the Delta Time
//////////////////////////////////////////////////////////////////////////
PyObject* AIE_GetDeltaTime(PyObject *self, PyObject *args)
{
	Py_RETURN_NONE;
}