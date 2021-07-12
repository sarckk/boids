# Boids

<p align="center">
  <img src="https://raw.githubusercontent.com/sarckk/boids/master/images/screenshot.png" alt="boids_screenshot" />
</p>


Boids simulation in C++. Made with [SFML](https://github.com/SFML/SFML) and [Imgui-SFML](https://github.com/eliasdaler/imgui-sfml).

This implementation uses [spatial hashing]() for an efficient radius
query, allowing it to render many more boids on the screen
than a naive search algorithm (O(N^2)) would afford.

I also considered tree-based techniques (think k-d trees and Quadtrees),
but ended up using spatial hashing for a few reasons:

1. Tree-based techniques are not optimal for when objects on which the queries
   are being run is constantly moving as in boids simulations. This is because rebuilding
   the tree is expensive, and thus most implementations of boids using such techniques
   usually end up rebuilding the entire tree again. In comparison, the spatial hashing method
   initialises the hash grid once, and only updates the boids' membership in each cell if it has moved (which,
   in our case, is not frequent because each boid moves very little during 1/60 seconds per frame).
   Constant-time updates thus give spatial hashing the upper hand here.
2. Quadtrees were not chosen because it can be tricky to find the optimal value for the max. no. of boids
   that can go into one leaf node. If we set it to 1 (default), this can be problematic when the distance between
   any two boids is miniscule, in which case the Quadtree will continue to recursively partition the space
   until we are left with leaf nodes that represent veeery small areas.
3. K-d trees were not chosen because they are not well-suited for dynamic insertions, as it is costly to
   rebalance the tree to ensure efficient queries. This was a problem because this simulation supports dynamically
   changing the number of boids in the program.
4. Simplicity. Spatial hashing is simpler to implement, and sometimes simplicity is the key.

That said, there were still some challenges I encountered along the way. For one, in an ideal scenario,
there should be an additional logic in the boids velocity update algorithm to make boids avoid overly-populated
grid cells, but I didnt' have the time to implement this.

Then there was the question about radius queries. Usually with fixed grid methods, you instantiate each cell size
to be equal to the query radius _r_, but in my case, since I wanted to allow users to dynamically change the perception
radius of the boids, this wasn't possible (unless I rebuilt the whole grid from scratch on each update).
I thus implemented the solution proposed in Nicolas Brodu's 2007 paper [Query Sphere Indexing for Neighborhood Requests](https://nicolas.brodu.net/common/recherche/publications/QuerySphereIndexing.pdf)
and adapted it for 2D grids. 

### Build Instructions

First, execute `build.sh` from the top-level directory of this project:

```bash
./build.sh
```

This will create a `build` directory with the executable inside. Then simply run:

```bash
./build/boids
```

You don't have to install SFML, ImGui or ImGui-SFML on your local machine
before you do this, because this project uses `CMake`'s `FetchContents`
to manage dependencies. As a result, the first time you execute the script
it might take a while since it will download all the dependencies
if they don't exist on your machine.

### References

-   https://www.red3d.com/cwr/boids/
-   https://www.youtube.com/watch?v=mhjuuHl6qHM&ab_channel=TheCodingTrain
-   http://www.kfish.org/boids/pseudocode.html
-   https://nicolas.brodu.net/common/recherche/publications/QuerySphereIndexing.pdf
-   https://conkerjo.wordpress.com/2009/06/13/spatial-hashing-implementation-for-fast-2d-collisions/
