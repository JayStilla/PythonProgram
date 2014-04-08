import AIE
import game
import math
import Collision
import random
from Collision import CircCollision
#Tank Entity
#   A simple entity that can be placed on the screen with a right click, you should modify this so that the tank can be told to 
#   navigate to a point instead of instantly move.

#Making a Vector 2 class to help make AI functions easier
class Vec2:
	def __init__(self, x, y):
		self.x = float(x)
		self.y = float(y)
	def __add__(self, other):
		return Vec2(self.x + other.x, self.y + other.y)
	def scaleBy(self, scalar):
		self.x *= scalar
		self.y *= scalar
		return self
	def getMag(self):
		first = self.x **2
		second = self.y **2
		return math.sqrt(first + second)
	def getNormal(self):
		mag = self.getMag()
		self.x /= mag
		self.y /= mag 
		return self
	def lerp(startVec, endVec, perc):
		newX = startVec.x + (endVec.x - startVec.x) * perc
		newY = startVec.y + (endVec.y - startVec.y) * perc
		return Vec2(newX, newY)
	def __getitem__(self, index):
		if(index is 0):
			return self.x
		else: 
			return self.y




class TankEntity:

	def __init__(self):
		self.Position = Vec2( 1000, 400 )
		self.Rotation = 0
		self.spriteName = "./images/PlayerTanks.png"
		self.size = (57, 72 )
		self.Collider = CircCollision(self, (self.size[1] / 2.0))
		self.origin = (0.5, 0.5)
		self.spriteID = AIE.CreateSprite( self.spriteName, self.size[0], self.size[1], self.origin[0], self.origin[1], 71.0/459.0, 1.0 - 72.0/158.0, 128/459.0, 1.0 , 0xff, 0xff, 0xff, 0xff )
		self.wanderAngle = 0.0
		self.inputBehavior = 0
		print "spriteID", self.spriteID
		#Move Tile to appropriate location
		
		self.turret = Turret(self)
	
	#seek function that when right mouse button is held down
	#the entity moves towards mouse X and Y location
	def seek(self, mouseX, mouseY):
		if(self.Position !=(mouseX, mouseY)):
			lerpX = (self.Position[0] + (mouseX - self.Position[0]) * 0.1)
			lerpY = (self.Position[1] + (mouseY - self.Position[1]) * 0.1)
			lerp = Vec2(lerpX, lerpY)
			return lerp
		else:
			pass
	#does the exact opposite of seek with the left mouse button
	def flee(self, mouseX, mouseY):
		lerpX = (self.Position[0] - (mouseX - self.Position[0]) * 0.1)
		lerpY = (self.Position[1] - (mouseY - self.Position[1]) * 0.1)
		lerp = Vec2(lerpX, lerpY)
		return lerp
		
	#end of custom functions
		
	def update(self, fDeltaTime ):
		self.Velocity = Vec2(5, 5)
		#self.input(fDeltaTime)
		#Update Collider
		self.Collider.update(self)
		
		self.Velocity += self.wander(10.0, 10.0)
		NewPosition = self.Position + self.Velocity
		self.Position = Vec2.lerp(self.Position, NewPosition, 0.12)
		self.keepOnScreen()
		mouseX, mouseY = AIE.GetMouseLocation()
		if( AIE.GetMouseButton(1)  ):
			self.Position = self.seek(mouseX, mouseY)
		elif( AIE.GetMouseButton(0) ):
			self.Position = self.flee(mouseX, mouseY)
		AIE.MoveSprite( self.spriteID, self.Position[0], self.Position[1] ) # write Python side positions to C++ side positions
		self.turret.update(fDeltaTime)
		
	#Building a wander class that will be called to make the entity
	#wander around the map smoothly
	def setAngle(self, vector, number):
		direction = vector

		mag = direction.getMag()

		direction.x = math.cos(number) * mag
		direction.y = math.sin(number) * mag

		return direction
	def wander(self, CIRCLE_DISTANCE, CIRCLE_RADIUS):
		if self.Velocity.x is 0.0 and self.Velocity.y is 0.0:
			return Vec2(0,0)

		wanderAngleDelta = 1.0

		circleCenter = self.Velocity
		circleCenter = circleCenter.getNormal()
		circleCenter.scaleBy(CIRCLE_DISTANCE)

		displacement = Vec2(0, -1)
		displacement.scaleBy(CIRCLE_RADIUS)
		self.setAngle(displacement, self.wanderAngle)
		random.seed(None)
		self.wanderAngle += random.random() * wanderAngleDelta - wanderAngleDelta * 0.8

		wanderForce = circleCenter + displacement
		return wanderForce

	#keep the entity from wandering off the screen
	def keepOnScreen(self):
		screenProperties = game.screenProperties

		if self.Position.x > (float(screenProperties['width'] + 1.0)):
			self.Position.x = 0

		if self.Position.x < -1.0:
			self.Position.x = screenProperties['width']

		if self.Position.y > (float(screenProperties['height'] + 1.0)):
			self.Position.y = 0

		if self.Position.y < -1.0:
			self.Position.y = screenProperties['height']

	def draw(self):
		
		AIE.DrawSprite( self.spriteID )
		self.turret.draw()
		
	def getImageName(self):
		return self.imageName
		
	def getState(self):
		return self.state
	
	def getSpriteID(self):
		return self.spriteID
		
	def setSpriteID(self, a_spriteID):
		self.spriteID = a_spriteID
	
	def getPosition(self):
		return self.Position

	def cleanUp(self):
		self.turret.cleanUp()
		AIE.DestroySprite( self.spriteID )
		
#Turret
#    This is an Entity Object that has an owner, it is up to you to implement inheritance (BaseEntity->Turret) 
#    The Turret's position is based on the location of it's owner, if it's owner (in this scenario a Tank) is moveable
#    The turret will move with it's base/owner

class Turret:
	
	def __init__(self, owner):
		self.owner = owner
		self.Position = ( 0, 0 )
		self.Rotation = 0
		self.spriteName = "./images/PlayerTanks.png"
		self.size = (29, 60 )
		self.origin = (0.55, 0.75)
		self.spriteID = AIE.CreateSprite( self.spriteName, self.size[0], self.size[1], self.origin[0], self.origin[1], 129.0/459.0, 1.0 - 61.0/158.0, 157.0/459.0, 1.0 , 0xff, 0xff, 0xff, 0xff )
		print "spriteID", self.spriteID
	
	def update(self, fDeltaTime):
		turretLocation = self.owner.getPosition()
		AIE.MoveSprite( self.spriteID, turretLocation[0], turretLocation[1] )
		
	def draw(self):
		AIE.DrawSprite( self.spriteID )
	
	def	cleanUp(self):
		AIE.DestroySprite( self.spriteID )
		