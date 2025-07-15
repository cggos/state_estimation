#!/usr/bin/env python
# -*- coding: utf-8 -*-

from collections.abc import *


class test():
    """
    可迭代对象：定义了__iter__函数
    迭代器：含有__next__()函数的对象

    当调用iter函数的时候，生成了一个迭代对象，要求__iter__必须返回一个实现了__next__的对象，
    我们就可以通过next函数访问这个对象的下一个元素了，
    并且在你不想继续有迭代的情况下抛出一个StopIteration的异常(for语句会捕获这个异常，并且自动结束for)
    """

    def __init__(self, data=1):
        self.data = data

    def __iter__(self):
        return self

    def __next__(self):
        if self.data > 5:
            raise StopIteration
        else:
            self.data += 1
            return self.data


class MyRange(object):
    def __init__(self, end):
        self.start = 0
        self.end = end

    def __iter__(self):
        return self

    def __next__(self):
        if self.start < self.end:
            ret = self.start
            self.start += 1
            return ret
        else:
            raise StopIteration


# 生成器
def fib(end=1000):
    prev, curr = 0, 1
    while curr < end:
        yield curr
        prev, curr = curr, curr + prev


# 生成器被包含在list()中，list会根据传进来的参数生成一个列表，所以它对fib()对象调用__next__方法
print(list(fib()))

for item in test(2):
    print(item)

t = test(3)
for i in range(3):
    print(t.__next__())

a = MyRange(5)
print(isinstance(a, Iterable))
print(isinstance(a, Iterator))

for i in a:
    print(i)
print(next(a))
print(next(a))
print(next(a))
print(next(a))
print(next(a))
print(next(a))
