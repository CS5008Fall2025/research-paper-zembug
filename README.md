[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/zBqi0PeJ)
# Research Paper
* Name: Emily Despres
* Semester: Fall 2026
* Topic: Ant Colony Optimization (ACO)



Note the following is an example outline to help you. Please rework as you need, you do not need to follow the section heads and *YOU SHOULD NOT* make everything a bulleted list. This needs to read as an executive report/research paper. 

## Introduction
- What is the algorithm/datastructure?
- What is the problem it solves? 
- Provide a brief history of the algorithm/datastructure. (make sure to cite sources)
- Provide an introduction to the rest of the paper. 

Ant Colony Optimization (ACO) is an algorithm inspired by the cooperative behavior of ants in nature. When ants search for food, they explore their environment and leave behind pheromone trails. These chemical signals act as a form of indirect communication, guiding other ants toward promising paths. Over time, trails along shorter or more efficient routes become stronger as more ants reinforce them, while less effective paths fade away. This simple biological process allows an ant colony, without any central control, to collectively discover near optimal solutions to complex problems such as finding the shortest path between their nest and a food source. ACO takes this idea and translates it into a computational model, where artificial “ants” explore graphs and update pheromone levels to gradually converge on good solutions. [1][2]

The algorithm was first introduced in the early 1990s by Marco Dorigo as part of his doctoral research. His initial version, called the Ant System (AS), laid the foundation for later refinements such as the Ant Colony System (ACS) and the MAX–MIN Ant System (MMAS), each designed to improve efficiency and avoid premature convergence. Since its introduction, ACO has grown into a family of related algorithms widely applied to NP‑hard problems like the Traveling Salesman Problem, vehicle routing, and scheduling. Instead of guaranteeing the absolute best answer, ACO uses probability and feedback to find solutions that are close to optimal in a reasonable amount of time. This report examines how the algorithm works, analyzes its complexity, presents test results, and explores real world applications in areas like telecommunications and logistics. By looking at both the strengths and challenges of ACO, the report shows how biologically inspired computing continues to provide creative strategies for solving difficult problems. [1][2]


## Analysis of Algorithm/Datastructure
Make sure to include the following:
- Time Complexity
- Space Complexity
- General analysis of the algorithm/datastructure

Time Complexity:

The runtime of Ant Colony Optimization depends on how many ants are used, how many iterations the algorithm runs, and the size of the graph. Each ant constructs a solution by exploring nodes and updating pheromone trails. For a graph with $n$ nodes, this process can take up to $O(n^2)$ steps in a single iteration, since ants may evaluate many possible edges. [2]

Space Complexity:

Ant Colony Optimization requires memory to store pheromone values and track the paths taken by ants. The largest part of this storage comes from the pheromone matrix, which keeps a value for every edge in the graph. In a fully connected graph with $n$ nodes, there are $O(n^2)$ edges, so the pheromone matrix alone requires $O(n^2)$ space. Each ant also maintains a record of the nodes it has visited (often called a tabu list) and the cost of its current path. This adds about $O(n)$ space per ant, but compared to the pheromone matrix, this is relatively small. Overall, the space complexity of ACO is dominated by the pheromone storage, giving a total of $O(n^2)$. [1][2]

| Scenario                | Time Complexity      | Space Complexity |
| :--                     | :--                  | :--              |
| One ant, one iteration  | $O(n^2)$             | $O(n^2)$         |
| $k$ ants, one iteration | $O(k \cdot n^2)$     | $O(n^2)$         |
| $k$ ants, $t$ iterations| $O(t \cdot k \cdot n^2)$ | $O(n^2)$     |
| Simplified reporting    | $O(k \cdot n^2)$     | $O(n^2)$         |


General analysis of the algorithm/datastructure:

ACO is a probabilistic search algorithm designed for complex optimization problems. Instead of guaranteeing exact optimality like deterministic algorithms such as Dijkstra’s, it relies on iterative exploration and reinforcement to find near optimal solutions. Its main strengths are adaptability to changing environments, the ability to run many ants at once, and robustness in large search spaces. ACO has been successfully applied to problems like the Traveling Salesman Problem, scheduling, and routing, where exact methods are impractical. At the same time, ACO has weaknesses. It converges more slowly than deterministic algorithms, requires careful parameter tuning, and can reinforce suboptimal paths if exploration is limited. Memory usage is also significant, since the pheromone matrix requires $O(n^2)$ space in dense graphs. Overall, ACO is not the fastest or most memory efficient method, but it is flexible and effective for NP‑hard problems where approximate solutions are acceptable. [2]


## Empirical Analysis
- What is the empirical analysis?
- Provide specific examples / data.

To evaluate the performance of Ant Colony Optimization (ACO), I conducted experiments varying the algorithm’s parameters: pheromone influence (α), heuristic influence (β), evaporation rate (ρ), and deposit amount (Q). Each run produced a convergence log in CSV format, recording both the best solution found in each iteration and the overall global best solution. At the end of each run, a “Final” row was appended to summarize the colony’s global best path length, cost, normalized cost, and improvement factor.

