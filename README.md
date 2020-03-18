# Toy Infectious Disease Simulation

Inspired by a recent popular news article about the effectivity of containment
measures during outbreaks, in this case the COVID-19 disease caused by the
SARS-COV-2 virus,
I wrote this small animation/simulation in a single C file (using [raylib](https://raylib.com),
you need it to build this)
to show what happens if you avoid moving and social contact during an epidemic.
All the parameters are easily adjustable (yes, you need to recompile if you change them).

**This is not a precise simulation**. This is a toy animation made to show that 
distancing/quarantine measures during outbreaks are good countermeasures 
and delay the rapid spread of infectious disease. 
Spheres in a box that are not bouncing around are simple enough to show this.

## 1/4 of the population is moving

![1/4 of the population is moving](https://raw.githubusercontent.com/0x0f0f0f/infectious-disease-toy-simulation/master/screenshots/4.png)

## 1/3 of the population is moving

![1/3 of the population is moving](https://raw.githubusercontent.com/0x0f0f0f/infectious-disease-toy-simulation/master/screenshots/3.png)

## 1/2 of the population is moving

![1/2 of the population is moving](https://raw.githubusercontent.com/0x0f0f0f/infectious-disease-toy-simulation/master/screenshots/2.png)

## 1/1 of the population is moving

![1/1 of the population is moving](https://raw.githubusercontent.com/0x0f0f0f/infectious-disease-toy-simulation/master/screenshots/1.png)

Colors are the following:
* Gray - Has never come in contact with the disease
* Purple - Is infected
* Green - Has recovered and is immune to the disease.
* Red - Has died

The graph shows every 5 frames the size of these partitions of the total population.
Default simulation time is 40 seconds and is easily tweakable. Other easily adjustable values
are window width and height, probability of dying if infected, probability of infecting another
individual upon contact, the time needed to heal or die, the population size,
the fraction of people that are moving and the graph sampling rate.
