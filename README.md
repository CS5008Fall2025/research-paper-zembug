[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/zBqi0PeJ)
# Research Paper
* Name: Emily Despres
* Semester: Fall 2026
* Topic: Ant Colony Optimization (ACO)


## Introduction

Ant Colony Optimization (ACO) is an algorithm inspired by the cooperative behavior of ants in nature. When ants search for food, they explore their environment and leave behind pheromone trails. These chemical signals act as a form of indirect communication, guiding other ants toward promising paths. Over time, trails along shorter or more efficient routes become stronger as more ants reinforce them, while less effective paths fade away. This simple biological process allows an ant colony, without any central control, to collectively discover near-optimal solutions to complex problems such as finding the shortest path between their nest and a food source. ACO takes this idea and translates it into a computational model, where artificial “ants” explore graphs and update pheromone levels to gradually converge on good solutions.[1][2]

The algorithm was first introduced in the early 1990s by Marco Dorigo as part of his doctoral research. His initial version, called the Ant System (AS), laid the foundation for later refinements such as the Ant Colony System (ACS) and the MAX–MIN Ant System (MMAS), each designed to improve efficiency and avoid premature convergence.[5] Since its introduction, ACO has grown into a family of related algorithms widely applied to NP‑hard problems like the Traveling Salesman Problem, vehicle routing, and scheduling. Instead of guaranteeing the absolute best answer, ACO uses probability and feedback to find solutions that are close to optimal in a reasonable amount of time. 

This report introduces the mechanics of ACO and then evaluates its scalability. By fixing key parameters and varying only graph size, we analyze how runtime, memory usage, and solution quality change as the problem grows. This highlights both the strengths and limitations of ACO when applied to graphs of growing size. [1][2][4]


## Analysis of Algorithm/Datastructure

Time Complexity:

The runtime of ACO depends on how many ants are used, how many iterations the algorithm runs, and the size of the graph. Each ant constructs a solution by exploring nodes and updating pheromone trails. For a graph with $n$ nodes, this process can take up to $O(n^2)$ steps in a single iteration, since ants may evaluate many possible edges.[2][4]

Space Complexity:

ACO requires memory to store pheromone values and track the paths taken by ants. The largest part of this storage comes from the pheromone matrix, which keeps a value for every edge in the graph. In a fully connected graph with $n$ nodes, there are $O(n^2)$ edges, so the pheromone matrix alone requires $O(n^2)$ space. Each ant also maintains a record of the nodes it has visited (often called a tabu list) and the cost of its current path. This adds about $O(n)$ space per ant, but compared to the pheromone matrix, this is relatively small. Overall, the space complexity of ACO is dominated by the pheromone storage, giving a total of $O(n^2)$.[1][2]

| Scenario                | Time Complexity      | Space Complexity |
| :--                     | :--                  | :--              |
| One ant, one iteration  | $O(n^2)$             | $O(n^2)$         |
| $k$ ants, one iteration | $O(k \cdot n^2)$     | $O(n^2)$         |
| $k$ ants, $t$ iterations| $O(t \cdot k \cdot n^2)$ | $O(n^2)$     |
| Simplified reporting    | $O(k \cdot n^2)$     | $O(n^2)$         |

General analysis of the algorithm/datastructure:

ACO is a search method that uses probability to tackle tough optimization problems. Unlike exact algorithms such as Dijkstra’s, it doesn’t guarantee the perfect answer every time. Instead, it works by letting many “ants” explore and gradually reinforce good paths, which leads to efficient solutions. ACO has been successfully applied to problems like the Traveling Salesman Problem, scheduling, and routing, where exact methods are impractical. At the same time, ACO has weaknesses. It converges more slowly than deterministic algorithms, requires careful parameter tuning, and can reinforce suboptimal paths if exploration is limited. Memory use can be high, because the pheromone matrix must store a value for every edge $(O(n^2))$. Overall, ACO may not be the fastest or most memory‑efficient approach, but it is a practical and effective way to find good solutions for very hard problems.[2][5]


## Empirical Analysis

To evaluate the performance of Ant Colony Optimization (ACO), Experiments were run varying the algorithm’s parameters: pheromone influence ($\alpha$), heuristic influence ($\alpha$), evaporation rate ($\rho$), and deposit amount ($Q$). Each run produced a convergence log in CSV format, recording both the best solution found in each iteration and the overall global best solution. At the end of each run, a "Final" row was appended to summarize the colony’s global best path length, cost, normalized cost, and improvement factor.

