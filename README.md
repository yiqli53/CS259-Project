# 259 Final Project

## Team
Yiqun Li, Chengxin Wang, Rui Lin

## Software Simulation
# Setup
Our program is run on the Merlin AWS instance. After you connect to an instance, please follow the following commands to set up the environment.
```bash
# set up FPGA
cd $AWS_FPGA_REPO_DIR
source vitis_setup.sh
source /opt/Xilinx/Vitis/2021.1/settings64.sh

# set up TAPA
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
cd CS259-Project
sudo yum install java
sudo yum install java-devel

# download datasets and pre-process them
wget https://snap.stanford.edu/data/facebook_combined.txt.gz
gzip -d facebook_combined.txt.gz
wget https://snap.stanford.edu/data/soc-LiveJournal1.txt.gz
gzip -d soc-LiveJournal1.txt.gz
javac BFSVerifier.java
javac ProcessFile.java
java ProcessFile
rm soc-LiveJournal1.txt
mv graph.txt soc-LiveJournal1.txt

mkdir build
cd build
cmake ..
cd ..
````

# Usage
```bash
# Make sure you are in "CS259-Project" directory
mv facebook_combined.txt graph.txt # or "soc-LiveJournal1.txt" if LiveJournal
cd build
make swsim # software simulation, this will generate "answer.txt"
cd ..
java BFSVerifier fb_answer.txt answer.txt # or "lj_answer.txt" if LiveJournal. It should say "PASS!"
rm graph.txt
rm answer.txt
```
Always name the graph you want to run on as "graph.txt".

## Hardware Simulation
