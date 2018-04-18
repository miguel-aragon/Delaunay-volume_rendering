# Delaunay volume rendering

## Description

This is a volume rendering code that produces gorgeous visualizations at interactive rates. Since it is based on a tessellation (in comparison to grid-based methods) there is no resolution limit and one can zoom in all the way to the smallest separation between particles. 

The technique us an approximation to actual volume rendering. It approximates the integrated of the scalar field signal along the line of sight (as in real volume rendering) for a simple triangle rendering of the Delaunay tessellation. So the continuous field is approximated by a series of delta functions (intersection with triangles) scaled with the density field at that location. For visualiation purposes the approximation is indistinguishable from a full volume rendering at a much lower computational cost.

The code is quite long and full of patches/bugs/cheap tricks. This is a research code that I use to do some data exploration visualizing simlated galaxies to find interesting features/events to study further.


Resolution independent, here showing particles
![picture](images/halo-0.png)

and the resulting density field
![picture](images/halo-1.png)


We can compute isodensity contours directly on the Delaunay tessellation.
![picture](images/isoden.png)

Here another view of isodensity contours on top of the density field.

![picture](images/density-isoden.png)


We can also cut a plane across the tessellation

![picture](images/isoplane.png)

and visualize the plane in 2D. Here showing the core of a dark matter halo.

![picture](images/2d-zoom.png)


