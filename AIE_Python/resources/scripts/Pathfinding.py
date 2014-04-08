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