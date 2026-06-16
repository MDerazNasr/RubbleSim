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

<!-- README-CONTINUE -->
