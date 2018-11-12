set -e
sudo losetup /dev/loop3 $2
sudo mount /dev/loop3 $1
