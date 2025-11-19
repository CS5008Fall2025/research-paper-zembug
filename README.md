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

| Scenario                | Time Complexity      | Space Complexity |
| :--                     | :--                  | :--              |
| One ant, one iteration  | $O(n^2)$             | $O(n^2)$         |
| $k$ ants, one iteration | $O(k \cdot n^2)$     | $O(n^2)$         |
| $k$ ants, $t$ iterations| $O(t \cdot k \cdot n^2)$ | $O(n^2)$     |
| Simplified reporting    | $O(k \cdot n^2)$     | $O(n^2)$         |


Space Complexity:

    Ant Colony Optimization requires memory to store pheromone values and track the paths taken by ants. The largest part of this storage comes from the pheromone matrix, which keeps a value for every edge in the graph. In a fully connected graph with $n$ nodes, there are $O(n^2)$ edges, so the pheromone matrix alone requires $O(n^2)$ space. Each ant also maintains a record of the nodes it has visited (often called a tabu list) and the cost of its current path. This adds about $O(n)$ space per ant, but compared to the pheromone matrix, this is relatively small. Overall, the space complexity of ACO is dominated by the pheromone storage, giving a total of $O(n^2)$. [1][2]


## Empirical Analysis
- What is the empirical analysis?
- Provide specific examples / data.


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