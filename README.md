# AI
This is a small project where I tried to implement some metaheuristic algorithms which are used in Artificial Intelligence.

#### Implemented algorithms
- [Particle Swarm Optimiztion (PSO)](https://en.wikipedia.org/wiki/Particle_swarm_optimization)
- [Max-Min Ant System (MMAS)](https://en.wikipedia.org/wiki/Ant_colony_optimization_algorithms)

## Build examples
```
$ mkdir build & cd build
$ cmake .. # or a path to the root CMake file
$ make
```

## TODO
I am considering to make from this project a small but very convinient library that could provide implementation for different metaheuristic algorithms.

**Improvements**
- Speed up MMAS. Poor perfomance on huge graphs.
*Hint*: try to play with *maxAntNoves* value or consider another criteria to skip ant that failed to find a path.

- Make PSO more stable.
- Rework architecture.
- Refactor code, especially random generator.
- Improve overall perfomance.
- Improve build system.

