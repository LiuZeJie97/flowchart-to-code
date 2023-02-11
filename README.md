# Flowchart2Code

convert Flowchart \ Activity-Diagram into Pseudo-Code. 

## Example
### input
<p align="center">
<img src="https://github.com/LiuZeJie97/flowchart-to-code/blob/master/img/FC_example_1.png" width="400"/>
<p>

### output

```python
def twoSum(self, nums, target):
    Set n to the length of the array num. 
    Set i as the index of the array, the initial value is 0
    while(I is less than n?)
        Define j as the subscript of the array, and the initial value of j is i+1
        while(I is less than n?)
            if (The sum of nums[i] and nums[j] is equal to the target)
                output:  Return their array indices
            Increment j
        Increment i
    output:  Returns an empty list
```

## Usage
```cpp
#include "AvtivityCodeGen.h"

int main()
{
	AvtivityCodeGen cg;
	cg.FC2Pseudocode("examples\\input.txt", "examples\\output.txt");
    return 0;
}
```
## Method
The method was proposed in the paper ["Research on and implementation of the algorithm from the program flowchart to the code"](https://journal.xidian.edu.cn/xdxb/CN/abstract/abstract11616.shtml#1.), and implemented in the paper ["Code Generation From Flowcharts with Texts: A Benchmark Dataset and An Approach"](https://aclanthology.org/2022.findings-emnlp.449/)

<img src="https://github.com/LiuZeJie97/flowchart-to-code/blob/master/img/fc2pseudocode3.png">fc2pseudocode

- Step 1: Find out the loop and selection in the flowchart. 
Flowchart is a combination of two basic structures: selection and loop. 
[Wang et al.](https://journal.xidian.edu.cn/xdxb/CN/abstract/abstract11616.shtml#1.) found that the flowchart can be seen as a directed graph, in which each loop forms a strongly connected sub-graph. They used this method to find all the loop structures in the flowchart. Then, the structures led by the remaining \emph{Condition} nodes are selection structures. 

- Step 2: Identify the nodes (e.g. while) and edges (e.g. continue, break, return) associated with the first category of tokens in the loop. 
We identified these structures based on their characteristics. For example: 1) The True branch of the continue node will point to the while node. 2) The True branch of the break and return nodes will jump out of the loop.

- Step 3: Determining the scoping of Selection. 
In the first step, we have found the Condition nodes related to selection. In this step, we need to find where the 2 branches of the selection meet.


## Reference
- [WANG Liming;WANG Guonü;ZHOU Mingyuan;CHU Yanli;CHEN Ke;CHEN Ping. Research on and implementation of the algorithm from the program flowchart to the code[J]. J4, 2012, 39(6): 70-77.](https://journal.xidian.edu.cn/xdxb/CN/abstract/abstract11616.shtml#1.)
- [Zejie Liu, Xiaoyu Hu, Deyu Zhou, Lin Li, Xu Zhang, and Yanzheng Xiang. 2022. Code Generation From Flowcharts with Texts: A Benchmark Dataset and An Approach. In Findings of the Association for Computational Linguistics: EMNLP 2022, pages 6069–6077, Abu Dhabi, United Arab Emirates. Association for Computational Linguistics.](https://aclanthology.org/2022.findings-emnlp.449)