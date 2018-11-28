# Компонент Mapper

Данный проект реализует компонент Mapper.

Поиск кратчайшего пути между нодами сети выполняется с помощью
алгоритма Дейкстры, реализованного с помощью биномиальной
очереди. Используется реализация биномиальной очереди из  `boost.heap`.

# Сборка

Для сборки необходимо выполнить команды

    cmake .
    make

При этом необходие зависимости, `googletest` и `boost`, будут загружены
cmake автоматически.

# Запуск тестов

Для запуска тестов необходимо выполнить команду

    ./test

При этом будут запущены тесты из `test.cpp`.