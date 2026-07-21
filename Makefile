CFLAGS=-march=native -std=c++23 -Wall -Wextra -Wshadow -pedantic
BENCH=-isystem abseil-cpp -isystem benchmark/include -Lbenchmark/build/src -lbenchmark -lpthread
CMAKE_OPT=-DBENCHMARK_DOWNLOAD_DEPENDENCIES=on -DCMAKE_BUILD_TYPE=Release

MACHINE=$(shell lscpu | grep -o -P "(?<=Model name:).*" | sed -E 's/\s+//; s/\(\w+\)//g; s/\s/_/g')

HEADERS=fast_B-trees/include/dynamic_search.hpp fast_B-trees/include/internal.hpp \
	    fast_B-trees/include/static_search.hpp abseil-cpp/absl/container/btree_set.h \
		op_templates.hpp

TEST_BINS=bench_std_set_int bench_std_set_double bench_std_set_dec bench_std_flat_set_int \
 		  bench_std_flat_set_double bench_std_flat_set_dec bench_absl_btree_set_int \
		  bench_absl_btree_set_double bench_absl_btree_set_dec bench_bt_dynamic_set_int \
		  bench_bt_dynamic_set_double bench_bt_dynamic_set_dec bench_bt_static_set_int \
		  bench_bt_static_set_double bench_bt_static_set_dec profile_std_set_int \
		  profile_std_set_double profile_std_set_dec profile_std_flat_set_int \
		  profile_std_flat_set_double profile_std_flat_set_dec profile_absl_btree_set_int \
		  profile_absl_btree_set_double profile_absl_btree_set_dec profile_bt_dynamic_set_int \
		  profile_bt_dynamic_set_double profile_bt_dynamic_set_dec profile_bt_static_set_int \
		  profile_bt_static_set_double profile_bt_static_set_dec

.phony: run

.phony: all

bench: bench.cpp benchmark/build/src/libbenchmark.a $(HEADERS)
	g++ $(CFLAGS) -DTEST_ALL -DNDEBUG -Ofast bench.cpp $(BENCH) -o bench

bench_std_set_int: bench.cpp benchmark/build/src/libbenchmark.a $(HEADERS)
	g++ $(CFLAGS) -DTEST_STD_SET_INT -DNDEBUG -Ofast bench.cpp $(BENCH) -o bench_std_set_int

bench_std_set_double: bench.cpp benchmark/build/src/libbenchmark.a $(HEADERS)
	g++ $(CFLAGS) -DTEST_STD_SET_DOUBLE -DNDEBUG -Ofast bench.cpp $(BENCH) -o bench_std_set_double

bench_std_set_dec: bench.cpp benchmark/build/src/libbenchmark.a $(HEADERS)
	g++ $(CFLAGS) -DTEST_STD_SET_DEC -DNDEBUG -Ofast bench.cpp $(BENCH) -o bench_std_set_dec

bench_std_flat_set_int: bench.cpp benchmark/build/src/libbenchmark.a $(HEADERS)
	g++ $(CFLAGS) -DTEST_STD_FLAT_SET_INT -DNDEBUG -Ofast bench.cpp $(BENCH) -o bench_std_flat_set_int

bench_std_flat_set_double: bench.cpp benchmark/build/src/libbenchmark.a $(HEADERS)
	g++ $(CFLAGS) -DTEST_STD_FLAT_SET_DOUBLE -DNDEBUG -Ofast bench.cpp $(BENCH) -o bench_std_flat_set_double

bench_std_flat_set_dec: bench.cpp benchmark/build/src/libbenchmark.a $(HEADERS)
	g++ $(CFLAGS) -DTEST_STD_FLAT_SET_DEC -DNDEBUG -Ofast bench.cpp $(BENCH) -o bench_std_flat_set_dec

bench_absl_btree_set_int: bench.cpp benchmark/build/src/libbenchmark.a $(HEADERS)
	g++ $(CFLAGS) -DTEST_ABSL_BTREE_SET_INT -DNDEBUG -Ofast bench.cpp $(BENCH) -o bench_absl_btree_set_int

bench_absl_btree_set_double: bench.cpp benchmark/build/src/libbenchmark.a $(HEADERS)
	g++ $(CFLAGS) -DTEST_ABSL_BTREE_SET_DOUBLE -DNDEBUG -Ofast bench.cpp $(BENCH) -o bench_absl_btree_set_double

