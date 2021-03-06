<h2> Реализация алгоритма APRIORI </h2>
<h3> Предназначение алгоритма </h3>
Алгоритм выявляет ассоциативные правила из матрицы транзакций.<br>
Матрицей транзакций считаем матрицу MxN, где M - количество транзакций, N - количество объектов.<br>
Элементы матрицы принимают значения 0 - при отсутствии в транзакции, 1 - при присутствии.<br>
В результате, для каждого выявленного правила вычисляется поддержка - отношение количества встретившихся правил к количеству транзакций.<br>
<h3> Описание реализации </h3>
Алгоритм строит для каждого объекта списки транзакций, отсекает те, частота которых ниже заданного уровня поддержки.<br>
После чего происходит сортировка по номеру объекта. Объекты считаем инициализацией ассоциативных правил.<br>
Следующим действием проходим по всем существующим правилам и проверяем, возможность добавления в них нового объекта.<br>
Каждое правило, включающее более, чем 1  объект сохраняется.
Повторяем процедуру генерации и проверки, пока хотя бы одна генерация создаёт правило с поддержой выше заданного уровня.<br>
В результате работы алгоритма мы получаем все ассоциативные правила.
<h3> Рекомендуемые требования </h3>
Ubuntu 16.04<br>
gcc 5.4.0<br>
<h3> Сборка </h3>
Для сборки проекта необходимо в папке Release выполнить команду make
<h3> Входные данные </h3>
./apriori {file name} {support in percents} {lines count} {sort type none:asc:desc}, где <br>
{file name} - имя входного файла, представленного в виде матрицы, состоящей из символьных значений '0','1' без разделителя, строчки разделены  переносом строки '\n' <br>
{support in percents} - значение поддержки типа double в процентах <br>
{lines count} - количество выводимых строчек <br>
{sort type none:asc:desc} - порядок выводимых строк, принимаемые значения none:asc:desc <br>
<h3> Выходные данные </h3>
Ассоциативные правила и их поддержка <br>
{4} {61} -> 0.0031 <br>

