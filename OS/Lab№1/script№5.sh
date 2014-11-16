#!/bin/bash 
# chmod +x script#5.sh
printf "Введите любую команду:\n"
read command
if [ -z "$command" ];
then
printf "Ошибка! Команда отсутствует.\n"
exit
fi
printf "Вы ввели команду $command \n"
"$command"