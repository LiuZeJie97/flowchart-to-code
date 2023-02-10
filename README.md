# Flowchart2Code

convert Flowchart \ Activity-Diagram into Pseudo-Code. 

## Example
### input
<p align="center">
<img src="https://github.com/LiuZeJie97/flowchart-to-code/blob/master/img/FC_example_1.png" width="400"/>
<p>

### output

```python
def twoSum(nums , target) :
    n = len(nums)
    for i in range (n) :
        for j in range (i + 1,n) :
            if nums [i]+ nums [j] == target:
                return [i , j]
    return []
```

## Usage
```cpp
AvtivityCodeGen cg;
cg.FC2Pseudocode(inputTxtPath, outTxtPath);
```

## Reference
The method was proposed by [Wang et al](https://journal.xidian.edu.cn/xdxb/CN/abstract/abstract11616.shtml#1.), and implemented in our paper ["Code Generation From Flowcharts with Texts: A Benchmark Dataset and An Approach"](https://aclanthology.org/D18-2002/)