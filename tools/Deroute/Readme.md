# Deroute

Tool for untangling wires.

![JPEG](/tools/Deroute/Build/sample.jpg)

It can be used for reverse engineering of integrated circuits and printed circuit boards.

## Features

At the center of the tool is a custom control called the EntityBox. Its task is to store and display a set of "Entities".

Entities are any connected elements that are used to build the topology of integrated circuits and printed circuit boards: wires, connecting contacts (vias), standard cells, register and memory blocks, other ICs and connectors.

## Source image layers

3 layers of images are supported.

Each layer has its own scrolling and zoom, independent of the scrolling and zoom of the entire scene.

Transparency can be assigned to each layer. Layers are drawn in the order: 2 -> 1 -> 0.

## Coordinate system

Deroute operates in two coordinate systems:
- To store vector data, raster-independent Lambda coordinates are used. 1 Lambda = n pixels (set in settings)
- To display vector data, Lambda coordinates are converted to screen coordinates, taking into account scrolling and zoom.

## Output

The collection of entities can be saved and loaded in XML format.
