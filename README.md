# B-tree
- A simple implementation of a B+ tree data structure.

An example output
```
-----Insert-----
[1]
[1 3]
[1 3 10]
[[1 3]10[10 15]]
[[1 3 6]10[10 15]]
[[1 3 6]10[10 15 22]]
[[1 3 6]10[10 15]22[22 30]]
[[1 3 6]10[10 15 20]22[22 30]]
[[1 3 6]10[10 15 20]22[22 30 50]]
[[1 3 6]10[10 15 20]22[22 30]50[50 55]]
[[1 3 6]10[10 15 20]22[22 30 40]50[50 55]]
[[1 3 6]10[10 15 20]22[22 30 40]50[50 55 60]]
[[[1 3]6[6 8]10[10 15 20]]22[[22 30 40]50[50 55 60]]]

[Done] exited with code=0 in 0.449 seconds
```
```
-----Insert-----
[[[[[[2 3 4]5[5 6]7[7 8]]9[[9 10]11[11 12]]13[[13 14]15[15 16]]17[[17 18]19[19 20]]]21[[[21 22]23[23 24]]25[[25 26]27[27 28]]]29[[[29 30]31[31 32]]33[[33 34]35[35 36]]]]37[[[[37 38]39[39 40]]41[[41 42]43[43 44]]]45[[[45 46]47[47 48]]49[[49 50]51[51 52]]]]53[[[[53 54]55[55 56]]57[[57 58]59[59 60]]]61[[[61 62]63[63 64]]65[[65 66]67[67 68]]]]]69[[[[[69 70]71[71 72]]73[[73 74]75[75 76]]]77[[[77 78]79[79 80]]81[[81 82]83[83 84]]]]85[[[[85 86]87[87 88]]89[[89 90]91[91 92]]]93[[[93 94]95[95 96]]97[[97 98]99[99 100]]]]]]

[Done] exited with code=0 in 0.864 seconds
```

```
~~
[[[[999973 999974]999975[999975 999976]999977[999977 999978]]999979[[999979 999980]999981[999981 999982]999983[999983 999984]]999985[[999985 999986]999987[999987 999988]999989[999989 999990]]]999991[[[999991 999992]999993[999993 999994]999995[999995 999996]]999997[[999997 999998]999999[999999 1000000]]]]]]]]]]]]]

[Done] exited with code=0 in 1.284 seconds
,,,