The normalized cost was calculated by dividing the best path cost by a simple baseline called the “chain” path, defined as $1.1 \cdot (𝑛 − 1)$ for a graph with $n$  nodes. The extra factor of 1.1 makes this baseline slightly larger than the trivial chain, so it’s easier to compare results across different graph sizes. This gives a scale‑free measure of efficiency. The improvement factor was then computed as the baseline cost divided by the best path cost, showing how much better the colony’s solution was compared to the chain path.

ACO Parameter Comparison:

| Run | $\alpha$ (Alpha) | $\beta$ (Beta) | $\rho$ (Evaporation) | Q (Deposit) | GlobalBestLength | GlobalBestCost | NormBestCost | ImprovementFactor |
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

Each run in the experiment used different parameter settings, which shaped how the ant colony explored the graph. Alpha ($\alpha$) controls how strongly ants follow pheromone trails, while Beta ($\beta$) determines how much they rely on heuristic information such as edge length. Evaporation ($\rho$) sets the rate at which pheromone trails fade, encouraging exploration when it is high, and Deposit (Q) is the amount of pheromone added when a path is found, reinforcing successful solutions more strongly when it is large. The outcome columns summarize what the colony achieved: `GlobalBestLength` is the number of nodes in the best path discovered, and `GlobalBestCost` is the total cost of that path. `NormBestCost` serves as a quality score, comparing the colony’s best path cost against a baseline “chain path” cost. Values closer to 0 indicate efficient solutions, while values closer to 1 reflect inefficiency.

In this experiment, all ten runs converged to the same final solution: a 13‑node path with a cost of 18.40, normalized to 0.3414, and an improvement factor of 2.9293. This consistency reflects the graph’s structure, which strongly favored the shortcut path. The parameters did not change the destination, but they did influence the journey. Some runs locked into the optimal path almost immediately, while others wandered through longer detours before reinforcing the best solution. In a few cases, the colony oscillated between multiple path lengths, showing unstable reinforcement before settling. The above results table captures only the destination, but the iteration logs and chart reveal the journey, making visible how different parameter choices shaped the exploration process.[6]

![Best Path Cost Convergence](bestpathcost.png)

The chart of `BestPathCost` across iterations shows these dynamics. Runs such as 4, 5, 9, and 10 dropped immediately to the cost of 18.40 and stayed flat, demonstrating rapid convergence. Runs 1, 2, 7, and 8 spent many iterations at higher costs between 26.90 and 31.90 before eventually rediscovering and locking into the optimum. Runs 3 and 6 illustrate the slowest convergence, oscillating between long detours with costs above 31 and even reaching 40.4 in Run 3 before finally stabilizing at 18.40 near the end. Although every run ended with the same final solution, the chart makes clear that the parameters shaped the path taken to reach it, influencing whether ants found the best path quickly, slowly, or unpredictably. Together, the tables and chart show that understanding convergence requires looking beyond the final row to the full trajectory of exploration.[5]

Convergence Statistics:

| Run | Iterations to Convergence* | Avg. Pre‑Convergence Cost | Max. Detour Cost | Oscillation Notes |
|-----|-----------------------------|---------------------------|------------------|-------------------|
| 1   | ~25                         | 28.7                      | 31.9             | Mild wandering before stabilizing |
| 2   | ~30                         | 29.5                      | 31.9             | Similar to Run 1, delayed lock‑in |
| 3   | ~45                         | 33.8                      | 40.4             | Long oscillations, slowest convergence |
| 4   | ~5                          | 18.4                      | 18.4             | Immediate convergence, no detours |
| 5   | ~5                          | 18.4                      | 18.4             | Same as Run 4, rapid lock‑in |
| 6   | ~40                         | 32.6                      | 36.9             | Oscillated between detours before stabilizing |
| 7   | ~28                         | 27.9                      | 31.9             | Moderate wandering, eventual lock‑in |
| 8   | ~32                         | 29.1                      | 31.9             | Similar to Run 7, delayed convergence |
| 9   | ~5                          | 18.4                      | 18.4             | Immediate convergence, stable |
| 10  | ~6                          | 18.4                      | 18.4             | Near instant convergence |

*Iterations to Convergence = approximate iteration count when BestPathCost first reached 18.40 and remained stable. Shorter iteration counts indicate faster lock‑in to the optimal path, while higher counts reflect slower or unstable convergence.

![ACO Convergence Speed by Run](ConvergenceSpeed.png)

