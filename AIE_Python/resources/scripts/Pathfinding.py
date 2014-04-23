class PathFinder(object):
	#A* pathfinding algorithm 
	def __init__(self, successors, move_cost, heuristic_to_goal):
		self.successors = successors
		self.move_cost = move_cost
		self.heuristic_to_goal = heuristic_to_goal

	def compute_path(self, start, goal):
		closed_set = {}

		start_node = self.Node(start)
		start_node.g_cost = 0
		start_node.f_cost = self.compute_f_cost(start_node, goal)

		#These will make sense when a priority request class is made
		open_set = PriorityQueueSet()
		open_set.add(start_node)

		while len(open_set) > 0:
			current_node = open_set.pop_smallest()

			if current_node.coord == goal:
				return self.reconstruct_path(current_node)

			closed_set[current_node] = current_node

			for succ_coord in self.successors(current_node.coord):
				succ_node = self._Node(succ_coord)
				succ_node.g_cost = self.compute_g_cost(current_node, succ_node)
				succ_node.f_cost = self.compute_f_cost(succ_node, goal)

				if succ_node in closed_set:
					continue

				if open_set.add(succ_node):
					succ_node.pred = current_node
		return []

	def compute_g_cost(self, from_node, to_node):
		return(from_node.g_cost + self.move_cost(from_node.coord, to_node.coord))

	def compute_f_cost(self, node, goal):
		return node.g_cost + self.cost_to_goal(node, goal)

	def cost_to_goal(self, node, goal):
		return self.heuristic_to_goal(node.coord, goal)

	def reconstruct_path(self, node):
		path = [node.coord]
		n = node
		while n.pred:
			n = n.pred
			path.append(n.coord)

		return reversed(path)

	class Node(object):
		def __init__(self, coord, g_cost=None, f_cost = None, pred=None):
			self.coord = coord
			self.g_cost = g_cost
			self.f_cost = f_cost
			self.pred = pred

		def __eq__(self, other):
			return self.coord == other.coord

		def __cmp__(self, other):
			return cmp(self.f_cost other.f_cost)

		def __hash__(self):
			return hash(self.coord)
		def __str__(self):
			return 'N(%s) -> g: %s f: %s' % (self.coord, self.g_cost, self.f_cost)

		def __repr__(self):
			return self.__str__()
upo 
if __name__ == "__main__":
	from gridmap import gridmap

	start = 0, 0
	goal = 1, 7

	tm = gridmap(8, 8)
	for b in [ (1, 1), (0, 2), (1, 2), (0,3), (2, 3), (2, 5), (2, 5), (2, 5), (2, 7)]:
		tm.set_blocked(b)

	tm.printme()

	pf = PathFinder(tm.successors, tm.move_cost, tm.move_cost)

	import time 
	t = time.clock()