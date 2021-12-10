# 259 Final Project

## Team
Yiqun Li, Chengxin Wang, Rui Lin

## Setup
Our program is run on the Merlin AWS instance. After you start an instance, please follow the following commands to set up the environment.
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
#git clone ...
````

## Usage
```bash
cd bfs
mkdir build
cd build
# move graph.txt into "build" directory
cmake ..
make swsim # software simulation
make hls # hardware simulation
```
Before we initiate any simulation, please make sure that the input filename is exactly "graph.txt" and it is located in the "build" directory. For the graph of LiveJournal, please remove all comment lines at the beginning.

For software simulation, a new file "answer.txt" will be generated, and it contains all vertices with their depth. For example, "2:1" means vertex 2 has depth of 1. When you have the answer.txt, you can use the Java program to verify the correctness of the output. Make sure you also have a copy of "graph.txt" in the same directory as "answer.txt". Also, based on different graphs you verify, you may need to change the input of split() function. For facebook graph, it should be a single whitespace; for LiveJournal graph, it should be a single tab.