Together, the empirical tables and charts demonstrate that evaluating ACO requires looking beyond final solutions to the full convergence trajectory, where parameter choices determine whether colonies find the optimum quickly, slowly, or unpredictably. Because each run was logged in CSV format and visualized, the experiments are reproducible and parameter effects can be compared directly.


## Scalability Analysis

The scalability of Ant Colony Optimization (ACO) reflects how runtime, memory usage, and convergence behavior change as the problem size grows. Theoretical bounds show that each ant requires up to $O(n^2)$ work per iteration on a graph with $n$ nodes, and pheromone storage also scales as $O(n^2)$. With $k$ ants over $t$ iterations, the total runtime grows as $O(t \cdot k \cdot n^2)$, while memory remains dominated by the pheromone matrix. This quadratic growth in both runtime and space means that ACO is practical for medium‑sized graphs but becomes increasingly costly for very large instances.[4][5]


Scalability Results:

| Nodes (GRAPH_SIZE)  | Runtime (s) | Memory (MB) | BestPathCost | NormBestCost |
|---------------------|-------------|-------------|--------------|--------------|
| 50                  | 0.121       | 0.06        | 18.40        | 0.3414       |
| 100                 | 0.249       | 0.23        | 73.40        | 0.6740       |
| 200                 | 0.670       | 0.92        | 183.40       | 0.8378       |
| 400                 | 2.541       | 3.66        | 403.40       | 0.9191       |
| 800                 | 9.802       | 14.65       | 843.40       | 0.9596       |
| 1000                | 16.019      | 22.89       | 1063.40      | 0.9677       |

All experiments used the same fixed parameters so the only variable was graph size. The colony had 50 ants running for 100 iterations. Pheromone influence ($\alpha$) was set to 1.0, heuristic influence ($\beta$) to 3.0, and evaporation rate ($\rho$) to 0.5. Each ant deposited a fixed amount of pheromone (10.0), and a small exploration probability (0.05) allowed occasional random moves. Keeping these values constant ensured the results show pure scalability effects.

![Runtime(s) vs Nodes](runtimeVSnodes.png)

Runtime vs Nodes: Runtime increases roughly quadratically with graph size, consistent with the $O(n^2)$ bound. The curve shows that small graphs are solved quickly, but runtime grows steeply beyond 400 nodes, reaching over 16 seconds at 1000 nodes.

![Memomory (MB) vs Nodes](memoryVSnodes.png)

Memory vs Nodes: Memory usage also scales quadratically, reflecting the adjacency‑matrix pheromone storage. Even at 1000 nodes, memory remains under 25 MB, suggesting that runtime rather than memory is the limiting factor for scalability in this implementation.

![NormBestCost vs Nodes](normbestcostVSnodes.png)

Normalized Best Cost vs Nodes: As graph size increases, NormBestCost values rise toward 1.0. This means that while the colony still finds efficient paths, the relative improvement over the trivial chain path becomes smaller in larger graphs. For example, at 50 nodes the normalized cost is 0.34 (a strong improvement), but by 1000 nodes it reaches 0.97, showing that the colony’s solution is only slightly better than the baseline.


## Theoretical Analysis
When we study Ant Colony Optimization (ACO), we want to know how fast it runs, how much memory it uses, and why it eventually finds good solutions.
For runtime, let $𝑛$ be the number of nodes, $𝑚$ the number of ants, and $𝑇$ the number of iterations. Each ant builds a path of up to $𝑛$ steps, and at each step it considers up to $𝑛$ neighbors. That means one ant does about $𝑂(𝑛^2)$ work in a single iteration. With $𝑚$ ants, the cost per iteration is $𝑂(𝑚\cdot𝑛^2)$ and across $𝑇$ iterations the total runtime is $𝑂(𝑇\cdot𝑚\cdot𝑛^2)$. For space, storing the graph with pheromone values takes $𝑂(𝑛^2)$, and storing each ant’s path takes $𝑂(𝑚\cdot𝑛)$. So overall space is $𝑂(𝑛^2+𝑚\cdot𝑛)$. Correctness comes from the way pheromone reinforcement works. If a path has cost $𝐿_𝑘$ the pheromone added:
$$\Delta \tau_{ij} = \frac{Q}{L_k}$$
so shorter paths get more pheromone. Over time, this makes them more likely to be chosen, and the probability of selecting the best path increases with each iteration until the colony converges.[1][2]

