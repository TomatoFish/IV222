# -*- coding: utf-8 -*-

a = int(raw_input('Введите высоту лифта '))
b = int(raw_input('Введите длину лифта '))
c = int(raw_input('Введите ширину лифта '))

a1 = int(raw_input('Введите высоту холодильника '))
b1 = int(raw_input('Введите длину холодильника '))
c1 = int(raw_input('Введите ширину холодильника '))

if a < a1 or b < b1 or c < c1:
    print 'Холодильник не влез'
else:
    print 'Свободного места в лифте {0}'.format((a*b*c)-(a1*b1*c1))
