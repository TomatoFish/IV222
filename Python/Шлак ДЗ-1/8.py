# -*- coding: utf-8 -*-

a = raw_input('Введите цитату ')
a = a[0].upper() + a[1:len(a)].lower()
print a
b = raw_input('Какое слово хотите заменить? ')
c = raw_input('На что хотите заменить? ')
a = a.replace(b, c)
print a
