## POSIX Thread

Документация:

* [pthreads](http://man7.org/linux/man-pages/man7/pthreads.7.html)
* [pthread_create](http://man7.org/linux/man-pages/man3/pthread_create.3.html)
* [pthread_join](http://man7.org/linux/man-pages/man3/pthread_join.3.html)
* [pthread_detach](http://man7.org/linux/man-pages/man3/pthread_detach.3.html)


Функция создания потока:
```cpp
pthread_create(pthread_t *thread_id, const pthread_attr_t *attr, void *(*start_fcn)(void *), void *arg);
```
_arg_ - будет передан в функцию `start_fcn`.

Пример:
```cpp
// Функция увеличивает переданное в неё число
// и возвращает увеличенное значение
void *thread_func(void *value) {
    // передаем целое число
    int *int_value = static_cast<int *> value;

    ++(*int_value);

    return int_value;
}
```

Потоки могут быть двух классов:

1. **Joinable** - похожи на процессы UNIX. Вызывая функции `wait` или `waitpid`, мы ожидаем статусы.
2. **Detached** - статус не ожидается

### Joinable-потоки

Любой поток при создании, если явно не указано другого, является *Joinable*. Идет
основной поток, мы вызвали `pthread_create`:

```
|
|
|
| pthread_create
x-----
|     |
|     |
|     x <- завершение и засыпание
|
| pthread_join
x <--- возврат результата из дочернего потока
|
```

Второй сценарий:

```
|
|
|
| pthread_create
x-----
|     |
x <---|---- pthread_join
      |
      |
      |
x <---|
```

```cpp
int pthread_join(pthread_t thread, void **retval);
```

_retval_ - получение возвращаемого значение из дочернего потока.


### Detached-потоки

Поток перестает нуждаться в join'е.

```
|
|
|
| pthread_create
x-----
|     |
|     |
|     |
|     x pthread_detach
|     |
|     x <- завершение потока
|
|
|
```

Но в этом случае возвращаемый результат теряется.

```cpp
int pthread_detach(pthread_t thread);
```

Что будет если потоки вызовут `join` друг для друга? Система не даст этого сделать,
так как получится **deadlock**. В этом случае `pthread_join` вернет `EDEADLK`.

Поток можно преобразовать из *Joinable* в *Detached*, но наоборот - нельзя.
Чтобы создать сразу *Detached*-поток, при создании необходимо задать соответствующий статус в структуре
`pthread_attr_t *attr` (для инициализации и удаления структуры используются функции
`pthread_attr_init` и `pthread_attr_destroy`). Для это используется функция

```cpp
int pthread_setdetachstate(pthread_attr_t *attr, int detachstate);
```
_detachstate_ - может быть двух типов - `PTHREAD_CREATE_JOINABLE` или `PTHREAD_CREATE_DETACHED`.

### Завершение потоков

Бывает явным и неявным. Пример неявного завершения:

```cpp
void *thread_func(void *arg) {
  // ...
  // ...
  return nullptr // <- Implicit termination
}
```

Для явного завершения потока используется функция:

```cpp
void pthread_exit(void *retval); // Explicit termination
```

Другой способ явно завершить поток, это отправка команды на завершение из другого потока.
Например:

```
|
|
x----
|   |
|   |
x <-x---- pthread_cancel
|
|
|
```

В этом случае мы не получаем возвращаемое значение дочернего потока.

```cpp
int pthread_cancel(pthread_t pthread);
```

Убить поток можно только во время исполнения какого-либо системного вызова. Например, во время
`read`, `waitpid` и т.д. Если мы используем конструкцию `while (true) {}`, то поток мы не сможем убить.
Функции `pthread_*`, `free`, `malloc`, `realloc`, `calloc` не являются точками завершения.
Мы можем самостоятельно установить точку завершения с помощью функции `void pthread_testcancel(void)`.

Ещё один момент:

```
|
| pthread_create
x---------
|         |
|         |
x----
|    |    |
|    |    |
|    x--->|---- pthread_join
|     ----|----> join вернул *retval = PTHREAD_CANCELED
x----x--->|---- pthread_cancel
|    |
|    |
```

Есть также параметры потока, которые мы можем установить находясь внутри него.

```cpp
int pthread_cancelstate(int state, int *oldstate);
```

_state_ - принимает два значения:

* `PTHREAD_CANCEL_ENABLED` - можем отменять выполнение потока с помощью
`pthread_cancel`.
* `PTHREAD_CANCEL_DISABLED` - поток не может быть завершен с помощью
`pthread_cancel`.

```cpp
int pthread_canceltype(int type, int *oldtype);
```

_type_ - может принимать два значения:

* `PTHREAD_CANCEL_DEFERED` - стандартный режим с точками завершения
* `PTHREAD_CANCEL_ASYNCHRONOUS` - в этом режиме мы не ждем точку завершения, а можем сразу
прервать работу потока.

Для установки функции, которая будет вызываться при завершении работы потока имеются
следующие вызовы:

```cpp
void pthread_cleanup_push(...); // для установки функции
void pthread_cleanup_pop(...); // для сброса
```

## Мьютексы

Потоки являются частью одного процесса и вся память процесса является для них общей.
Как из разных потоков работать с общей памятью? Для этого существует несколько способов и
один из них это использование **мьютексов**.

Мьютекс как бинарный семафор (флаг), может находится либо в активном состоянии («1»), либо
в неактивном («0»). Это позволяет только одному процессу работать с каким-либо общим ресурсом
в конкретный момент времени.

Для создания мьютекса используется:

```cpp
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER; // создание мьютекса

int pthread_mutex_init(pthread_mutex_t *mp, pthread_mutex_attr_t *mattrp); // инициализация памяти под мьютекс
```

Для удаления мьютекса используется функция:

```cpp
int pthread_mutex_destroy(pthread_mutex_t *mp);
```

Пример работы потоков с мьютексом:

```
|   |
|   |
x <-|--- lock_1
|   o <- lock_2
|   -    поток 2 спит
|   -    поток 2 спит
x <-x--- unlock_1 / lock_2
```

Для работы с блокировками мьютекса существуют следующие функции:

```cpp
int pthread_mutex_lock(pthread_mutex_t *mp); // lock в блокирующем режиме
int pthread_mutex_trylock(pthread_mutex_t *mp); // lock в неблокирующем режиме
                                                // в случае, если мьютекс занят, то вернется отказ
int pthread_mutex_unlock(pthread_mutex_t *mp); // снятие блокировки
```

### spin-блокировки

При работе с блокировками, если время работы первого процесса мало, то вместо того, чтобы заснуть и ожидать
освобождения мьютекса, целесообразнее использовать spin-блокировку. В этом случае другой поток, ожидающий
разблокировки мьютекса в бесконечном цикле постоянно проверяет мьютекс и «греет камень».

```cpp
pthread_spin_lock_t psl;

int pthread_spin_init(pthread_spin_lock_t *lock, int pshared); // инициализация
```

spin-локи могут работать для синхронизации потоков в одном процессе, так и для синхронизации
потоков разных процессов! То есть можно синхронизировать процессы. В таком случае
переменная `pshared` должна принять значение `PTHREAD_PROCESS_SHARED`.
Если же `pshared` равно `PTHREAD_PROCESS_PRIVATE`, то в таком случае spin-lock используется в
рамках одного процесса.

Для уничтожения spin-lock'а используется:

```cpp
int pthread_spin_destroy(pthread_spin_lock_t *lock);
```

Для работы со spin-блокировками используются функции:

```cpp
int pthread_spin_destroy(pthread_spin_lock_t *lock);
int pthread_spin_lock(pthread_spin_lock_t *lock); // занять spin-lock (в блокирующем режиме)
int pthread_spin_trylock(pthread_spin_lock_t *lock); // занять spin-lock (в неблокирующем режиме)
int pthread_spin_unlock(pthread_spin_lock_t *lock); // освободить spin-lock
```

**Важно!** Если блокировки короткие по времени, то рекомендуется использовать spin-блокировки
вместо засыпания потока.

### Блокировки на запись и на чтение

* [PTHREAD_RWLOCK](https://www.daemon-systems.org/man/pthread_rwlock.3.html)

```cpp
pthread_rwlock_t *rwlock = PTHREAD_RWLOCK_INITIALIZER;

pthread_rwlock_init(...);
pthread_rwlock_destroy(...);
```

Установка блокировок на чтение и запись:

```cpp
int pthread_rwlock_rdlock(*rwlock);
int pthread_rwlock_tryrdlock(*rwlock);
int pthread_rwlock_wrlock(*rwlock);
int pthread_rwlock_trywrlock(*rwlock);
```

### Условные переменные. Барьеры

Пример использования условных переменных:

```
|     |
|     |
|     |
x <---|---- Поток 1 устанавливает условную переменную
- сон |
-     |
-     |
|<----|---- Поток 2 пробуждает Поток 1
```

Создание условной переменной:

```cpp
int pthread_cond_init(pthread_cond_t * restrict cond, pthread_condattr_t * restrict attr);
// для удаления
int pthread_cond_destroy(pthread_cond_t *cond);
// если переменная создается на стеке
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
```

Для того, чтобы работать с условными переменными, необходимо создать мьютекс. После этого
можно использовать следующую функцию:

```cpp
// встать на ожидание
int pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex);
// встать на ожидание с таймаутом
int pthread_cond_timedwait(pthread_cond_t *cond, pthread_mutex_t *mutex, const struct timespec * restrict abstime);
```

Сигнализировать о прекращении ожидания можно либо одному потоку, либо всем сразу.

```cpp
// для отправки сигнала конкретному потоку
int pthread_cond_signal(pthread_cond_t *cond);
// для отправки сигнала всем спящим потокам
int pthread_cond_broadcast(pthread_cond_t *cond);
```

Создание барьеров позволяет пробуждать все потоки, при достижении определенного числа заснувших потоков.

```cpp
// инициализация барьера
int pthread_barrier_init(pthread_barrier_t *bp, pthread_barrierattr_t *attr, unsigned count);
// уничтожение барьера
int pthread_barrier_destroy(pthread_barrier_t *bp);
// ожидание достижения count-спящих потоков
int pthread_barrier_wait(pthread_barrier_t *bp);
```

### Однократный запуск потоков

Если из нескольких потоков запускается одна и таже процедура, но мы хотим, чтобы она вызвалась только
один раз, то можно использовать однократный запуск:

```cpp
pthread_once_t once_control = PTHREAD_ONCE_INIT;
int pthread_once(pthread_once_t *ocp, void (*init_routine)(void));
```

### C++11 Threads

Для создания потоков в C++11 можно использовать класс `thread` (определен в заголовочном файле
<thread>).

```cpp
std::thread t(ThreadFunc);
// прототип функции потока
void ThreadFunc(void);
```

Если необходимо передавать дополнительные параметры в функцию, то можно их указать при
объявлении потока:

```cpp
std::thread t(ThreadFunc, 16, 3.14, "Hello");
// прототип функции
void ThreadFunc(int i, double d, std::string s);
```

Все передаваемые значения будут скопированы в функцию потока. Чтобы этого избежать
можно использовать обертку `std::ref`:

```cpp
std::thread t(ThreadFunc, std::ref(i), std::ref(d));
// если не использовать std::ref, то даже с такой сигнатурой функции все значения будут копироваться
void ThreadFunc(int& i, double& d);
```

Кроме обычных определений функций можно определять соответствующие классы с перегруженным
оператором круглые скобки `void operator()() {}` или же использовать лямбды.

```cpp
std::thread t([]() {...});
```

Для работы с потоками используются методы:

```cpp
std::thread t(...);

t.join();
t.detach();
```

* [C++11 Thread документация](http://en.cppreference.com/w/cpp/thread)

## Параллельное программирование

1. OpenMP
2. Intel Threading Building Blocks (TBB)
3. MPI

1, 2 подход - это подходы к программированию с общей памятью, а 3 - с использованием
сообщений.

### OpenMP

* [Поддерживаемые компиляторы](http://openmp.org/wp/openmp-compilers/)
* [GNU libomp docs](https://gcc.gnu.org/onlinedocs/libgomp/)
* [OpenMP Tutorials](https://computing.llnl.gov/tutorials/openMP/)
* [Подводные камни OpenMP](http://www.viva64.com/ru/a/0054/)


С помощью специальной директивы `pragma` можно запустить необходимый участок кода в нескольких
потоках. Например:

```cpp
#include <iostream>

int main() {
  // Запуск в несколько потоков
  #pragma omp parallel
  {
    std::cout << "Hello, world!" << std::endl;
  }
  return 0;
}

```

Для задания количества потоков:

*	непосредственно в директиве `#pragma` - `#pragma omp parallel num_threads(4)`
* до директивы `#pragma` - `omp_set_num_threads(4)`
* с помощью `export` - `export OMP_NUM_THREADS=4`

В итоге мы получили следующую схему выполнения программы:

```
      |
      |
      |
---------------
|   |    |    |
|   |    |    |
|   |    |    |
---------------
      |
      |
      |
```

Для того, чтобы подключить OpenMP следует использовать следующие флаги:

```
gcc -fopenmp
Intel: -openmp (Linux, MacOSX), -Qopenmp (Windows)
Microsoft: -openmp (настройки проекта в Visual Studio)
```

### Переменные

При использовании директивы `pragma` мы создаем новую область видимости. И переменные в такой области
могут быть как глобальными (единственный экземпляр для всех потоков), так и внутренними (для каждого
потока – своя переменная). Это необходимо явно указывать в директиве `pragma`:

```cpp
#pragma omp parallel shared(A, B, C) private(i, n)
// shared - глобальные переменные. Видны всем
// private - внутренняя переменые. У каждого потока своя переменная
```

### Циклы

```cpp
#pragma omp parallel for {
  for (int i = 0; i < 1e7; ++i) {
    // ...
  }
}
```

С помощью такой конструкции мы можем разбить выполнение циклов на несколько потоков.
Если необходимо помимо разбиения сохранить общий результат выполнения (например, накопить общую сумму),
то используется следующая конструкция:

```cpp
#pragma omp parallel for reduction(+:sum)
{
  for (int i = 0; i < 1e7; ++i) {
    sum = sum + f(i);
  }
}

// после выполнения в переменной sum будет результат сложения во всех потоках
```

### Барьеры

Барьеры устанавливаются в тех участках кода, в которых необходимо синхронизировать все потоки. То есть
по достижении барьера, поток будет ждать пока другие потоки также достигнут этого барьера,
после чего выполнение работы продолжится.

```cpp
#pragma omp parallel
{
  std::cout << 1 << std::endl;
  std::cout << 2 << std::endl;
  #pragma omp barrier
  std::cout << 3 << std::endl;
}
```

### Single-запуск

```cpp
#pragma omp parallel
{
  std::cout << 1 << std::endl;
  // выполнить один раз (какой-то из потоков выполнит последующий код)
  #pragma omp single
  {
    std::cout << 2 << std::endl;
  } // тут срабатывает барьер для все потоков

  std::cout << 3 << std::endl;
}
```

Если мы не хотим, чтобы был барьер:

```cpp
#pragma omp parallel
{
  std::cout << 1 << std::endl;

  #pragma omp single nowait
  {
    std::cout << 2 << std::endl;
  }

  std::cout << 3 << std::endl;
}
```

Если требуется, чтобы определенный код был выполнен только главным потоком, то используем
следующую конструкцию:

```cpp
#pragma omp parallel
{
  std::cout << 1 << std::endl;

  #pragma omp master
  {
    std::cout << 2 << std::endl;
  }

  std::cout << 3 << std::endl;
}
```

### Упорядоченное выполнение

Допустим у нас есть цикл и мы хотим последовательно выводить индексы действий:

```cpp
for (int i = 0; i < 1e7; ++i) {
  std::cout << i << std::endl;
}
```

Для распараллеливания этой задачи используем соотвествующую директиву `pragma omp parallel for` и
добавляем модификатор `ordered`:

```cpp
#pragma omp parallel for ordered
for (int i = 0; i < 1e7; ++i) {
  #pragma omp ordered
  {
    std::cout << i << std::endl;
  }
}
```

В этом случае все потоки будут согласованы между собой и вывод индекса будет выполнен
в нужном порядке.

### Критическая секция

Критическая секция может быть только одна в единицу времени.

```cpp
#pragma omp parallel
{
  #pragma omp critical
  {
    // Критическая секция
  }
}
```

При попадании в критическую секцию, все остальные потоки приостанавливают свое выполнение. После выхода одного
потока из критической секции, другой поток продолжает свое выполнение и т.д.

### Атомарные операции

```cpp
#pragma omp parallel
{
  #pragma omp atomic
  n++;
}
```

### Параллельное исполнение разного кода

```cpp
#pragma omp sections
{
  #pragma omp section
  {
    // action 1
  }
  #pragma omp section
  {
    // action 2
  }
  #pragma omp section
  {
    // action 3
  }
}
```

### Task-потоки

```cpp
#pragma omp task
{
  // Task
}

#pragma omp taskwait
```

*Полезные ссылки*:

* [Основной сайт](http://openmp.org/wp/)
* [Полная спецификация](http://www.openmp.org/mp-documents/openmp-4.5.pdf﻿)
* [Примеры](http://openmp.org/mp-documents/openmp-examples-4.0.2.pdf﻿)
* [Статья в Википедии (есть пара примеров)](https://en.wikipedia.org/wiki/OpenMP﻿)

## Intel TBB

Документация:

* [Основной сайт](https://www.threadingbuildingblocks.org/)
* [Документация](https://software.intel.com/en-us/node/506039)
* Включает в себя:
  * [﻿Developer Guide](https://software.intel.com/en-us/node/506045﻿)
  * [﻿Developer Reference Manual](https://software.intel.com/en-us/node/506130﻿)
﻿

Функция `parallel_for` аналог `#pragma omp parallel for` в OpenMP.

```cpp
// исходная функция, которую хотим распараллелить
void SerialApplyFoo(float a[], int n) {
  for (size_t i = 0; i < n; ++i) {
    Foo(a[i]);
  }
}

// создание класса для применения функции Foo к массиву float a[]
class ApplyFoo {
  float *my_a;
public:
  void operator()(const tbb::blocked_range<size_t>& r) {
    float *a = my_a;

    for (size_t i = r.begin(); i != r.end(); ++i) {
      Foo(a[i]);
    }

    ApplyFoo(float a[]) : my_a{a} {}
  }
};

// запуск параллельных задач
void ParallelApplyFoo(float a[], int n) {
  // разбиение массива на блоки
  tbb::parallel_for(tbb::blocked_range<size_t>(0, n), ApplyFoo(0, n));
}
```

Эту запись можно сократить с помощью применения лямбда-функций:

```cpp
void ParallelApplyFoo(float a[], int n) {
  auto Apply = [=](const tbb::blocked_range<size_t>& r) {
    for (size_t i = r.begin(); i != r.end(); ++i) {
      Foo(a[i]);
    }
  }

  tbb::parallel_for(tbb::blocked_range<size_t>(0, n), Apply);
}
```

### Разбиение на блоки

```cpp
// G/2 <= Chunck size (размер блока) < G
// simple_partitioner() - функция для разбиения на блоки
tbb::parallel_for(tbb::blocked_range<size_t>(0, n, G), ApplyFoo(a), simple_partitioner());
```

### Распаралеливание вычислений

Допустим есть код, который считает сумму числе в массиве:

```cpp
float SerialSumFoo(float a[], size_t n) {
  float sum = 0;

  for (size_t i = 0; i < n; ++i) {
    sum += a[i];
  }

  return sum;
}
```

Допустим, мы хотим разделить наш массив на несколько частей, посчитать сумму для каждой части
и затем объединить:

```cpp
class SumFoo {
  float *my_a;
public:
  float my_sum;

  void operator()(tbb::blocked_range<size_t>& r) {
    float *a = my_a;
    float sum = my_sum;
    size_t end = r.end();

    for (size_t i = r.begin(); i != end; ++i) {
      sum += Foo(a[i]);
    }

    my_sum = sum;
  }

  SumFoo(SumFoo& x, split) : my_a{x.my_a}, my_sum{0} {}

  void join(const SumFoo& y) {
    my_sum += y.my_sum;
  }

  SumFoo(float a[]) : my_a{a}, my_sum{0} {}
};

float ParallelSumFoo(float a[], size_t n) {
  SumFoo sf(a);

  tbb::parallel_reduce(tbb::blocked_range<size_t>(0, n), sf);

  return sf.my_sum;
}
```

## MPI

* [Вся документация по MPI](http://www.mpi-forum.org/docs/docs.html)
* [Основной документ (огромный)](http://www.mpi-forum.org/docs/mpi-3.1/mpi31-report.pdf)
* [Документация с сайтам MPICH](https://www.mpich.org/documentation/guides/﻿)
