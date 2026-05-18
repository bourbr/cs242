import heapq

class GridWorld:
    """A grid world with varying terrain costs."""
    
    def __init__(self, grid, start, goal):
        self.grid = grid  # 2D list where values are terrain costs
        self.start = start
        self.goal = goal
        self.rows = len(grid)
        self.cols = len(grid[0])
    
    def get_actions(self, state):
        """Returns valid moves from state."""
        row, col = state
        actions = []
        
        if row > 0: actions.append(('UP', row - 1, col))
        if row < self.rows - 1: actions.append(('DOWN', row + 1, col))
        if col > 0: actions.append(('LEFT', row, col - 1))
        if col < self.cols - 1: actions.append(('RIGHT', row, col + 1))
        
        return actions
    
    def step_cost(self, state, action):
        """Returns cost of moving to new position."""
        _, new_row, new_col = action
        return self.grid[new_row][new_col]
    
    def is_goal(self, state):
        return state == self.goal

def ucs_search(problem):
    """Uniform-cost search implementation."""
    
    # Priority queue: (cost, counter, state, path)
    counter = 0  # For tie-breaking
    initial_node = (0, counter, problem.start, [])
    frontier = [initial_node]
    explored = set()
    nodes_expanded = 0
    
    print("UNIFORM-COST SEARCH")
    print("=" * 50)
    print("\nTerrain costs (lower is better):")
    for row in problem.grid:
        print(" ", row)
    print(f"\nStart: {problem.start}, Goal: {problem.goal}\n")
    
    while frontier:
        cost, _, state, path = heapq.heappop(frontier)  # Get lowest-cost node
        
        if state in explored:
            continue
        
        nodes_expanded += 1
        print(f"Expanding node {nodes_expanded}: {state}, Cost: {cost}")
        
        if problem.is_goal(state):
            print(f"\n✓ Goal found! Total nodes expanded: {nodes_expanded}")
            print(f"Optimal path: {path}")
            print(f"Total cost: {cost}")
            return path, cost
        
        explored.add(state)
        
        for action in problem.get_actions(state):
            action_name, new_row, new_col = action
            child_state = (new_row, new_col)
            
            if child_state not in explored:
                step_cost = problem.step_cost(state, action)
                child_cost = cost + step_cost
                child_path = path + [action_name]
                counter += 1
                
                heapq.heappush(frontier, (child_cost, counter, child_state, child_path))
    
    return None, float('inf')

# Grid where numbers represent terrain difficulty (cost to enter)
grid = [
    [1, 1, 1, 1, 1],
    [1, 5, 5, 5, 1],
    [1, 5, 1, 5, 1],
    [1, 1, 1, 5, 1],
    [1, 1, 1, 1, 1]
]

problem = GridWorld(grid, start=(0, 0), goal=(4, 4))
solution, cost = ucs_search(problem)
