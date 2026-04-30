# Week 2 Journal Entry

Output thresholds for week 2 have been met. There is a rudimentary linked list data structure working alongside the pre-existing array structure and both are successfully benchmarking at 100k particle elements.

At this input level, the linked list implementation is slightly faster than the array, which was surprising to see. This could be down to a few reasons, and one that sticks out the most would be the fact that arrays in python are, as far as I understand, contiguously stored pointers rather than actual elements at contiguous memory addresses. This is a kind of inbetween as far as the efficiency of a linked list and a true array. 

Still, I imagine at larger values of n, without optimization, the array structure would overtake my linked list structure; even though python is storing the array items as objects in the heap, the caching of their pointers adds an access efficiency which the linked list will never be able to leverage. But the referencing and dereferencing of these python pointers in the array are still making this structure less efficient than it would have been in c++. Using Numpy and leveraging vectorization should also give the array structure a decisive advantage as well.

Next week I'll begin marching toward the 1 million mark, expanding and optimizing this existing structure to handle larger total element values. 

1. What I built this week

Output thresholds for Week 2 have been met. I implemented a rudimentary linked list data structure to run alongside the pre-existing array-based structure. Both systems now support basic CRUD operations and are successfully benchmarking at around 100K particle elements.

The code now allows for direct comparison between the two implementations under the same workload, which was not possible last week.

2. Code Highlight

```cpp
def update(self):
    current = self.head
    prev = None

    while current:
        p = current.particle
        p.update()

        if p.life <= 0:
            if prev:
                prev.next = current.next
            else:
                self.head = current.next

            if current == self.tail:
                self.tail = prev

            self.size -= 1
        else:
            prev = current

        current = current.next
```

3. Complexity checkpoint

Both implementations currently operate in O(n) time for updates:

- Array version: O(n) traversal + O(n) reconstruction (due to rebuilding the list each frame)
- Linked list version: O(n) traversal with O(1) deletions via pointer updates

Compared to last week, complexity has not changed, but the implementation detail has—I now have two structures with different constant factors and memory behaviors.

4. What broke / what was hard

The most challenging part this week was correctly implementing deletion in the linked list while maintaining head and tail pointers. Handling edge cases (such as deleting the first or last node) required careful pointer management.

Another difficulty was interpreting the benchmark results. The linked list performed slightly faster at 100K elements, which was unexpected and required some investigation into Python’s memory model and overhead costs.

5. Plan for next week

By next Thursday, I need to scale both implementations toward 1 million particles and begin refining performance measurement. This will likely require optimizing the update loop and ensuring the system can handle larger workloads without excessive slowdown or memory issues.

As part of this, I will begin familiarizing myself with NumPy and incorporating vectorization into the array-based implementation. This should reduce Python-level iteration overhead and allow operations to be applied across entire particle datasets more efficiently.