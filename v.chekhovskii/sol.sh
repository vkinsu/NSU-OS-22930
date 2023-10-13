#!/bin/bash

echo -e "\n"$(date)
echo -e "====== Подключение к Solaris... ======\n"

ssh 10.4.0.68
# ssh solarka

echo -e "\n"$(date)
echo -e "====== Вы отключены от Solaris. ======\n"
