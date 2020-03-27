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


## Screenshot of the Simulation

![Screenshot](https://raw.githubusercontent.com/0x0f0f0f/infectious-disease-toy-simulation/master/screenshot.png)

The graph shows every 5 frames the size of these partitions of the total population.
Default simulation time is 40 seconds and is easily tweakable. Other easily adjustable values
are window width and height, probability of dying if infected, probability of infecting another
individual upon contact, the time needed to heal or die, the population size,
the fraction of people that are moving and the graph sampling rate.
The simulation outputs CSV data, plotted with gnuplot.
Thanks to [ix](https://github.com/ix) for very useful improvements to the simulation.

In the examples, population size is 1100:

## 10/1100 people are quarantined

![10/1100 quarantined](https://raw.githubusercontent.com/0x0f0f0f/infectious-disease-toy-simulation/master/examples/10.png)

## 110/1100 people are quarantined

![110/1100 quarantined](https://raw.githubusercontent.com/0x0f0f0f/infectious-disease-toy-simulation/master/examples/110.png)

## 210/1100 people are quarantined

![210/1100 quarantined](https://raw.githubusercontent.com/0x0f0f0f/infectious-disease-toy-simulation/master/examples/210.png)

## 310/1100 people are quarantined

![310/1100 quarantined](https://raw.githubusercontent.com/0x0f0f0f/infectious-disease-toy-simulation/master/examples/310.png)

## 410/1100 people are quarantined

![410/1100 quarantined](https://raw.githubusercontent.com/0x0f0f0f/infectious-disease-toy-simulation/master/examples/410.png)

## 510/1100 people are quarantined

![510/1100 quarantined](https://raw.githubusercontent.com/0x0f0f0f/infectious-disease-toy-simulation/master/examples/510.png)

## 610/1100 people are quarantined

![610/1100 quarantined](https://raw.githubusercontent.com/0x0f0f0f/infectious-disease-toy-simulation/master/examples/610.png)

## 710/1100 people are quarantined

![710/1100 quarantined](https://raw.githubusercontent.com/0x0f0f0f/infectious-disease-toy-simulation/master/examples/710.png)
