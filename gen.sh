# genereate a pdf first
# modify the order numbers
folder="ds-gen6"
path="../$folder"
rm -rf $path
rm "../$folder.zip"
mkdir -p $path
git clone "file://$(pwd)" "$path/Stage 1 - BSTNode BSTMap" -b stage1-v0.1 --depth=1
git clone "file://$(pwd)" "$path/Stage 2 - Balanced BST" -b stage2-v0.3 --depth=1
git clone "file://$(pwd)" "$path/Stage 3 - Iterator" --depth=1 # -b stage3-v0.2
cp References.md README.md README.pdf test-crun.sh $path
chmod +x $path/test-crun.sh
cd $path
./test-crun.sh && zip -r "../$folder.zip" *
