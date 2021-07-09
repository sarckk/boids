# Boids 

Boids simulation in C++. Made with [SFML](https://github.com/SFML/SFML) and [Imgui-SFML](https://github.com/eliasdaler/imgui-sfml).

Uses a [k-d tree](https://en.wikipedia.org/wiki/K-d_tree) for an efficient radial neighbor search, allowing it to render many more boids on the screen than a naive search algorithm would afford.

### Features
- Perching
- External Force generation 
- Obstacles avoidance
- Tunable parameters
- Predator behavior 
- Variable Mass

### Build Instructions 
This project uses `CMake`. Assuming you are in the top-level directory of this project, run the following command to build this project:
```bash
cmake . && cmake --build . && ./boids
```

The project uses `CMake`'s `FetchContents` for dependency management, so you will not have to download and build SFML and Imgui-SFML locally for this to work. 
Since `FetchContents` downloads the dependencies from the github repo at `configure` time, this means that the initial `cmake ../` will take some time the first
time you run it or when dependencies change. Consider using [ccache](https://github.com/ccache/ccache) to speed up this process with caching. 

For more information about how `FetchContents` was used, see [here](https://eliasdaler.github.io/using-cmake/).

### References 
- https://www.red3d.com/cwr/boids/
- http://www.kfish.org/boids/pseudocode.html
- https://eliasdaler.github.io/using-cmake/
