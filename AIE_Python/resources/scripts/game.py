# This is the main entry point for the Python Scripted Game that we are making using 
# the AIE Static framework
# This functionality gets called from our Main.cpp however this could be modified to be called from 
# elsewhere within out C++ application

#Python uses import instead of #include I is for all intents and purposes the same
import AIE
import Level_Grid
import Entity

#Some global variables that are going to be used in this classless basefile, please note there are OO ways to pass this
#information around, which would be overall more desirable
screenProperties	= { 'width':1280, 'height':720, 'fullscreen':False, 'title':"Python Game Example"  }
MouseButtons 		= { 'button_one':0, 'button_two':1, 'button_three':2 }
_level 				= None #none is the equivalent of nullptr in C++0x11 (or NULL in regular C++)
_entity 			= None

#PyInitialise 
#    - This function is called to initialise our Python Game and returns the Screen properties for our C++ application
#      to set up the appropriate screen resolution title and weather or not to go fullscreen
def PyInitialise():
	# Another option for the intialisation is to return a dictionary object however this would require 
	# a different method of parsing than PyArg_ParseTuple as the return type is a Dictionary object
	#return { 'width':1024, 'height':720, 'fullscreen':False, 'title':"Python Game Example" }
	return ( screenProperties['width'], screenProperties['height'], screenProperties['fullscreen'], screenProperties['title'] )

#PyLoad 
#   - This function is called after Py Initialise, once in here our C++ window should have been set up and OpenGL will have
#     been initialised allowing us to call any functions in our framework dealing with sprite creation etc
def PyLoad():
	#Initialise the background level grid, the global keyword is used to ensure that we are affecting the global variable and not
	#just creating a new local variable with the same name as the global one we intended to use. Python is fun like that
	global _level
	_level = Level_Grid.LevelGrid( screenProperties, {'width':64, 'height':64 } )
	_level.loadSprites()
	#Here we set up a Tank Entity, this is just a simple NPC Entity that will move to where we click the second mouse button within our window
	global _entity
	_entity = Entity.TankEntity()
	
	return 0
	
#PyUpdate
#    - This is the update funciton that will get called each frame to update our Python implementations that need to be updated
#      This function works very much like a standard main loop, except it is called from C++
def PyUpdate( fDeltaTime ):
	global _level
	global _entity
	#Update the Entities and the Level
	_level.update( fDeltaTime )
	_entity.update(fDeltaTime)
	#Draw all our entities and Level
	_level.draw()
	_entity.draw()
	
	return 0
	
#PyShutdown
#    - This function is called when we want to leave our Python project, it will tell any entities that rely on the AIE framework or 
#      Have been created within that framework that they need to be destroyed and their C++ side memory set to free
def PyShutdown():
	#We're shutting down now
	global _level
	global _entity
	
	_level.cleanUp()
	_entity.cleanUp()
	
	return 0

#End of File