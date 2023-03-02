# **Source code for QPT mining, MQPCore and MQPClique mining**
This repository contains a reference implementation of algorithms for Quasi-Periodic Communities in Temporal Network, including algorithms for QPT mining, MQPCore and MQPClique mining.
## **Environment Setup** ##
All algorithms are implemented with C++11. You may use Git to clone the repository from GitHub and enter the main folder manually using the following command:
```
git clone git@github.com:bruce1114/qpcommunity.git
cd qpcommunity
```
## **QPT mining** ##
For QPT mining, you may complile "test_weds.cpp" first and run it with any datasets in "smalldatasets/smallsequences":
```
g++ test_weds.cpp -o test_weds -std=c++11
./test_weds 
seq_100 4 5 \*dataset name, sigma, epsilon (%)*\
build oracle time: 20
total time: 27 
total result: 1767
```
Process above enables the DAG oracle based method. To switch to the basic method you may "#define candidate" rather than "#define oracle" at the top of "test_weds.cpp".
## **MQPCore and MQPClique mining** ##
For MQPCore mining (and MQPClique mining), you may complile "main.cpp" first and run it with any datasets in "smalldatasets/smallgraphs":
```
g++ main.cpp -o main -std=c++11
./main
test2 3 3 20 1 \*dataset, k, sigma, epsilon (%). 1 indicates that MQPCL-B(+) is invoked*\
temporal graph size:
total node: 8
total edge in de-temporal graph: 16
total temporal edge: 54
total time: 0
total result: 5
all pcliques:
1 2 3 -1 3 5 6
1 2 3 -1 4 5 6
1 3 5 -1 0 1 2 3
3 5 7 -1 0 1 2
3 5 7 -1 0 1 3
```
Process above enables the DAG oracle based method. To switch to the basic method you may "#define candidate" rather than "#define oracle" at the top of "main.cpp".