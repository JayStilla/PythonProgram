import AIE
import game

import Collision
from Collision import CircCollision
#Tank Entity
#   A simple entity that can be placed on the screen with a right click, you should modify this so that the tank can be told to 
#   navigate to a point instead of instantly move.

#start of custom classes
class LinkedListNode:
	def __init__(self, inData, inNext):
		self.data = inData
		self.next = inNext

class LinkedList:
	def __init__(self):
		self.firstNode = LinkedListNode(None, None)
		self.lastNode = self.firstNode
		self.size = 0
	def add(self, data):
		node = LinkedListNode(data, None)
		node.data = data
		
		if self.firstNode.data == None:
			self.firstNode = node
			self.lastNode = node
		else:
			self.lastNode.next = node
			self.lastNode = node
			
		self.size += 1
	
	def add_many(self, list_of_data):
		for x in list_of_data:
			self.add(x)
	
	def remove(self, data):
		currentNode = self.firstNode
		wasDeleted = False
		
		if self.size == 0:
			pass
		
		if data == currentNode.data:
			if currentNode.next == None:
				self.firstNode = LinkedListNode(None, None)
				self.lastNode = self.firstNode
				self.size = self.size - 1
				return
				
			currentNode = currentNode.next
			self.firstNode = currentNode
			self.size = self.size - 1
			return
		
		while True:
			if currentNode == None:
				wasDeleted = False
				break
			
			nextNode = currentNode.next
			if nextNode != None:
				if data == nextNode.data:
					nextNextNode = nextNode.next
					currentNode.next = nextNextNode
					
					nextNode = None
					wasDeleted = True
					break
			currentNode = currentNode.next
		if wasDeleted:
			self.size = self.size - 1
			
	def remove_many(self, list_of_data):
		for x in list_of_data:
			self.remove(x)

#end of custom node and list classes

class TankEntity:

	def __init__(self):
		self.Position = ( 1000, 400 )
		self.Rotation = 0
		self.spriteName = "./images/PlayerTanks.png"
		self.size = (57, 72 )
		self.Collider = CircCollision(self, (self.size[1] / 2.0))
		self.origin = (0.5, 0.5)
		self.spriteID = AIE.CreateSprite( self.spriteName, self.size[0], self.size[1], self.origin[0], self.origin[1], 71.0/459.0, 1.0 - 72.0/158.0, 128/459.0, 1.0 , 0xff, 0xff, 0xff, 0xff )
		print "spriteID", self.spriteID
		#Move Tile to appropriate location
		
		self.turret = Turret(self)
	#start of custom functions
	def seek(self, mouseX, mouseY):
		if(self.Position !=(mouseX, mouseY)):
			lerpX = (self.Position[0] + (mouseX - self.Position[0]) * 0.1)
			lerpY = (self.Position[1] + (mouseY - self.Position[1]) * 0.1)
			lerp = (lerpX, lerpY)
			return lerp
		else:
			pass
	
	def flee(self, mouseX, mouseY):
		lerpX = (self.Position[0] - (mouseX - self.Position[0]) * 0.1)
		lerpY = (self.Position[1] - (mouseY - self.Position[1]) * 0.1)
		lerp = (lerpX, lerpY)
		return lerp
		
	#end of custom functions
		
	def update(self, fDeltaTime ):
		#Update Collider
		self.Collider.update(self)
	
		mouseX, mouseY = AIE.GetMouseLocation()
		if( AIE.GetMouseButton(1)  ):
			self.Position = self.seek(mouseX, mouseY)
		elif( AIE.GetMouseButton(0) ):
			self.Position = self.flee(mouseX, mouseY)
		AIE.MoveSprite( self.spriteID, self.Position[0], self.Position[1] ) # write Python side positions to C++ side positions
		self.turret.update(fDeltaTime)
		
	
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
		