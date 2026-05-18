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

# Create a problem instance
initial = (1, 2, 3, 4, 0, 5, 6, 7, 8)  # 0 represents blank
goal = (0, 1, 2, 3, 4, 5, 6, 7, 8)

problem = EightPuzzle(initial, goal)

print("PROBLEM FORMULATION DEMONSTRATION")
print("=" * 50)
print("\nInitial State:")
problem.display_state(initial)

print("\nGoal State:")
problem.display_state(goal)

print("\nAvailable actions from initial state:", problem.get_actions(initial))

print("\nApplying action 'UP':")
new_state = problem.result(initial, 'UP')
problem.display_state(new_state)

print("\nIs new state the goal?", problem.is_goal(new_state))
print("Is goal state the goal?", problem.is_goal(goal))

print("\nPath cost: Each move costs 1 (implicit in our formulation)")
