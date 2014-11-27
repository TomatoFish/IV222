# -*- coding: utf-8 -*-

import math
x = float(raw_input('Введите диаметр бассейна '))
h = float(raw_input('Введите глубину бассейна '))
print '%f' % (math.pi * (x/2)**2 * h)