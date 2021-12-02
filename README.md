# UCLA CS259 Fall 2021 Final Project

## Team
Yiqun Li, Chengxin Wang, Rui Lin

## Setup
Our program runs on the Merlin AWS instance. After you start an instance, please follow the following commands to set up the environment.
```bash
# set up FPGA
cd $AWS_FPGA_REPO_DIR
source vitis_setup.sh
source /opt/Xilinx/Vitis/2021.1/settings64.sh
# set up TAPA and cmake
sudo yum install epel-release
sudo yum install python-pip
sudo pip install cmake
PATH="${HOME}/.local/bin:${PATH}"
curl -L git.io/JnERa | bash
source <(curl -L bit.ly/3BzVG16)
cmake --version # make sure the version is >= 3.13
# download file
cd $VITIS_DIR/examples/xilinx
git clone https://github.com/yiqli53/CS259-Project.git
````

## Usage
```bash
cd CS259-Project
mkdir build
cd build
# move graph.txt into "build" directory
cmake ..
make swsim # This will show the results of partitioning for now
make hls
```
