# -*- coding: utf-8 -*-

def bills(res):
    a = res / 50
    res -= a * 50
    b = res / 20
    res -= b * 20
    c = res / 10
    res -= c * 10
    d = res / 5
    res -= d * 5
    print 'Количество 50 - [%d], 20 - [%d], 10 - [%d], 5 - [%d]' % (a, b, c, d)


a = float(raw_input('Введите сумму в рублях '))
b = float(raw_input('Введите курс евро '))
result = int(a / b)
print 'Результат евро - {0}'.format(result)
bills(result)

