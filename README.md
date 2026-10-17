# Solar System Simulation
A 2D N-body gravity simulation in C++ with raylib.

Every body pulls on every other body, and their motion is integrated with velocity Verlet, using AU, solar masses and years as units.
It starts with the Sun, Mercury, Venus and Earth on circular orbits.
You can add your own bodies with the mouse: pick a preset or set a name, mass, radius and colour in the side panel, and a line shows the path the body will follow while you aim.
When two bodies collide, both are removed.

## Building

raylib 5.5 and raygui 4.0 are downloaded during the first build, so you need an internet connection.

### Ubuntu (with sudo)

raylib is compiled from source, so first install a compiler, CMake and the libraries it needs:

```sh
sudo apt install build-essential cmake git libasound2-dev libx11-dev libxrandr-dev libxi-dev libgl1-mesa-dev libglu1-mesa-dev libxcursor-dev libxinerama-dev libwayland-dev libxkbcommon-dev
cmake -B build
cmake --build build
./build/solar_sim
```

### Linux without admin rights

If you have `g++` (version 10 or newer) and `git` but no CMake or sudo (for example on university machines), use the build script.
It downloads a prebuilt raylib and raygui into `~/deps` (set `DEPS_DIR` to use a different folder) and compiles with g++ directly.

```sh
./build_lab.sh
./solar_sim
```

### Windows (Visual Studio)

You need Visual Studio with the "Desktop development with C++" workload (it includes CMake) and [Git](https://git-scm.com/).
From a Developer PowerShell for Visual Studio, in the project folder:

```powershell
cmake -B build
cmake --build build --config Release
.\build\Release\solar_sim.exe
```

## Controls

- **Left-drag:** place a body and aim it. It's launched in the direction you drag, faster the further you drag.
- **Right-click:** cancel placing.
- **Space:** pause or resume.
