
class EightPuzzle:
    """Represents an 8-puzzle problem."""
    
    def __init__(self, initial_state, goal_state):
        self.initial = initial_state
        self.goal = goal_state
    
    def get_actions(self, state):
        """Returns list of valid actions from this state."""
        actions = []
        blank_pos = state.index(0)  # Find the blank tile
        row, col = blank_pos // 3, blank_pos % 3
        
        if row > 0: actions.append('UP')
        if row < 2: actions.append('DOWN')
        if col > 0: actions.append('LEFT')
        if col < 2: actions.append('RIGHT')
        
        return actions
    
    def result(self, state, action):
        """Returns the state that results from executing action."""
        new_state = list(state)
        blank_pos = state.index(0)
        row, col = blank_pos // 3, blank_pos % 3
        
        # Calculate new position based on action
        if action == 'UP':
            new_pos = (row - 1) * 3 + col
        elif action == 'DOWN':
            new_pos = (row + 1) * 3 + col
        elif action == 'LEFT':
            new_pos = row * 3 + (col - 1)
        elif action == 'RIGHT':
            new_pos = row * 3 + (col + 1)
        
        # Swap blank with tile in new position
        new_state[blank_pos], new_state[new_pos] = new_state[new_pos], new_state[blank_pos]
        return tuple(new_state)
    
    def is_goal(self, state):
        """Tests if state is the goal."""
        return state == self.goal
    
    def display_state(self, state):
        """Pretty print a state."""
        for i in range(0, 9, 3):
            print(f"  {state[i]} {state[i+1]} {state[i+2]}")
            
def dfs_search(problem, max_depth=100):
    """Depth-first search with depth limiting."""
    
    initial_node = (problem.initial, [], 0)
    
    if problem.is_goal(problem.initial):
        return []
    
    frontier = [initial_node]  # LIFO stack (use list as stack)
    explored = set()
    nodes_expanded = 0
    max_frontier_size = 1
    
    print("DEPTH-FIRST SEARCH")
    print("=" * 50)
    
    while frontier:
        max_frontier_size = max(max_frontier_size, len(frontier))
        
        # Remove last node from frontier (LIFO)
        state, path, cost = frontier.pop()
        
        if len(path) > max_depth:
            continue  # Depth limit reached
        
        nodes_expanded += 1
        print(f"\nExploring node {nodes_expanded} (depth {len(path)}):")
        problem.display_state(state)
        
        if problem.is_goal(state):
            print(f"\n{'='*50}")
            print(f"✓ Goal found! Total nodes expanded: {nodes_expanded}")
            print(f"Maximum frontier size: {max_frontier_size}")
            print(f"\nGoal state reached:")
            problem.display_state(state)
            print(f"\nSolution path: {path}")
            print(f"Path length: {len(path)}")
            return path
        
        explored.add(state)
        
        # Show available actions
        actions = problem.get_actions(state)
        print(f"  Available actions: {actions}")
        
        # Add children to frontier (in reverse order for consistent behavior)
        for action in reversed(actions):
            child_state = problem.result(state, action)
            print(f"  Applying '{action}' →", end=" ")
            
            if child_state not in explored and child_state not in [n[0] for n in frontier]:
                child_path = path + [action]
                child_cost = cost + 1
                frontier.append((child_state, child_path, child_cost))
                print("added to frontier")
            else:
                if child_state in explored:
                    print("already explored")
                else:
                    print("already in frontier")
    
    return None

initial = (8, 6, 7, 2, 5, 4, 3, 0, 1)
goal    = (1, 2, 3, 4, 5, 6, 7, 8, 0)

problem = EightPuzzle(initial, goal)
solution = dfs_search(problem)
