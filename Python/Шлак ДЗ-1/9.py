# -*- coding: utf-8 -*-

a = int(raw_input('Введите год '))
b = int(raw_input('Введите месяц '))
c = int(raw_input('Введите число '))
time = {
    'year':'г',
    'month': ['Января','Февраля','Марта','Апреля','Мая','Июня','Июля','Августа','Сентября','Октября','Ноября','Декабря'],
    'day':'-ое'
}
print '%s %s %s' % (str(c)+time['day'],str(b)+' '+time['month'][b-1],str(a)+time['year'])