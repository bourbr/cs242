# CPTR 242 Final Project Proposal

# Brendan Bourassa

I’ve decided to tackle the particle engine project-track. In this vein, I’ll attempt to implement a particle simulation engine capable of managing up to 1,000,000 particles in real time. The system will simulate simple physical behaviors such as motion, velocity decay, and lifetime-based removal. The primary focus is not graphical complexity, but efficient data storage, update, and deletion of large numbers of dynamic objects.

The engine will visually represent particles as simple points in a 2D space using a lightweight visualization library, allowing performance characteristics of different data structures to be observed directly.

The project will employ both array and linked-list based data structures and compare these against one another. I’m going to use Python’s list as the array structure, even though it isn’t a pure array, because I think Python will be easier to work with and ensure (or better ensure) a working and complete product by the deadline. The linked list will be a custom implementation, but will employ the features of linked lists we have learned up to this point which make them most efficient (head, tail, dummy nodes). I’ll use Pygame for the visualization aspect, and the time module or timeit with perhaps some custom implementation for benchmarking.

Finally, a rough road-map of the coming weeks:

## Week 1:
-	Project setup + particle structure design 
-	Basic array-based particle system working 

## Week 2:
-	Implement linked list version 
-	Basic CRUD operations complete for both 

## Week 3:
-	Optimize update loop for 100K–1M particles 
-	Initial benchmarking framework 

## Week 4:
-	Add visualization using Pygame 
-	Display live particle system 

## Week 5:
-	Full benchmarking (10K / 100K / 1M) 
-	Begin complexity analysis write-up 

## Week 6:
-	Final polish 
-	Complete reflection write-up 
-	Prepare demo and presentation 

This project is reasonable in scope, I believe, and within the bounds of my coding experience to complete in the given time available. Implementing the linked list and using Pygame will be the most novel elements, but won’t really stretch my current abilities to the point that I’m lost with respect to the function of either. It should be a good but not excessive coding challenge and an opportunity to see the out-working of the themes we’ve covered in the course so far (and likely those to come).
