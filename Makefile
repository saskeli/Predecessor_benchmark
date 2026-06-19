CFLAGS=-march=native -std=c++23 -Wall -Wextra -Wshadow -pedantic
BENCH=-isystem abseil-cpp -isystem benchmark/include -Lbenchmark/build/src -lbenchmark -lpthread
CMAKE_OPT=-DBENCHMARK_DOWNLOAD_DEPENDENCIES=on -DCMAKE_BUILD_TYPE=Release

MACHINE=$(shell lscpu | grep -o -P "(?<=Model name:).*" | sed -E 's/\s+//; s/\(\w+\)//g; s/\s/_/g')

HEADERS=fast_B-trees/include/dynamic_search.hpp fast_B-trees/include/internal.hpp \
	    fast_B-trees/include/static_search.hpp abseil-cpp/absl/container/btree_set.h \
		op_templates.hpp

.phony: run

bench: bench.cpp benchmark/build/src/libbenchmark.a $(HEADERS)
	g++ $(CFLAGS) -DNDEBUG -Ofast bench.cpp $(BENCH) -o bench

profile: profile.cpp counters/counters.hpp $(HEADERS)
	g++ $(CFLAGS) -DNDEBUG -Ofast profile.cpp -isystem abseil-cpp -o profile

benchmark/include:
	git submodule update --init

fast_B-trees/include:
	git submodule update --init

fast_B-trees/include/%.hpp: | fast_B-trees/include

benchmark/build/src/libbenchmark.a: | benchmark/include
	mkdir -p benchmark/build
	(cd benchmark; cmake $(CMAKE_OPT) -S . -B "build")
	(cd benchmark/build/src; make)

abseil-cpp/absl/container/btree_set.h:
	git submodule update --init

counters/counters.hpp:
	git submodule update --init

run: bench profile
	./bench | tee $(MACHINE).res
	./profile | tee $(MACHINE).prof