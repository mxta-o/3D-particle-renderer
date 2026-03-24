cmake -S . -B build -DCMAKE_POLICY_VERSION_MINIMUM=3.5
cmake --build build --config Release
./bin/particle_renderer

to run cmake window