```
Pseudocode:

for iteration = 1 to T: # repeat search process T times
    for each ant in colony: # each ant explores independently
        path = construct_path() # build path using pheromone + heuristic
        cost = compute_cost(path) # evaluate path quality
        if cost < best_cost: # track best solution so far
            best_path = path
    update_pheromones(best_path) # reinforce good edges (Δτ_ij = Q / L_best)
    evaporate_pheromones() # decay all edges (τ_ij ← (1 - ρ) τ_ij)
```
Correctness can be understood by looking at how the algorithm behaves over multiple iterations. At the very beginning, all paths have the same amount of pheromone, so ants choose randomly. As iterations go on, ants add more pheromone to shorter paths, which makes those paths more attractive. During each iteration, each ant deposits pheromone in proportion to $\frac{Q}{L_k}$, which means shorter paths get stronger reinforcement. Among all ants, the best path overall (with cost $𝐿_best$) receives the largest reinforcement. Evaporation reduces pheromone across all edges, but because the best path keeps getting reinforced, it maintains its advantage. Over time, ants become more likely to follow the best path. As this reinforcement continues, the colony gradually converges toward a good solution.

The pheromone levels are updated using two complementary rules: deposit and evaporation. The deposit rule strengthens the edges of paths that ants discover. The amount of pheromone added is inversely proportional to the path’s cost, so shorter paths receive stronger reinforcement. This is expressed mathematically as: 
$$\Delta \tau_{ij} = \frac{Q}{L_{\text{best}}}$$
where $𝑄$ is a constant and $𝐿_best$ is the cost of the chosen path. This appears in the `deposit_pheromones()` function, where the program calculates `deposit = colony->deposit_amount / L` and adds that value to each edge in the path. This ensures that paths with lower cost accumulate more pheromone, making them more likely to be selected in future iterations.[1][2]
The evaporation rule works in the opposite direction by gradually reducing pheromone levels across all edges. This prevents older or less effective paths from dominating indefinitely and encourages continued exploration. The formula is:
$$\tau_{ij}(t+1) = (1 - \rho) \cdot \tau_{ij}(t)$$
where $𝜌$ is the evaporation rate. This is handled in the `evaporate_pheromones()` function, which multiplies each pheromone value by `(1.0 - colony->evaporation_rate)`. This ensures that pheromone trails decay over time.[1][2]

Together, these two rules balance reinforcement and decay: deposit makes good paths stronger, while evaporation keeps the search space open. This combination allows the colony to steadily converge toward quality solutions without getting stuck too early on suboptimal paths.[5]


## Application

In practice, ACO has been applied across many domains. In computer science and telecommunications, it is used for adaptive network routing, where dynamic traffic conditions require flexible path selection to minimize congestion and latency. In operations research, ACO has proven effective for the Traveling Salesman Problem and vehicle routing, helping logistics companies reduce delivery times and costs. Manufacturing and project management benefit from ACO’s scheduling capabilities, where tasks must be allocated across machines or workers in ways that balance efficiency and resource use. Engineering fields employ ACO for design optimization, while water resource management has leveraged it to improve reservoir operations and groundwater allocation. More recently, healthcare applications have emerged, such as optimizing treatment schedules and hospital resource distribution.[3]

ACO is particularly useful in these areas because it is robust, scalable, and adaptable to dynamic environments. By balancing exploration and exploitation, it escapes local optima and converges on strong global solutions. Its ability to model distributed decision‑making makes it a natural fit for real‑world problems where traditional deterministic algorithms struggle with complexity and uncertainty. [3]


## Implementation

The Ant Colony Optimization (ACO) algorithm was implemented in C. Standard libraries such as `<stdio.h>` and `<stdlib.h>` were used for basic input/output and memory management. In addition, simple custom data structures were created to keep track of the graph connections and the pheromone levels. A challenge was ensuring that path construction respected the maximum path length without exceeding allocated buffers. Defensive programming techniques were applied to prevent overflow errors, with explicit boundary checks and invalid path handling.

Path construction was handled through a loop that walks the graph step by step. Each ant terminates either upon reaching the target node or when no valid move exists, while strict bounds on path length prevent buffer overflows. The inclusion of a visited array prevents cycles, and the `pick_next_node` function introduces a small probability of random exploration to balance exploitation with exploration.

```
// Walk through the graph until reaching the end or hitting max steps
for (int steps = 0; steps < max_steps; steps++) {
    if (current == end) break; // stop if target reached
    int next = pick_next_node(g, current, previous, visited, num_nodes, colony);
    if (next == -1) {
        *path_length = 0;
        free(visited);
        return;
    }
    // Add the chosen node to the path
    path[idx++] = next;
    visited[next] = 1;
    previous = current;
    current = next;
}
```
This loop shows how ants explore the graph step by step, stopping when they reach the target or get stuck.[1][2][5]

