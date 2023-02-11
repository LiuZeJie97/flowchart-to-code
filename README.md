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
AvtivityCodeGen cg;
cg.FC2Pseudocode(inputTxtPath, outputTxtPath);
```

## Reference
The method was proposed in the paper [程序流程图到代码的自动生成算法](https://journal.xidian.edu.cn/xdxb/CN/abstract/abstract11616.shtml#1.), and implemented in the paper ["Code Generation From Flowcharts with Texts: A Benchmark Dataset and An Approach"](https://aclanthology.org/D18-2002/)