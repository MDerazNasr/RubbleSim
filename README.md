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

### Object state

Every simulated object carries an ID, shape type, transform, linear velocity,
mass, friction, restitution, static/sleeping flags, and bounding volume.
Orientation and angular velocity can be added where they materially improve a
scenario. The robot tool is represented as a kinematic collision participant.

### Collision pipeline

A spatial hash assigns bounds to grid cells and generates candidate pairs.
AABB tests provide the required narrowphase baseline; OBB and better
cylinder/capsule approximations remain extensions. Per tick, the profiler
records occupied cells, candidates, actual contacts, and broadphase time.

### Contact and settling

The baseline solver estimates a contact point and normal, applies a simple
push/impulse response, then damps motion using friction and settling
heuristics. Objects below motion thresholds may sleep. Each resolved pair is
also emitted to the contact graph with relative velocity and a strength proxy.

## Deterministic replay

A recording stores the scenario seed, initial object states, fixed timestep,
simulation configuration, and robot input sequence. Playback rebuilds that
state and applies commands at the original ticks. The viewer should support
play/pause, single-step, timeline scrubbing, camera changes, and frame export.

## Profiling and benchmarks

Per-frame telemetry covers total, simulation, broadphase, contact, render, and
export time alongside active objects, candidate/contact pairs, dirty objects,
and draw calls. Fixed benchmark scenes target 50, 100, 250, and—if
practical—500 objects. Reports include averages plus p50, p95, and maximum
frame time.

## Synthetic sensor and map export

Selected frames may produce RGB, depth, object-ID, occupancy, and contact
heatmap images together with pose and contact JSON. The occupancy grid labels
free, occupied, robot, and cleared-path cells; unknown cells are optional.
Exports are inspection artifacts, not a claim of photorealistic sensor
simulation.

### Planned output layout

~~~text
output/scenario_0001/
├── metadata.json
├── replay.json
├── benchmark.csv
├── frames/
│   ├── rgb/
│   ├── depth/
│   ├── object_id/
│   ├── occupancy/
│   └── contact_heatmap/
├── poses/
├── contacts/
└── videos/
~~~

## Build and run

> The commands in this section describe the planned interface. They will not
> work until the CMake application and scenario configs are implemented.

Expected prerequisites are a C++20 compiler, CMake 3.20 or newer, OpenGL
drivers, and GLFW development files. Third-party dependency handling is still
to be decided.

### Configure

~~~bash
git clone <repository-url>
cd RubbleSim
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
~~~

For development builds, use `-DCMAKE_BUILD_TYPE=Debug` and enable the project’s
test option once it is introduced.

### Build and test

~~~bash
cmake --build build --parallel
ctest --test-dir build --output-on-failure
~~~

A successful build is expected to produce a `rubblesim` executable and focused
collision, spatial-hash, and replay-determinism tests.

### Launch

~~~bash
./build/rubblesim --scenario configs/blocked_doorway.json
~~~

The default interactive launch should open the main view with the debug panel
available and preserve deterministic simulation ticks independently of render
rate.

### Run scenarios

~~~bash
./build/rubblesim --scenario configs/blocked_doorway.json
./build/rubblesim --scenario configs/stack_collapse.json
./build/rubblesim --scenario configs/narrow_path.json
~~~

Scenario files are planned, not currently included.

### Run a benchmark

~~~bash
./build/rubblesim --benchmark configs/benchmark_250.json \
  --csv output/benchmarks/benchmark_250.csv
~~~

Benchmark runs should accept an explicit tick count and avoid UI-dependent
timing where possible.

### Record, replay, and export

~~~bash
./build/rubblesim --scenario configs/blocked_doorway.json \
  --record output/replays/doorway.json
./build/rubblesim --replay output/replays/doorway.json
./build/rubblesim --replay output/replays/doorway.json --export-frame 240
~~~

### CLI contract

The final flag names may change during implementation. Whichever interface is
chosen should make the seed, fixed timestep, scenario, tick limit, replay path,
benchmark output, headless mode, and export selection explicit so runs can be
reproduced from a command line alone.

## Configuration and data contracts

A scenario config should define its name and seed, fixed timestep, arena
geometry, robot tool, object list or generation rules, gravity/damping,
collision grid size, camera presets, and enabled outputs. Configuration values
that affect simulation must be copied into replay metadata.

### Scenario metadata

| Field | Purpose |
| --- | --- |
| `scenario_name`, `scenario_seed` | Identify a reproducible scene |
| `object_count`, `object_types` | Summarize scene contents |
| `robot_tool_type` | Identify the controlled collider |
| `simulation_dt`, `frame_count` | Define the recorded timeline |
| `camera_params` | Reconstruct exported views |
| `benchmark_enabled` | Distinguish measured runs |
| `exported_outputs` | Inventory generated artifacts |

