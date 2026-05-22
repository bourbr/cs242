# Journal Entry Week 5

## 1. What I built this week

This week focused on extending the benchmarking and analysis layer of the particle system rather than modifying the underlying data structures. The main addition was a benchmarking pipeline that logs results to a CSV file.

This allows performance data from all three implementations (Python list, linked list, and NumPy vectorized arrays) to be saved in a reproducible format instead of only being printed to the console. In addition, a plotting workflow was introduced to visualize performance scaling across increasing particle counts.

## 2. Code highlight

```py
import csv

def save_results_csv(results, filename="benchmark_results.csv"):
    with open(filename, "w", newline="") as f:
        writer = csv.writer(f)

        writer.writerow(["System", "N", "Time(s)", "Remaining", "PPS"])

        for r in results:
            writer.writerow([
                r["name"],
                r["N"],
                r["time"],
                r["remaining"],
                r["pps"]
            ])
```
This function writes benchmark results into a CSV file after each run. This was added to support the above mentioned plot comparison of the different data structures over multiple input sizes. There's an interesting quirk with VectArray at n=100000 made evident by the plot that I plan to explore in the research for my writeup.

## 3. Complexity checkpoint

Space and time complexity of the three structures remain the same.

## 4. What broke / what was hard

Nothing implementation-wise was a hurdle this week; mainly focused on interpretation of results and how to convey their relevance in my presentation. I have a few running ideas, mainly circling the notion of efficiency and compute in relation to ever increasing levels of data-collection and storage. 

## 5. Plan for next week

Next week will focus on finalizing the project for submission. This includes:

- Completing write-up and complexity analysis
- Interpreting benchmark and visualization results in detail
- Refining explanations of performance differences between data structures
- Final code checks and refinements where sensible

The emphasis will shift away from implementation and toward synthesis, explanation, and presentation of results.