bench_absl_btree_set_dec: bench.cpp benchmark/build/src/libbenchmark.a $(HEADERS)
	g++ $(CFLAGS) -DTEST_ABSL_BTREE_SET_DEC -DNDEBUG -Ofast bench.cpp $(BENCH) -o bench_absl_btree_set_dec

bench_bt_dynamic_set_int: bench.cpp benchmark/build/src/libbenchmark.a $(HEADERS)
	g++ $(CFLAGS) -DTEST_BT_DYNAMIC_SET_INT -DNDEBUG -Ofast bench.cpp $(BENCH) -o bench_bt_dynamic_set_int

bench_bt_dynamic_set_double: bench.cpp benchmark/build/src/libbenchmark.a $(HEADERS)
	g++ $(CFLAGS) -DTEST_BT_DYNAMIC_SET_DOUBLE -DNDEBUG -Ofast bench.cpp $(BENCH) -o bench_bt_dynamic_set_double

bench_bt_dynamic_set_dec: bench.cpp benchmark/build/src/libbenchmark.a $(HEADERS)
	g++ $(CFLAGS) -DTEST_BT_DYNAMIC_SET_DEC -DNDEBUG -Ofast bench.cpp $(BENCH) -o bench_bt_dynamic_set_dec

bench_bt_static_set_int: bench.cpp benchmark/build/src/libbenchmark.a $(HEADERS)
	g++ $(CFLAGS) -DTEST_BT_STATIC_SET_INT -DNDEBUG -Ofast bench.cpp $(BENCH) -o bench_bt_static_set_int

bench_bt_static_set_double: bench.cpp benchmark/build/src/libbenchmark.a $(HEADERS)
	g++ $(CFLAGS) -DTEST_BT_STATIC_SET_DOUBLE -DNDEBUG -Ofast bench.cpp $(BENCH) -o bench_bt_static_set_double

bench_bt_static_set_dec: bench.cpp benchmark/build/src/libbenchmark.a $(HEADERS)
	g++ $(CFLAGS) -DTEST_BT_STATIC_SET_DEC -DNDEBUG -Ofast bench.cpp $(BENCH) -o bench_bt_static_set_dec

profile: profile.cpp counters/counters.hpp $(HEADERS)
	g++ $(CFLAGS) -DTEST_ALL -DNDEBUG -Ofast profile.cpp -isystem abseil-cpp -o profile

profile_std_set_int: profile.cpp counters/counters.hpp $(HEADERS)
	g++ $(CFLAGS) -DTEST_STD_SET_INT -DNDEBUG -Ofast profile.cpp -isystem abseil-cpp -o profile_std_set_int

profile_std_set_double: profile.cpp counters/counters.hpp $(HEADERS)
	g++ $(CFLAGS) -DTEST_STD_SET_DOUBLE -DNDEBUG -Ofast profile.cpp -isystem abseil-cpp -o profile_std_set_double

profile_std_set_dec: profile.cpp counters/counters.hpp $(HEADERS)
	g++ $(CFLAGS) -DTEST_STD_SET_DEC -DNDEBUG -Ofast profile.cpp -isystem abseil-cpp -o profile_std_set_dec

profile_std_flat_set_int: profile.cpp counters/counters.hpp $(HEADERS)
	g++ $(CFLAGS) -DTEST_STD_FLAT_SET_INT -DNDEBUG -Ofast profile.cpp -isystem abseil-cpp -o profile_std_flat_set_int

profile_std_flat_set_double: profile.cpp counters/counters.hpp $(HEADERS)
	g++ $(CFLAGS) -DTEST_STD_FLAT_SET_DOUBLE -DNDEBUG -Ofast profile.cpp -isystem abseil-cpp -o profile_std_flat_set_double

profile_std_flat_set_dec: profile.cpp counters/counters.hpp $(HEADERS)
	g++ $(CFLAGS) -DTEST_STD_FLAT_SET_DEC -DNDEBUG -Ofast profile.cpp -isystem abseil-cpp -o profile_std_flat_set_dec

profile_absl_btree_set_int: profile.cpp counters/counters.hpp $(HEADERS)
	g++ $(CFLAGS) -DTEST_ABSL_BTREE_SET_INT -DNDEBUG -Ofast profile.cpp -isystem abseil-cpp -o profile_absl_btree_set_int

profile_absl_btree_set_double: profile.cpp counters/counters.hpp $(HEADERS)
	g++ $(CFLAGS) -DTEST_ABSL_BTREE_SET_DOUBLE -DNDEBUG -Ofast profile.cpp -isystem abseil-cpp -o profile_absl_btree_set_double

