### ИВ-921 Жданов Артём.

Язык C#

Для запуска необходимо:
* Склонировать этот репозиторий
* Установить Open JDK 7(или выше)
* Установить Antlr 4.9
* Установить Cmake 3
* GNU Make 4.3
* Clang 14.0

Сборка грамматики:
(В корне репозитория)
```
chmod +x install_grammar.sh && ./install_grammar.sh
```

Сборка проекта:
(В корне репозитория)
```
mkdir build && cd build && cmake .. && cmake --build .
```

Опции для запуска: \
``--dump-tokens`` выводит все токены в терминал в формате: \
``Loc=<строчка, столбец> имя токена 'значение токена'`` 

``--dump-ast`` выводит AST-tree в формате XML 
Пример(examples/test.cs):
```
<program>
  <function name='main' return-type='int', return_statement=(value="Enough.", literal=1, type=STRING)/>
    <scope name='main'>
      <variable name='f' type='float'/>
      <variable name='f' type='float'/>
      <assign lhs=f, rhs=10.1, op=''/>
      <variable name='s' type='string'/>
      <assign lhs=s, rhs="Hello", op=''/>
      <variable name='c' type='char'/>
      <assign lhs=c, rhs='A', op=''/>
      <variable name='i' type='int'/>
      <assign lhs=i, rhs=10, op=''/>
      <assign lhs=f, rhs=(10, "Hello"), op='+'/>
      <variable name='d' type='float'/>
      <assign lhs=d, rhs=(10.2, 'G'), op='+'/>
      <variable name='e' type='float'/>
      <assign lhs=e, rhs=('E', 42.2), op='+'/>
      <assign lhs=f, rhs=(a, b), op='+'/>
      <call name='func', args=("Hello, World!", 3.0, 2, 1, 0.0, 0, 'R', 'U', 'N', '!')/>
    </scope>
</program>
```

``--dump-table`` выводит таблицу символов
Пример 1:
```
                  10              NUMBER  1           LVARIABLE
                  15              NUMBER  1           LVARIABLE
                   2              NUMBER  1           LVARIABLE
                   a           ASSIGN_ID  1            VARIABLE
                   b                 int  1            VARIABLE
                   c                 int  1            VARIABLE
                   d                 int  1            VARIABLE
                   i                 int  1           PVARIABLE
                   j                 int  1            VARIABLE
                main                 int  0        FUNCTION_DEF
```
Первая колонка - значение символа, указанного в коде. \
Вторая колонка - тип символа. У литератов - это NUMBER, в случае оператора присваивания: ``int b = a + 2``, ``a`` будет является ASSIGN_ID \
Третья колонка - уровень вложенности. 0 - глобальный scope. \
Четвертая колонка - описание символа. Префикс "L" говорит о том, что символ литерал, "P" - символ относится к параметру функции.

Пример 2(examples/test3.cs):
```
Scope: Global
                            sm                 int  0            VARIABLE
Scope: func
                             a                 int  1            VARIABLE
                             c                char  1      PVARIABLE_func
                             f               float  1      PVARIABLE_func
                          func                 int  0        FUNCTION_DEF
                             i                 int  1      PVARIABLE_func
                             s              string  1      PVARIABLE_func
                         smain                 int  1            VARIABLE
Scope: main
                            10              NUMBER  1           LVARIABLE
                             2              NUMBER  2           LVARIABLE
                             i                 int  1            VARIABLE
                          main               float  0        FUNCTION_DEF
                            sm                 int  1       FOROPVARIABLE
```
У каждой функции выводится свой Scope. Также существует глобальная область Global.

``-x, --to-xml``(optional) \
ключ, указывающий парсеру в какой файл записывать AST в формате XML

``--wall``(optional) \
ключ, который активирует дополнительные проверки:
  1. На неиспользуемые переменные.
  2. На пустые функции.

``--opt``(optional) \
ключ, активирующий оптимизацию AST. Из AST удаляются: \
  1. Все неиспользуемые переменные. \
  2. Пустые функции, в том числе все вызовы таких функций. \
Пример: \

```
int func(string s, int i, float f, char c) {
    int var = 42;
}

int main()
{
    string s;
    int i;
    float f;
    char c;
    func(s, c, f, i);
    return i;
}
```

Если посмотреть на AST, то оно выглядит так:
```
<program>
  <function name='func', params=(s, string; i, int; f, float; c, char), return-type='void'/>
    <scope name='func'>
      <assign lhs=(var, int), rhs=42, op=''/>
    </scope>
  <function name='main' return-type='int', return_statement=(value=i, literal=0, type=ID)/>
    <scope name='main'>
      <assign lhs=(s, string)/>
      <assign lhs=(i, int)/>
      <assign lhs=(f, float)/>
      <assign lhs=(c, char)/>
      <call name='func', args=(s, c, f, i)/>
    </scope>
</program>
```

Представленный код, хоть и создает разнообразные переменные, передаёт их в функцию, но толком ничего не делает. Результат этого кода заложен здесь:
```
return i;
```
И это является, пожалуй, самой важной частью, которую следовало бы сохранить после оптимизации.
Если к компиляции применить опцию ``--opt``, то AST будет выглядеть так:
```
<program>
  <function name='main' return-type='int', return_statement=(value=i, literal=0, type=ID)/>
    <scope name='main'>
      <assign lhs=(i, int), rhs=(1, 2), op='+'/>
    </scope>
</program>
```

``--dump-asm``(optional) \
ключ, для вывода llvm ir в терминал

``-o, --out``(optional) \
ключ, указывающий куда сохранять ll файл. Обязательно указать имя самого файла с расширением .ll

``-f, --file``(required) \
ключ, указывающий какой файл обработать до IR \
Пример:
```
Loc=<29, 48> ZERO '0'
Loc=<29, 49> RRP ')'
Loc=<29, 50> SEMICOLON ';'
Loc=<29, 52> ID 'i'
Loc=<29, 53> UNARYMATHEXP '++'
```

Запуск проекта: \
Перейти в папку `build/bin` и запустить проект с указанием любого файла, лежащего в `examples` после ключа `-f`:
```
cd build/bin && ./app -f ../../examples/nod.cs -o ../../examples/nod
```
На выходе получаем исполняемый файл ``nod`` в папке examples






