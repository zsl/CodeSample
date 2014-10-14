# 用1 2 3 4 5 6 7 8 9 九个数字，用加减法弄成结果等于100
# 也不能上网。用纸和笔试了试，半天也没试出来。
# 还是python写个程序吧。暴力试探，把所有的情况全给它暴力试一遍得啦：
# 一试答案还不止一个
# 
# 1+2+3-4+5+6+78+9 = 100
# 1+2+34-5+67-8+9 = 100
# 1+23-4+5+6+78-9 = 100
# 1+23-4+56+7+8+9 = 100
# 12+3+4+5-6-7+89 = 100
# 12-3-4+5-6+7+89 = 100
# 12+3-4+5+67+8+9 = 100
# 123-4-5-6-7+8-9 = 100
# 123+4-5+67-89 = 100
# 123+45-67+8-9 = 100
# 123-45-67+89 = 100
# 
# 有没有其他方法呢？

#!/usr/bin/env python
#-*- coding:gbk -*-
#暴力解决方式
def get_list_list(n):
    if n == 1:
        return [[1]]
    else:
        list_list = get_list_list(n - 1)
        list_list_2 = []
        for l in list_list:
            l_temp = l[::] 
            l_temp.append(n)
            list_list_2.append(l_temp)
            l[-1] = l[-1]*10 + n
            list_list_2.append(l)
    return list_list_2
def parse_list(l):
    if len(l) == 1:
        return {str(l[0]) : l[0]}
    else:
        dic = parse_list(l[0:-1])
        num = l[-1]
        dic2 = {}
        for key in dic.keys():
            dic2[key + ' + ' + str(num)] = dic[key] + num
            dic2[key + ' - ' + str(num)] = dic[key] - num
        return dic2
NUM_RESULT = 100
if __name__ == '__main__':
    list_list = get_list_list(9)
    for l in list_list:
        dic = parse_list(l)
        for key in dic.keys():
            if dic[key] == NUM_RESULT:
                print key, "=" , dic[key]

