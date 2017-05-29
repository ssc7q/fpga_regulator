# -*- coding: utf-8 -*-
"""
Редактор Spyder

Это временный скриптовый файл.
"""


a = 2047
b = 7
def getDx(cnt, pre_cnt):
    if(cnt>=pre_cnt):
        if((cnt-pre_cnt)<=1024):
            return (cnt-pre_cnt) #+
        else:
            return cnt-(2048+pre_cnt) #+
    else:
        if((pre_cnt-cnt)<=1024):
            return  cnt-pre_cnt
        else:
            return (2048+cnt-pre_cnt)

print(getDx(a, b))