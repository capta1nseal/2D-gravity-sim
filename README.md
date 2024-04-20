# 2D-gravity-sim
2D gravity simulation made with c++, sdl2 and openGL.

## licence
GNU GPL 3.0. in root of repo and online.

### project status
Flexible two-dimensional gravity simulation with very customisable features.

The simulation is based on real physical properties, and by default will model the solar system.

The system will generally be stable even with relatively large time steps, but is most stable and accurate at smaller time steps.

The simulation's size is technically the entire range of the double precision floating-point datatype, and off-screen particles are all simulated.

#### installation
Clone the repository, make sure you have SDL2, glew and glu on your include path. (usually in apt as ´´´libX-dev´´´, where X is the library name)

Build the project with ´´´make build -j n´´´ (where n is thread count. Build is fast even on one thread). The binary is generated to ´´´bin/gravitysim´´´.

Other make rules:
- ´´´make clean´´´ deletes all binaries and dependencies, occasionally useful if you think your changes are not being detected.
- ´´´make test -j n´´´ builds the project with n threads, then immediately executes the binary.

##### Usage
Run the binary, and the program will automatically run in fullscreen. You can exit with ´´´Esc´´´, or ´´´Alt + F4´´´ (or the equivalent for your system).

Controls are as follows:
- ´´´Arrow keys´´´ for moving the camera.
- ´´´PG UP / PG DN´´´ for zooming in/out. Will zoom further when held.
- ´´´HOME / END´´´ for increasing or decreasing the time scale of the simulation. (WARNING: VERY UNSTABLE AT HIGH SPEED)
- ´´´LClick´´´ for adding a particle at the mouse.
- ´´´RClick´´´ for removing the nearest particle to the mouse.
- ´´´Esc´´´ to close the program.
- ´´´F´´´ to start following particle nearest to mouse, or stop following currently followed particle.
