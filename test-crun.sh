crun() {
    cd "$*"
    mkdir build
    cd build
    cmake ..
    make -j8
    ./main
    cd ..
    rm -rf build
    cd ..
}

crun "Stage 1 - BSTNode BSTMap"
crun "Stage 2 - Balanced BST"
crun "Stage 3 - Iterator"