Once path construction was stable, the next challenge was implementing reinforcement and decay through pheromone updates. During testing, edges with very low weights sometimes gathered pheromone too quickly, which made the colony collapse onto a single shortcut. To fix this, a small amount of randomness was added so ants didn’t all follow the same path too early, and the update rules were adjusted to keep shortcuts strong without overwhelming longer detours. The deposit rule was implemented as a separate function, ensuring that pheromone amounts were inversely proportional to path cost and clamped within reasonable bounds:

```
// Deposit pheromones along the path taken by an ant.
static void deposit_pheromones(AntGraph* g, int* path, int path_length, AntColony* colony) {
    // Compute the total cost (length) of the path
    double L = compute_path_cost(g, path, path_length);
    // Amount of pheromone to deposit is inversely proportional to path cost
    double deposit = colony->deposit_amount / L;
    // Walk through each edge in the path
    for (int i = 0; i < path_length - 1; i++) {
        int u = path[i]; // current node
        int v = path[i + 1]; // next node
        // Add pheromone to both directions of the edge
        g->edges[u][v].pheromone += deposit;
        g->edges[v][u].pheromone += deposit;
        // Clamp pheromone values to stay within reasonable bounds
        if (g->edges[u][v].pheromone < 0.01) g->edges[u][v].pheromone = 0.01; // minimum floor
        if (g->edges[u][v].pheromone > 10.0) g->edges[u][v].pheromone = 10.0; // maximum cap
    }
}
```
This reflects the reinforcement rule from the theoretical analysis, where shorter paths receive stronger pheromone deposits and become more attractive in future iterations.[1][2][5]

Complementing this, the evaporation rule was implemented to gradually reduce pheromone levels across all edges, preventing older or less effective paths from dominating indefinitely:

```
// Evaporate pheromones on all edges in the graph.
static void evaporate_pheromones(AntGraph* g, AntColony* colony) {
    for (int i = 0; i < g->num_nodes; i++) {
        // Evaporate pheromone on each edge connected to node i
        for (int j = 0; j < g->num_nodes; j++) {
            // Only evaporate if the edge exists
            if (g->edges[i][j].exists) {
                g->edges[i][j].pheromone *= (1.0 - colony->evaporation_rate);
                if (g->edges[i][j].pheromone < 0.01) g->edges[i][j].pheromone = 0.01;
            }
        }
    }
}

```
This corresponds to the evaporation rule, which prevents older trails from dominating and keeps exploration active.[1][2][5]

Together, these functions formed the backbone of the ACO cycle. Ants construct paths probabilistically based on pheromone and heuristic information, pheromone trails are updated after each iteration, and evaporation ensures weaker paths fade over time. Separate functions for path construction, pheromone updates, and logging made it straightforward to experiment with different parameter settings and observe their effects.

## Summary
- Provide a summary of your findings
- What did you learn?



## References

1. GeeksforGeeks. 2025. Introduction to Ant Colony Optimization. (July 12, 2025). Retrieved November 18, 2025 from https://www.geeksforgeeks.org/machine-learning/introduction-to-ant-colony-optimization/

2. Wikipedia. 2025. Ant colony optimization algorithms. (October 1, 2025).  Retrieved November 18, 2025 from https://en.wikipedia.org/wiki/Ant_colony_optimization_algorithms

3. Springer Nature Link. 2024. Applications of Ant Colony Optimization and its Variants. Retrieved December 2, 2025 from https://link.springer.com/book/10.1007/978-981-99-7227-2

4. Dorigo, M., Birattari, M., & Stützle, T. 2006. Ant Colony Optimization: Artificial Ants as a Computational Intelligence Technique. Université Libre de Bruxelles. Retrieved December 2, 2025 from https://www.cs.princeton.edu/courses/archive/fall24/cos597C/bib2/Ant_colony_optimization.pdf

5. Dorigo, M., & Stützle, T. 2018. Ant Colony Optimization: Overview and Recent Advances. In Handbook of Metaheuristics (pp. 311–351). Springer. Retrieved December 2, 2025 from https://iridia.ulb.ac.be/~mdorigo/Published_papers/2018/DorStu2018MetaHandBook.pdf

6. Abdelmoaty, A. M., & Ibrahim, I. I. 2024. Comparative Analysis of Four Prominent Ant Colony Optimization Variants: Ant System, Rank-Based Ant System, Max-Min Ant System, and Ant Colony System. Retrieved December 2, 2025 from https://arxiv.org/pdf/2405.15397