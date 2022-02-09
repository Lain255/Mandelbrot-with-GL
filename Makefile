d:
	ninja -C build/debug -f build.ninja
	./build/debug/mandelbrot

r:
	ninja -C build/release -f build.ninja
	./build/debug/mandelbrot

b:
	cmake -Bbuild/debug -GNinja -DCMAKE_BUILD_TYPE=Debug -DCMAKE_C_COMPILER=gcc .
	cmake -Bbuild/release -GNinja -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_COMPILER=gcc .