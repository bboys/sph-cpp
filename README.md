# sph-cpp
A Smoothed Particle Hydrodynamics implementation in C++

## Running the code
Compile with make and run the ./ray executable from the build directory

## Optional arguments:
    -p: stiffness
    -s: surface tension
    -u: dynamic viscosity
    -r: particle radius
    -f: smoothing factor
    -m: particle mass
    -d: density
    -n: number of particles
    -q: particle spawning rate

## Hotkeys:
    r: Reset camera
    o: Toggle OpenGL mode (non-raytraced spheres)
    d: Toggle deferred shading. Deferred shading is needed for the surface splatting to work correctly. Disabled by default
    n: Toggle normals. Shows the normals as a colour
    w: Toggle water. Uses surface splatting to draw the surface. Should be used in combination with deferred shading
    y: Toggle 2D mode. This removes the z-component from the simulation
    g: Toggle gravity. If enabled, the gravity will rotate along with the cube
    f: Toggle FPS dependent timestep. This should make the simulation time independent of the framerate.
    t: Change the timestep
    b: Take a screenshot
    q: Quit
    c: Toggle contours. Not very useful in this context
    s: Toggle shadows. Not very useful in this context

## Movement:
    Click and drag: Rotate
    Shift+Click and drag: Move
    Ctrl+Click and drag: Zoom