The normalized cost was calculated by dividing the best path cost by a baseline “chain” path cost, defined as $1.1 \cdot (𝑛 − 1)$ for a graph of $𝑛$
nodes. This provides a scale free measure of efficiency, allowing comparisons across different graph sizes. The improvement factor was computed as the ratio of the baseline cost to the best path cost, showing how much better the colony’s solution was compared to the trivial chain.

# ACO Parameter Comparison

| Run | α (Alpha) | β (Beta) | ρ (Evaporation) | Q (Deposit) | GlobalBestLength | GlobalBestCost | NormBestCost | ImprovementFactor |
|-----|-----------|----------|-----------------|-------------|------------------|----------------|--------------|-------------------|
| 1   | 1.0       | 0.5      | 0.1             | 1.0         | 13               | 18.40          | 0.3414       | 2.9293            |
| 2   | 1.0       | 3.0      | 0.5             | 10.0        | 13               | 18.40          | 0.3414       | 2.9293            |
| 3   | 0.5       | 4.0      | 0.6             | 10.0        | 13               | 18.40          | 0.3414       | 2.9293            |
| 4   | 2.0       | 2.0      | 0.4             | 10.0        | 13               | 18.40          | 0.3414       | 2.9293            |
| 5   | 1.0       | 2.0      | 0.5             | 1.0         | 13               | 18.40          | 0.3414       | 2.9293            |
| 6   | 1.0       | 3.0      | 0.3             | 10.0        | 13               | 18.40          | 0.3414       | 2.9293            |
| 7   | 1.0       | 3.0      | 0.6             | 10.0        | 13               | 18.40          | 0.3414       | 2.9293            |
| 8   | 0.5       | 3.0      | 0.4             | 10.0        | 13               | 18.40          | 0.3414       | 2.9293            |
| 9   | 2.0       | 1.0      | 0.5             | 10.0        | 13               | 18.40          | 0.3414       | 2.9293            |
| 10  | 1.5       | 3.0      | 0.4             | 10.0        | 13               | 18.40          | 0.3414       | 2.9293            |

Each run in the experiment used different parameter settings, which shaped how the ant colony explored the graph. Alpha (α) controls how strongly ants follow pheromone trails, while Beta (β) determines how much they rely on heuristic information such as edge length. Evaporation (ρ) sets the rate at which pheromone trails fade, encouraging exploration when it is high, and Deposit (Q) is the amount of pheromone added when a path is found, reinforcing successful solutions more strongly when it is large. The outcome columns summarize what the colony achieved: GlobalBestLength is the number of nodes in the best path discovered, and GlobalBestCost is the total cost of that path. NormBestCost expresses this cost on a standardized 0–1 scale, showing how close the solution is to the optimal compared to the worst possible path in the graph. ImprovementFactor shows how much better the final solution was compared to the baseline.

In this experiment, all ten runs converged to the same final solution: a 13‑node path with a cost of 18.40, normalized to 0.3414, and an improvement factor of 2.9293. This consistency reflects the graph’s structure, which strongly favored the shortcut path. The parameters did not change the destination, but they did influence the journey — some runs locked into the optimal path almost immediately, while others wandered through longer detours before reinforcing the best solution. The final row captures only the end result, which is why iteration logs are essential: they reveal how exploration dynamics differ even when the colony ultimately finds the same optimum.

Although every run ended with the same final solution (a 13‑node path with cost 18.40) the way each colony reached that solution was very different. The Final Results table captures only the destination, but the iteration logs reveal the journey. Some runs converged almost instantly, locking onto the optimal path from the very first iteration. Others wandered through longer detours, reinforcing suboptimal paths before eventually rediscovering the shortcut. In a few cases, the colony oscillated between multiple path lengths, showing unstable reinforcement before settling. This contrast highlights the role of parameters: they did not change the ultimate answer in this graph, but they shaped the exploration process, influencing whether ants found the best path quickly, slowly, or unpredictably. The Iteration Comparison table makes these differences visible, showing that the colony’s dynamics depend on how pheromone influence, heuristic bias, evaporation, and deposit strength interact during the search.

![Best Path Cost Convergence](bestpathcost)


## Application
- What is the algorithm/datastructure used for?
- Provide specific examples
- Why is it useful / used in that field area?
- Make sure to provide sources for your information.


## Implementation
- What language did you use?
- What libraries did you use?
- What were the challenges you faced?
- Provide key points of the algorithm/datastructure implementation, discuss the code.
- If you found code in another language, and then implemented in your own language that is fine - but make sure to document that.


## Summary
- Provide a summary of your findings
- What did you learn?



## References
1. GeeksforGeeks. 2025. Introduction to Ant Colony Optimization. (July 12, 2025). Retrieved November 18, 2025 from https://www.geeksforgeeks.org/machine-learning/introduction-to-ant-colony-optimization/

2. Wikipedia. 2025. Ant colony optimization algorithms. (October 1, 2025).  Retrieved November 18, 2025 from https://en.wikipedia.org/wiki/Ant_colony_optimization_algorithms