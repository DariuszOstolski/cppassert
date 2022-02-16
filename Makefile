.PONY: test, test_release

test:
	cmake ./ -B ./build -DCMAKE_BUILD_TYPE:STRING=Debug -G "Ninja Multi-Config" -DFEATURE_TESTS=ON
	cmake --build ./build --config Debug
	cd build && ctest . --config Debug --verbose

test_release:
	cmake ./ -B ./build -DCMAKE_BUILD_TYPE:STRING=Release -G "Ninja Multi-Config" -DFEATURE_TESTS=ON
	cmake --build ./build --config Release
	cd build && ctest . --config Release --verbose
