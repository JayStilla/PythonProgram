from collections import defaultdict 
from math import sqrt

class Grid(object):
	def __init__(self, nrows, ncols):
		self.nrows = nrows
		self.ncols = ncols

		self.map = [[0] * self.ncols for i in range(self.nrows)]
		self.blocked = defaultdict(lambda: False)

	def set_blocked(self, coord, blocked=True):
		self.map[coord[0]][coord[1]] = blocked

		if blocked:
			self.blocked[coord] = True
		else:
			if coord in self.blocked:
				del self.blocked[coord]

	def move_cost(self, c1, c2):
		return sqrt((c1[0] - c2[0]) ** 2 + (c1[1] - c2[1]) ** 2)

	def successors(self, c):
		slist = []

		for drow in (-1, 0, 1):
			for dcol in (-1, 0, 1):
				if drow == 0 and dcol == 0:
					continue

				newrow = c[0] +drow
				newcol = c[1] +dcol
				if(0 <= newrow <= self.nrows - 1 and
					0 <= newcol <= self.ncols - 1 and
					self.map[newrow][newcol] == 0):
					slist.append((newrow, newcol))

		return slist 

	def printme(self):
		for row in range(slf.nrows):
			for col in range(self.ncols):
				print "%s" % ('O' if self.map[row][col] else '.'),
				print ''