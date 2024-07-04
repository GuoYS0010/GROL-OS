# 一个全局错误
文档中对`base_address` `rom` `uboot` 的关系阐述有误。使用过`vivado`作硬件开发的同学应该都有经验，在作`block_design`的时候就要进行`address map`，也就是说，`base_address`在硬件流出来之前就已经确定好了，和`rom` `uboot`无关。