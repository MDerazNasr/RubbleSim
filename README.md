# RubbleSim

> A planned real-time C++ sandbox for robot–debris interaction, contact debugging,
> deterministic replay, performance profiling, and synthetic sensor-map export.

[![Status: specification](https://img.shields.io/badge/status-specification-orange)](#project-status)
[![Language: C++20](https://img.shields.io/badge/C%2B%2B-20-blue)](#planned-technology)
[![Renderer: OpenGL](https://img.shields.io/badge/renderer-OpenGL-5586A4)](#planned-technology)

## Project status

RubbleSim is currently in the **specification and repository setup phase**. The
simulation executable, build system, screenshots, benchmarks, and exports
described below are planned deliverables and are not yet present in this
repository. The source of truth for the intended scope is
[RubbleSim Project Scope.pdf](./RubbleSim%20Project%20Scope.pdf).

## Demo

> Demo GIF coming after the first end-to-end scenario is implemented.

The target demo is a 60–90 second sequence: clear a blocked doorway, enable the
contact overlay, inspect an unstable stack collapse, replay the same input
deterministically, and preview profiling and sensor outputs.

## Overview

RubbleSim is designed as a compact but serious simulation-infrastructure
project. A keyboard- or script-controlled pusher moves through unstable piles
of boxes, planks, pipes, and rubble proxies. The engine exposes what is
happening through collision graphs, spatial-hash cells, occupancy maps,
timings, replay files, and per-frame exports.

The goal is not to replace a production physics engine. It is to demonstrate
the systems surrounding an understandable, deterministic interaction model.

## Why robot–debris simulation?

Robots in disaster sites, warehouses, and industrial spaces encounter clutter
that shifts, jams, settles, and blocks passages. A useful simulator must make
those interactions reproducible and inspectable—not merely render them.
RubbleSim focuses on the engine facilities needed to investigate such scenes.

### Design principles

- **Inspectable:** expose contacts, bounds, velocities, and broadphase cells.
- **Repeatable:** use a fixed timestep, scenario seed, and recorded inputs.
- **Measurable:** report simulation, collision, render, and export timings.
- **Focused:** favor simple, explicit physics over a feature-complete engine.
- **Robotics-oriented:** export maps and state that downstream tools can use.

## Planned highlights

### Simulation

- Lightweight rigid-object state and deterministic fixed-step integration
- AABB collision with a spatial-hash broadphase
- Push/impulse response, damping, friction approximation, and sleeping
- Contact records linking object pairs to positions, normals, and strength
- Controllable blade, box pusher, wedge, or simple rover body

### Engine tooling

- OpenGL scene, debug, top-down occupancy, and synthetic sensor views
- Deterministic recording, playback, pause, and timeline scrubbing
- CPU timers plus FPS and p50/p95 frame-time summaries
- RGB, depth, object-ID, occupancy, contact-heatmap, pose, and contact exports
- Reproducible scenario configs for demos and fixed-size benchmarks

## Planned technology

| Area | Choice |
| --- | --- |
| Language | C++20 (C++17-compatible design where practical) |
| Build | CMake |
| Window/input | GLFW |
| Graphics | OpenGL + GLAD + GLM |
| Debug UI | Dear ImGui |
| Assets/exports | tinyobjloader and stb libraries, as needed |
| Data | JSON metadata/replay and CSV benchmarks |
| Quality | clang-format and focused unit tests |

## Architecture

The planned design separates deterministic simulation state from presentation,
recording, profiling, and export. This keeps headless benchmarks possible and
prevents rendering behavior from changing simulation results.

~~~text
                       controls / replay
                              |
                              v
scenario config ---> simulation world ---> contact graph
                         |        |               |
                         |        v               v
                         |   spatial hash      debug draw
                         v
                    frame snapshot
                    /      |       \
                   v       v        v
              renderer  profiler  exporters
~~~

### Frame pipeline

1. Load a scenario seed, object states, and simulation settings.
2. Sample live controls or the next recorded input.
3. Advance the world by one fixed timestep.
4. Build broadphase candidates, resolve contacts, and update sleeping state.
5. Freeze a frame snapshot for rendering, telemetry, replay, and export.
6. Present the selected view without feeding view state back into simulation.

## Planned scenarios

Each scenario is seeded and config-driven so interactive runs, replays, and
benchmarks begin from the same state.

### 1. Blocked Doorway

A blade pushes rubble away from a passage. The demo compares occupancy before
and after the interaction and reports the percentage of path cells cleared.

### 2. Unstable Stack Collapse

The pusher disturbs a stacked pile and triggers a chain of contacts. Trajectory
trails, the contact graph, and settling time make the collapse inspectable.

### 3. Narrow Path Clearing

The robot moves through corridor clutter that may shift, jam, or clear. A
top-down grid shows how free space changes. A pipe-and-plank jam is a possible
fourth stress scenario once the three core scenes work.

## Interaction model

Exact key bindings will be documented after the application skeleton lands.
The planned controls cover pusher translation, camera orbit/pan/zoom,
pause/single-step, replay scrubbing, scenario reset, view switching, debug
toggles, and selected-frame export.

### Views

- **Main:** shaded arena, debris, pusher, floor grid, and barriers
- **Contact debug:** IDs, bounds, contact points/normals, and velocity arrows
- **Top-down occupancy:** blocked, free, robot, and cleared-path cells
- **Sensor:** depth, object-ID, or contact output from a fixed camera

### Debug overlays

Debug drawing is a first-class output rather than a late visualization pass.
The overlay should reveal active collision pairs, occupied spatial-hash cells,
contact strength, object motion, camera frusta, and live timing counters.

### Runtime lifecycle

Interactive control, scripted control, and replayed control all produce the
same input-command representation. Reset reconstructs the world from its
scenario seed; pause and single-step operate on simulation ticks rather than
render frames.

## Simulation system

The simulation is intentionally lightweight. It should model enough motion and
contact behavior to create credible clearing, jamming, collapse, and settling
without claiming the completeness or accuracy of a production rigid-body
solver.

<!-- README-CONTINUE -->
