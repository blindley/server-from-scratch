if (-not (Test-Path ./build/windows/CMakeCache.txt)) {
    cmake -S . -B ./build/windows
}

cmake --build ./build/windows
