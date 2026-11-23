# Y-Render

Y-Render is a C++20 starter project for a PC/game performance diagnostics and resource-management toolkit. This archive is a clean foundation created from scratch; it is **not** a finished universal game optimizer.

## Build locally

Requirements: CMake 3.20+ and a C++20 compiler (Visual Studio 2022, GCC 11+, or Clang 14+ recommended).

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
ctest --test-dir build -C Release --output-on-failure
```

Run:
- Windows: `build/Release/yrender.exe`
- Linux: `./build/yrender`

## Automatic compilation

The workflow `.github/workflows/build.yml` builds Windows x64, Windows x86 and Linux x64 on every push or pull request, runs the tests, and uploads the executable artifacts. In GitHub, open **Actions**, select **Y-Render Build**, and download the artifact from a successful run.

## Included modules

| Module | Current status |
|---|---|
| Y-Render Core | Working console entry point and CMake library |
| Y-Hardware Intelligence | Detects OS, architecture, logical threads and RAM where supported |
| Y-Memory Cache | Working thread-safe LRU cache for data explicitly passed to it |
| Y-Predict | Frequency-based resource observation heuristic |
| Y-Async Loading | Asynchronous local file loading |
| Y-Bottleneck Detector | Basic interpretation of manually supplied telemetry |
| Y-Evolution Engine | Stores samples and selects highest recorded FPS profile |
| Y-Texture Optimizer | File metadata analyzer only; no texture conversion |
| Y-Audio Optimizer | File metadata analyzer only; no audio conversion |
| Y-Service Manager | Safety policy only; does not change Windows services |
| AUTO-CONFIG | Planned; no system settings are automatically changed |
| SUPER OPTIMIZER / Legacy | Planned; no unsupported performance promises |
| Y-Game Launcher | Planned; does not launch arbitrary executables yet |
| GUI | Planned; this starter uses a console interface |

## Important technical limits

- A normal desktop application cannot automatically intercept every game's asset requests, GPU commands, shader compilation, or memory allocations. Those features need engine-specific plugins, supported graphics APIs, or game integrations.
- GPU model, VRAM, temperatures and utilization are not yet queried. Add platform/API-specific adapters (for example DXGI on Windows or Vulkan/OpenGL integrations where supported).
- The predictor learns from events explicitly supplied to it. It does not inspect another process.
- The media analyzers inspect metadata only. They never rewrite game assets.
- The service manager is read-only. It never disables security or operating-system services.
- The included workflow verifies that the code compiles; it does not certify performance gains.

## Repository layout

```text
Y-Render/
├── .github/workflows/build.yml
├── include/yrender/
├── src/
├── tests/
├── CMakeLists.txt
└── README.md
```

## Next development stages

1. Add a real telemetry collector with explicit user consent.
2. Add Windows GPU/VRAM detection using DXGI.
3. Add a GUI (for example Qt or a native Windows UI) after selecting the supported platforms.
4. Implement engine-specific archive and texture adapters with backups and integrity checks.
5. Benchmark repeatable scenes before and after each reversible optimization.