### Replay and contact records

Replay JSON contains initial object states, the robot input sequence, scenario
seed, and simulation configuration. A contact record identifies the frame and
object pair plus contact position, normal, relative velocity, and a contact
strength proxy. Object IDs remain stable for the entire run.

### Benchmark CSV

One row per measured frame should include total, simulation, broadphase,
contact, and render milliseconds; active object, candidate-pair, contact-pair,
dirty-object, and draw-call counts; plus the scenario, seed, tick, build type,
and relevant hardware labels needed to interpret results.

## Evaluation

RubbleSim is evaluated on understandable simulation behavior, real-time
performance, debug visibility, replay determinism, export usefulness, and code
organization—not on ML accuracy or fidelity to a production physics engine.
All tables remain blank until measured by an implemented build.

### Performance results

| Objects | Avg FPS | Frame p50 | Frame p95 | Sim ms | Broadphase ms | Render ms |
| ---: | ---: | ---: | ---: | ---: | ---: | ---: |
| 50 | — | — | — | — | — | — |
| 100 | — | — | — | — | — | — |
| 250 | — | — | — | — | — | — |
| 500 | — | — | — | — | — | — |

### Scenario results

| Scenario | Objects | Contact pairs | Settling time | Path cleared |
| --- | ---: | ---: | ---: | ---: |
| Blocked Doorway | — | — | — | — |
| Stack Collapse | — | — | — | n/a |
| Narrow Path | — | — | — | — |
| Pipe/Plank Jam | — | — | — | — |

### Export results

| Output | Resolution/grid | Export time | File size |
| --- | --- | ---: | ---: |
| RGB | — | — | — |
| Depth | — | — | — |
| Object ID | — | — | — |
| Occupancy | — | — | — |
| Contact heatmap | — | — | — |
| Pose JSON | n/a | — | — |
| Replay JSON | n/a | — | — |

## Testing strategy

Focused tests should cover AABB overlap and separation, spatial-hash cell
assignment and pair deduplication, integration at a fixed timestep, sleeping
thresholds, scenario parsing, output schemas, and replay determinism. Small
headless fixtures are preferred over rendering-dependent tests.

### Planned test commands

~~~bash
ctest --test-dir build --output-on-failure
./build/tests/test_aabb
./build/tests/test_spatial_hash
./build/tests/test_replay_determinism
~~~

### Determinism check

A replay test should run the same seed and command stream twice, serialize a
canonical state snapshot at chosen ticks, and compare object transforms,
velocities, sleeping flags, and contact pairs. Divergence must report the first
mismatching tick and object rather than only a final checksum.

### Quality gates

Before merging an implementation change: format touched C++ files, build in
Debug and Release modes, run the headless test suite, and execute a short fixed
benchmark. Continuous integration may automate these checks after the project
skeleton and dependency strategy stabilize.

## Planned repository layout

~~~text
RubbleSim/
├── CMakeLists.txt
├── configs/             # scenarios and fixed benchmarks
├── assets/              # lightweight meshes and textures
├── include/rubblesim/   # public C++ headers
├── src/                 # app, sim, collision, physics, rendering,
│                        # replay, profiling, export, and UI modules
├── shaders/
├── scripts/
├── docs/
├── tests/
└── output/              # generated artifacts; normally gitignored
~~~

## Roadmap

### Milestone 1 — Engine skeleton and scene

- Configure CMake and dependencies.
- Open a GLFW/OpenGL window with camera controls and ImGui.
- Render the arena and primitive debris.
- Load a basic config and move a simple pusher.

### Milestone 2 — Collision and contacts

- Implement object state, AABBs, and the spatial-hash broadphase.
- Add simple response, damping, settling, and sleeping.
- Draw bounds, contact points, normals, grid cells, and pair counts.
- Log stable contact records for inspection and tests.

### Milestones 3–4 — Scenarios, replay, profiling, and export

- Build the three required interaction scenarios.
- Record seeds and input commands; add deterministic playback and scrubbing.
- Introduce fixed benchmark scenes and timing summaries.
- Export maps, masks, poses, contacts, and selected replay frames.
- Replace placeholders here with real media, commands, and measured results.

## Limitations

- There is currently no executable implementation or build configuration.
- Physics is planned as a simplified approximation, not a validated solver.
- V1 excludes deformables, fluids, fracture, SLAM, planning, ROS 2, and ML.
- Photorealistic rendering and calibrated physical sensors are out of scope.
- Performance and scenario tables intentionally contain no invented numbers.

## Future improvements

After the baseline is measured and documented, useful extensions include OBB
and capsule collision, angular response, instanced rendering, GPU picking, a
BVH broadphase, a ROS 2 bridge, comparison against Isaac Sim, and a path
planner consuming exported occupancy maps.

<!-- README-CONTINUE -->
