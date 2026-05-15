CFLAGS=-march=native -std=c++2a -Wall -Wextra -Wshadow -pedantic
BENCH=-isystem benchmark/include -Lbenchmark/build/src -lbenchmark -lpthread
CMAKE_OPT=-DBENCHMARK_DOWNLOAD_DEPENDENCIES=on -DCMAKE_BUILD_TYPE=Release

HEADERS=fast_B-trees/include/dynamic_search.hpp fast_B-trees/include/internal.hpp \
	    fast_B-trees/include/static_search.hpp

bench: bench.cpp benchmark/build/lib/libgtest.a $(HEADERS)
	g++ $(CFLAGS) -DNDEBUG -Ofast bench.cpp $(BENCH) -o bench

benchmark/include:
	git submodule update --init

fast_B-trees/include:
	git submodule update --init

fast_B-trees/include/%.hpp: | fast_B-trees/include

benchmark/build/lib/libgtest.a: | benchmark/include
	mkdir -p benchmark/build
	(cd benchmark; cmake $(CMAKE_OPT) -S . -B "build")
	(cd benchmark; cmake --build "build" --config Release)


