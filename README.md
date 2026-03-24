# 3D Particle Renderer

An intermediate C++ project for learning **OpenGL** and **CUDA** through the lens of real-time 3D particle simulation and rendering.

This is a personal learning journey — a hands-on way to build intuition for graphics programming, GPU computing, and modern C++ while shipping something visually satisfying.

---

## Table of Contents

- [Project Goals](#project-goals)
- [What to Know Before Starting](#what-to-know-before-starting)
- [What to Prepare](#what-to-prepare)
- [Suggested Architecture](#suggested-architecture)
- [Suggested File & Folder Structure](#suggested-file--folder-structure)
- [Development Roadmap](#development-roadmap)
- [Key Concepts to Explore Along the Way](#key-concepts-to-explore-along-the-way)
- [Learning Resources](#learning-resources)

---

## Project Goals

- Practice modern **C++ (C++17/20)** — memory management, move semantics, templates, and clean OOP design.
- Get a working understanding of the **OpenGL rendering pipeline** (vertex/fragment shaders, VAOs/VBOs, framebuffers).
- Introduce **CUDA** for GPU-accelerated particle physics — forces, collisions, and integration.
- Build something you can see: an interactive 3D particle system rendered in real time.

---

## What to Know Before Starting

These are the areas worth brushing up on before diving in:

### C++
- Pointers, references, and memory management (`new`/`delete`, RAII, smart pointers)
- Classes, inheritance, and virtual functions
- Templates and the STL (`std::vector`, `std::array`, `std::unordered_map`)
- Build systems: **CMake** basics (you'll be using it to tie OpenGL, CUDA, and your code together)

### Mathematics
- **Linear algebra**: vectors, matrices, dot/cross products
- **Transformations**: model/view/projection (MVP) matrix pipeline
- **Basic calculus** for physics: integration (Euler or Verlet) to advance particle positions over time
- Coordinate systems: world space, camera/view space, clip space

### Graphics Fundamentals (conceptual is fine at first)
- What the GPU pipeline does: vertices → rasterization → fragment shading → framebuffer
- The role of shaders (GLSL): vertex shader positions geometry, fragment shader decides pixel color
- What a texture, a framebuffer, and a VAO/VBO actually are

### GPU Computing (conceptual)
- The difference between CPU and GPU threading models
- What a CUDA kernel is and why launching thousands of threads is cheap on the GPU

---

## What to Prepare

### Hardware & Drivers
- An **NVIDIA GPU** (CUDA requires NVIDIA hardware)
- Up-to-date **NVIDIA drivers** and the **CUDA Toolkit** installed
- OpenGL 3.3+ support (almost any GPU from the last decade qualifies)

### Software & Toolchain
| Tool | Purpose |
|------|---------|
| **CMake ≥ 3.18** | Build system |
| **CUDA Toolkit** | GPU compute (`nvcc` compiler) |
| **GLFW** | Window creation and input handling |
| **GLAD** | OpenGL function loader (generate for your target version at [glad.dav1d.de](https://glad.dav1d.de)) |
| **GLM** | GLSL-style math (vectors, matrices) for CPU-side code |
| **A C++17/20-capable compiler** | GCC 10+, Clang 10+, or MSVC 2019+ (C++17 minimum; C++20 optional for later phases) |

### Recommended IDE / Editor
- **VS Code** with the C/C++ and CMake Tools extensions, or
- **CLion** (has first-class CMake and CUDA support)

---

## Suggested Architecture

The system splits naturally into three layers:

```
┌──────────────────────────────────────────────────────┐
│                   Application Layer                   │
│  Window, input handling, main loop, camera control   │
└───────────────────────┬──────────────────────────────┘
                        │
          ┌─────────────┴─────────────┐
          │                           │
┌─────────▼──────────┐   ┌───────────▼───────────────┐
│  Simulation Layer  │   │      Rendering Layer        │
│  (CUDA kernels)    │   │  (OpenGL shaders & draw     │
│                    │   │   calls)                    │
│  • Particle update │   │  • VAO/VBO management       │
│  • Force calc      │   │  • Shader compilation       │
│  • Collision detect│   │  • Camera / MVP uniforms    │
└─────────┬──────────┘   └───────────┬───────────────┘
          │                          │
          └───────────┬──────────────┘
                      │
          ┌───────────▼───────────┐
          │   Particle Data Store  │
          │  (GPU-resident buffer) │
          │  CUDA/OpenGL interop   │
          └───────────────────────┘
```

**CUDA ↔ OpenGL Interop** is the interesting seam: you register an OpenGL buffer object (VBO) with CUDA so the simulation kernel can write positions directly into the buffer that OpenGL will render — no CPU round-trip.

---

## Suggested File & Folder Structure

```
3D-particle-renderer/
├── CMakeLists.txt              # Top-level build definition
├── README.md
├── .gitignore
│
├── src/
│   ├── main.cpp                # Entry point: init, main loop, teardown
│   │
│   ├── app/
│   │   ├── Application.h/.cpp  # Owns window, renderer, simulation; runs the loop
│   │   └── Camera.h/.cpp       # View matrix, zoom, orbit controls
│   │
│   ├── render/
│   │   ├── Renderer.h/.cpp     # OpenGL state, draw calls
│   │   ├── Shader.h/.cpp       # Compile/link GLSL shaders, set uniforms
│   │   ├── ParticleBuffer.h/.cpp # VAO/VBO setup, CUDA interop registration
│   │   └── shaders/
│   │       ├── particle.vert   # Vertex shader (position → gl_Position, point size)
│   │       └── particle.frag   # Fragment shader (color, soft circle SDF)
│   │
│   ├── sim/
│   │   ├── Simulation.h/.cpp   # Manages CUDA resources, launches kernels
│   │   ├── Particle.h          # Plain struct: position, velocity, color, life
│   │   └── kernels/
│   │       ├── update.cu       # Per-particle integration kernel
│   │       ├── forces.cu       # Gravity, attractors, turbulence
│   │       └── collisions.cu   # (Later) spatial hashing, collision response
│   │
│   └── util/
│       ├── Timer.h/.cpp        # Delta-time calculation
│       └── Random.h            # Fast GPU/CPU random utilities
│
├── extern/                     # Third-party dependencies (git submodules or fetched by CMake)
│   ├── glad/
│   ├── glfw/
│   └── glm/
│
└── assets/                     # Textures, config files (optional)
```

---

## Development Roadmap

Work through the phases in order — each one builds on the last and gives you something you can see and interact with.

### Phase 1 — Foundation (CPU only, no CUDA yet)
- [ ] Set up CMake project linking GLFW and GLAD
- [ ] Open a window and clear it to a solid color
- [ ] Compile and run a "Hello Triangle" — your first VAO/VBO/shader pipeline
- [ ] Add a `Camera` class with keyboard/mouse orbit controls
- [ ] Draw a single point sprite as a placeholder particle

### Phase 2 — CPU Particle System
- [ ] Define a `Particle` struct (position, velocity, color, lifetime)
- [ ] Allocate a `std::vector<Particle>` and update it on the CPU each frame
- [ ] Upload positions to a VBO with `glBufferSubData` and render with `GL_POINTS`
- [ ] Write a fragment shader that draws soft circles (use `gl_PointCoord` + SDF)
- [ ] Add basic emitter: spawn particles from a point with random velocity
- [ ] Add lifetime and fade-out

### Phase 3 — Introduce CUDA
- [ ] Add CUDA to the CMakeLists and confirm `nvcc` compiles a trivial kernel
- [ ] Allocate particle data on the GPU with `cudaMalloc`
- [ ] Write an `update` kernel that advances position by velocity × Δt (Euler integration)
- [ ] Register the OpenGL VBO with CUDA (`cudaGraphicsGLRegisterBuffer`) so the kernel writes directly into the VBO — no CPU copy needed
- [ ] Profile with **Nsight Systems** or `nvprof` to understand GPU utilization

### Phase 4 — Physics
- [ ] Add gravity and a global attractor force in the CUDA kernel
- [ ] Try **Verlet integration** for better energy conservation compared to Euler
- [ ] Implement turbulence using a noise function on the GPU
- [ ] Add basic bounding-box collisions (reflect velocity at walls)

### Phase 5 — Visual Polish
- [ ] Particle color gradients over lifetime (lerp between two colors in the fragment shader)
- [ ] Additive blending (`glBlendFunc(GL_SRC_ALPHA, GL_ONE)`) for a glowing effect
- [ ] Billboard sprites: render a textured quad per particle instead of a point
- [ ] Bloom post-processing effect using a framebuffer and a Gaussian blur pass

### Phase 6 — Scale & Performance
- [ ] Benchmark with 100 K, 1 M, and 10 M particles; identify bottlenecks
- [ ] Implement spatial hashing on the GPU for particle–particle collision detection
- [ ] Explore CUDA shared memory and warp-level optimizations

### Phase 7 — Features & Extensions (choose your own adventure)
- [ ] Multiple emitter types: point, sphere surface, mesh surface
- [ ] Inter-particle forces (attraction/repulsion) — classic N-body simulation
- [ ] Export frames to PNG for offline rendering (use `stb_image_write`)
- [ ] ImGui UI for tweaking parameters at runtime

---

## Key Concepts to Explore Along the Way

| Concept | Why It Matters |
|---------|---------------|
| RAII & smart pointers | Safely manage GPU/CPU resources without leaks |
| OpenGL state machine | Understanding why order of GL calls matters |
| GLSL data types (`vec3`, `mat4`, `uniform`) | Bridge between CPU and GPU code |
| CUDA thread hierarchy (thread → block → grid) | Mapping work to thousands of parallel threads |
| CUDA/OpenGL interop (`cudaGraphicsResource`) | Avoiding expensive CPU↔GPU copies |
| Delta-time & fixed vs. variable timestep | Stable, frame-rate-independent simulation |
| Depth testing & blending | Correct transparency for overlapping particles |
| Profiling with Nsight | Finding real bottlenecks instead of guessing |

---

## Learning Resources

### OpenGL
- [learnopengl.com](https://learnopengl.com) — the single best free resource; work through it in order
- *OpenGL Programming Guide* ("The Red Book") — comprehensive reference
- [docs.gl](https://docs.gl) — fast function reference with version annotations

### CUDA
- [CUDA C++ Programming Guide](https://docs.nvidia.com/cuda/cuda-c-programming-guide/) — the authoritative reference
- [CUDA by Example](https://developer.nvidia.com/cuda-example) — gentle introductory book (free PDF from NVIDIA)
- NVIDIA's [Nsight Systems](https://developer.nvidia.com/nsight-systems) — free GPU profiler

### C++ & Math
- [cppreference.com](https://en.cppreference.com) — definitive C++ reference
- *3D Math Primer for Graphics and Game Development* (Dunn & Parberry) — very readable linear algebra focused on graphics
- [GLM documentation](https://glm.g-truc.net/0.9.9/index.html) — mirrors GLSL math in C++

### Particle Systems & Graphics Theory
- Robert Bridson's *Fluid Simulation for Computer Graphics* — for advanced physics
- [Physically Based Rendering (pbr-book.org)](https://www.pbr-book.org) — deeper rendering theory for later
- GPU Gems (free online via NVIDIA): chapters on particle systems and GPU physics
