buildDir=build
#buildDir=build_debug

#change to Debug for debug mode
buildType=Release
#buildType=Debug

all: build

mkbuilddir:
	test -d $(buildDir) || mkdir -p $(buildDir)

cmake: mkbuilddir CMakeLists.txt
	cd $(buildDir) && cmake -DCMAKE_BUILD_TYPE=$(buildType) ..

build: cmake
	$(MAKE) -C $(buildDir) -std=c++11

clean: mkbuilddir
	$(MAKE) -C $(buildDir) clean

install: mkbuilddir
	$(MAKE) -C $(buildDir) install
	
cleanup_cache: mkbuilddir
	cd $(buildDir) && rm -rf *
	
