# Week 1 Journal Entry

The thresholds for my final project have been met for week 1. Getting back into Python was a little clunky, but a quick reminder about the syntax around classes and already the coding aspect is feeling a little smoother than working with C++. Although that's an interesting language and I want to deepen my understanding of it, I think the language choice is already proving to be the correct one so far.

As far as the particle and particle system objects created this week, these are basic implementations but they work well enough that it's definitely sharpening my interest in what future iterations will be capable of. 

Initially I had ps.update as a single call. When running a single update, execution time was negligible and I predictably wasn't seeing any decay. However, running multiple updates in a loop introduced noticeable delay (~1–2 seconds at 100K particles), reflecting the O(n) cost of iterating over all particles each frame. This demonstrates how even linear-time operations become significant at scale.

The fairly small but noticeable time delay at this stage also makes me realize how optimizing operations will become increasingly important. The linked list, as this will be my own implementation, should be a challenge in this regard, but that will be tackled beginning with next week's project-phase.