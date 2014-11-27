# -*- coding: utf-8 -*-

a = raw_input('Введите цитату ')
b = a[0:len(a)].lower()
c = a[0:len(a)].upper()
d = a[0].upper() + a[1:len(a)].lower()
print '%s\n%s\n%s\n' % (b,c,d)