profile_absl_btree_set_dec: profile.cpp counters/counters.hpp $(HEADERS)
	g++ $(CFLAGS) -DTEST_ABSL_BTREE_SET_DEC -DNDEBUG -Ofast profile.cpp -isystem abseil-cpp -o profile_absl_btree_set_dec

profile_bt_dynamic_set_int: profile.cpp counters/counters.hpp $(HEADERS)
	g++ $(CFLAGS) -DTEST_BT_DYNAMIC_SET_INT -DNDEBUG -Ofast profile.cpp -isystem abseil-cpp -o profile_bt_dynamic_set_int

profile_bt_dynamic_set_double: profile.cpp counters/counters.hpp $(HEADERS)
	g++ $(CFLAGS) -DTEST_BT_DYNAMIC_SET_DOUBLE -DNDEBUG -Ofast profile.cpp -isystem abseil-cpp -o profile_bt_dynamic_set_double

profile_bt_dynamic_set_dec: profile.cpp counters/counters.hpp $(HEADERS)
	g++ $(CFLAGS) -DTEST_BT_DYNAMIC_SET_DEC -DNDEBUG -Ofast profile.cpp -isystem abseil-cpp -o profile_bt_dynamic_set_dec

profile_bt_static_set_int: profile.cpp counters/counters.hpp $(HEADERS)
	g++ $(CFLAGS) -DTEST_BT_STATIC_SET_INT -DNDEBUG -Ofast profile.cpp -isystem abseil-cpp -o profile_bt_static_set_int

profile_bt_static_set_double: profile.cpp counters/counters.hpp $(HEADERS)
	g++ $(CFLAGS) -DTEST_BT_STATIC_SET_DOUBLE -DNDEBUG -Ofast profile.cpp -isystem abseil-cpp -o profile_bt_static_set_double

profile_bt_static_set_dec: profile.cpp counters/counters.hpp $(HEADERS)
	g++ $(CFLAGS) -DTEST_BT_STATIC_SET_DEC -DNDEBUG -Ofast profile.cpp -isystem abseil-cpp -o profile_bt_static_set_dec

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

all: $(TEST_BINS)

run: all
	./bench_std_set_int | tee $(MACHINE).res
	./bench_std_set_double | tee -a $(MACHINE).res
	./bench_std_set_dec | tee -a $(MACHINE).res
	./bench_std_flat_set_int | tee -a $(MACHINE).res
	./bench_std_flat_set_double | tee -a $(MACHINE).res
	./bench_std_flat_set_dec | tee -a $(MACHINE).res
	./bench_absl_btree_set_int | tee -a $(MACHINE).res
	./bench_absl_btree_set_double | tee -a $(MACHINE).res
	./bench_absl_btree_set_dec | tee -a $(MACHINE).res
	./bench_bt_dynamic_set_int | tee -a $(MACHINE).res
	./bench_bt_dynamic_set_double | tee -a $(MACHINE).res
	./bench_bt_dynamic_set_dec | tee -a $(MACHINE).res
	./bench_bt_static_set_int | tee -a $(MACHINE).res
	./bench_bt_static_set_double | tee -a $(MACHINE).res
	./bench_bt_static_set_dec | tee -a $(MACHINE).res
	
	./profile_std_set_int | tee $(MACHINE).prof
	./profile_std_set_double | tee -a $(MACHINE).prof
	./profile_std_set_dec | tee -a $(MACHINE).prof
	./profile_std_flat_set_int | tee -a $(MACHINE).prof
	./profile_std_flat_set_double | tee -a $(MACHINE).prof
	./profile_std_flat_set_dec | tee -a $(MACHINE).prof
	./profile_absl_btree_set_int | tee -a $(MACHINE).prof
	./profile_absl_btree_set_double | tee -a $(MACHINE).prof
	./profile_absl_btree_set_dec | tee -a $(MACHINE).prof
	./profile_bt_dynamic_set_int | tee -a $(MACHINE).prof
	./profile_bt_dynamic_set_double | tee -a $(MACHINE).prof
	./profile_bt_dynamic_set_dec | tee -a $(MACHINE).prof
	./profile_bt_static_set_int | tee -a $(MACHINE).prof
	./profile_bt_static_set_double | tee -a $(MACHINE).prof
	./profile_bt_static_set_dec | tee -a $(MACHINE).prof