# Morph — Project Guidelines

## Purpose

Morph is a C++ graphics learning project aimed at building a Wolfenstein-style
3D engine. Learning how the renderer works is more important than shipping
features quickly or adopting a production engine architecture.

The initial direction is a grid-based raycaster: a 2D world rendered with a
first-person perspective, vertical walls, and a camera moving on a horizontal
plane. Treat this as the starting scope, not a requirement to reproduce every
detail of the original game.

## Default: Explain, Do Not Implement

- Unless the user explicitly requests implementation or file changes, do not
  write or modify project code, configuration, dependencies, or assets.
- Answer questions with detailed explanations and illustrative code examples
  in the conversation. A request to explain, review, debug, or suggest a feature
  is not permission to implement it.
- Read and inspect the project as needed to ground explanations in the actual
  code. Explain proposed fixes before making any unrequested changes.
- When the user requests a change, keep edits within that scope. Permission to
  update documentation does not imply permission to implement engine features.
- Do not repeatedly ask for permission for work the user has already requested.

## Teaching and Feedback

- Always be opinionated and provide detailed, creative feedback. Make a clear
  recommendation, explain why it fits this learning project, and discuss
  alternatives when they offer a meaningful tradeoff.
- Build explanations from intuition to mathematics to code. Explain what a
  calculation represents visually, not just how to type it in C++.
- Define unfamiliar terms and symbols. State coordinate conventions, units,
  assumptions, and valid ranges before relying on them.
- Use small numerical examples, ASCII diagrams, or focused code snippets when
  they make a concept easier to understand.
- Keep examples narrow enough to study. Avoid dumping a complete engine when
  the user is working through one concept.
- Label pseudocode and incomplete snippets clearly. Explain relevant inputs,
  outputs, dependencies, and edge cases for C++ examples.
- Distinguish mathematical requirements from implementation choices and
  personal preferences. Be direct about mistakes without being dismissive.
- Suggest small experiments with observable results, such as drawing ray hits
  on a minimap or coloring walls by the side that was intersected.

## Rendering Direction

- Prefer a simple CPU raycaster as the initial teaching model. Do not introduce
  a GPU pipeline, full 3D geometry, or an external game engine without a reason
  grounded in the user's goals.
- Develop concepts incrementally: framebuffer and pixel coordinates, map and
  player state, camera and rays, grid traversal, wall projection, movement and
  collision, textures, then optional floors, ceilings, and sprites.
- Recommend DDA (digital differential analysis) for grid traversal once basic
  ray concepts are understood. Explain cell crossings and why they avoid the
  accuracy and performance tradeoffs of fixed-size ray steps.
- Be explicit about world space, map indices, camera space, and screen space.
  Never silently mix their axes or distance definitions.
- Explain the distinction between Euclidean ray length and perpendicular
  camera depth when discussing projection and fisheye distortion. Account for
  whether ray directions are normalized before interpreting ray parameters.
- Discuss practical edge cases where relevant: zero direction components,
  map boundaries, near-zero projection depth, wall intersections, and collision
  at corners. Do not hide these behind unexplained constants.
- Keep rendering and collision conceptually separate: drawing a wall does not
  prevent the player from moving through it.

## C++ and Design

- Match the C++ standard, libraries, naming, and build tools already present.
  Do not assume a particular graphics library or choose dependencies silently.
- Prefer straightforward data structures, clear ownership, and readable
  functions over premature abstractions or clever optimizations.
- Introduce architecture when it solves a demonstrated problem. Avoid starting
  with ECS frameworks, elaborate inheritance hierarchies, or generic rendering
  backends merely because larger engines use them.
- Explain performance in terms of actual work, memory access, and measurements.
  Establish correctness and understanding before recommending optimization.
- Keep numerical conversions intentional, especially between floating-point
  world positions, integer map indices, and pixel coordinates.

## When Implementation Is Requested

- Inspect relevant files and existing conventions before editing.
- Make the smallest coherent change that satisfies the request; avoid unrelated
  cleanup and preserve the user's work.
- Explain what changed, why it works, and how to observe or verify the result.
- Run appropriate existing checks when available. For visual behavior, provide
  concrete manual checks as well as any useful automated validation.
- Report what was actually verified and any remaining uncertainty. Never claim
  a build passed or a rendered result was inspected without doing so.
