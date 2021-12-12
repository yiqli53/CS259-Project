# 259 Final Project

## Team
Yiqun Li, Chengxin Wang, Rui Lin

## About Project
In this project, we try to implement FPGP and accelerate using TAPA. The repository contains 2 branches. The "main" branch contains the baseline or standard version of FPGP, and the "TAPA" branch contains the baseline plus TAPA acceleration. For now, we have been able to perform software simulation on both versions, and get good results.

## How to run
Please initiate a "Vitis 2020.2 Developer AMI" instance on AWS.

### Setup
```bash
git clone https://github.com/aws/aws-fpga.git $AWS_FPGA_REPO_DIR
cd aws-fpga
source vitis_setup.sh

cd Vitis/examples/xilinx
sudo apt-get install cmake
sudo apt install ca-certificates
sudo curl -L git.io/JnERa | bash
source <(curl -L bit.ly/3BzVG16)
PATH="${HOME}/.local/bin:${PATH}"
sudo apt install default-jdk
```

### Download
```bash
git clone https://github.com/yiqli53/CS259-Project.git
cd CS259-Project

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
```

### Run
Before you run, please make sure that the input graph is named as "graph.txt".
```bash
make swsim
make hwsim
```

### Verify Correctness
Make sure you are in "CS259-Project" directory, not "build"。
```bash
java BFSVerifier fb_answer.txt answer.txt
```
or
```bash
java BFSVerifier lj_answer.txt answer.txt
```
