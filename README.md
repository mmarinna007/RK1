<div align="center">

# test-typing
Пользователю выпадает текст или набор слов, которые он должен правильно вводить с клавиатуры. 
По завершению игры, независимо то того как она завершилась (пользователь все напечатал правильно, или проиграл из-за времени или ошибок) должна выводится статистика о игре 

![rk1(1)](https://github.com/mmarinna007/RK1/assets/119120876/4788ca8e-441d-496a-ab66-88e058e1a015)

</div>


### Compile
```
gcc main.c file.c -o typetest -lncurses
```

### Example Run
```
./typetest input.txt -t 10
```


### Options
```
-i <filename> (входной файл)
-t [0 - +inf] (таймер)
-l [0 - 2]    (уровень сложности)
-f [0 - +inf] (макс. кол-во ошибок)
```
