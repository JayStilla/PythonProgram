import math
import Entity

class CircCollision(object):
	def __init__(self, entity, radius):
		self.Position = entity.Position
		self.Radius = radius
		
	def update(self, entity):
		self.Position = entity.Position
	
	@staticmethod
	def checkCollision(firstCircle, secondCircle):
		#calculate radius between two circles
		distX = (float(firstCircle.Position[0]) - float(secondCircle.Position[0]))
		distY = (float(firstCircle.Position[1]) - float(secondCircle.Position[1]))

		distSquared = (float((distX * distX))) - (float((distY * distY)))

		if(float(distSquared))<=(float((firstCircle.Radius + secondCircle.Radius) ** 2)):
			print "You hit Some Shit"
			return True
		else:
			return False