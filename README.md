# 2D particles collisions in C using Raylib

Small Raylib projects showing particle collisions.<br>
It contains a small `particle_lib` library for handling particles in Raylib and two small examples involving elastic particle collisions in 2D.

## Brownian particle 
A larger Brownian particle colliding with smaller & lighter particles:<br>
[<img src="assets/brownian.png" width="480">](https://github.com/so-groenen)

By pressing the `SPACE BAR` we can move over to the *Brownian path trace*-view which shows the trajectory of Brownian particle:<br>
[<img src="assets/brownian_trace.png" width="480">](https://github.com/so-groenen)
This is similar as you would see in a stochastic processes in 2D (ex, [Wiener process](https://en.wikipedia.org/wiki/Wiener_process))
and makes connection to the Langevin equation approach to the Brownian motion more clear.
See the [wikipedia page](https://en.wikipedia.org/wiki/Brownian_motion) for more information on the Brownian motion.

## Maxwell-Boltzmann distribution

Shows the Maxwell-Boltzmann distribution in real time from the velocity distrubtion:
[<img src="assets/maxwell_boltzmann.jpg" width="960">](https://github.com/so-groenen)

## Related Projects: Langevin equation & Event Driven Simulation

Instead of considering thousands of "solvent" particle interacting with a single larger Brownian particle, Paul Langevin showed in the 1920s
that the problem can be mapped to the larger particle interacting with a head bath represented by random forces, giving rise to the Langevin equation.<br>
You can find my study using Python Numba here:
* [Langevin equation using Python Numba](https://github.com/so-groenen/Langevin-equation-and-Brownian-motion)

Another popular approach to solving multiple interacting particles is to use the so-callled "Event-driven" approach. Here, instead of checking for collisions at every frame, we pre-compute all the collision events (treated as a class containing the colliding pairs and predicted time of collision), put them in a priority queue (ordred by decreasing collision time) and move from collision to collision. 
* [Event Driven using Python PyGame](https://github.com/so-groenen/Event-Driven-particle-simulation-in-Pygame)

## TODO:
* Add video link, if OBS lets me do it without frame drops
 