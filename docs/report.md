# Hyper OS 实验报告

## 概述

#### 实验目标

操作系统是计算机专业的一门综合性课程。实现一个操作系统，不仅需要处理器体系结构、编译原理、汇编语言、算法与数据结构等计算机核心知识作为支撑，还需要很强的文档阅读能力和系统编程能力。正因如此，操作系统课程的难度较大，对于那些不是非常熟悉硬件的同学更是如此。

另一方面，对于操作系统原理的教学和研究也经常会需要一个硬件无关的，足够简单的模拟环境。这样的模拟环境应当具有以下的特点：

1. 硬件无关：模拟环境应该反应 OS 协调进程工作的本质原理，屏蔽那些和具体硬件相关的细节问题。考虑以下两个情景。

   - 在使用 ucore lab 学习操作系统启动时，学生需要阅读 Intel 手册，了解 A20 Gate 的作用和原理、系统进入保护模式的方法以及如何设置段表、页表，这大大提高了学生理解 OS 原理的难度。事实上，如果使用模拟环境辅助学习，则可以帮助学生更清晰地理解操作系统的本质原理，启发学生如何将原理变为实现，将“怎么办到为什么”变为“为什么到怎么办”，更加符合系统开发的真实情况。
   - 在研究各种页面置换算法的优劣时，如果直接使用真实的操作系统，则不得不解决许多和页面置换问题本身无关的细节，对于需要大量实验的研究而言这样的开销是很大的。如果能先在模拟环境上测试，再上真实系统，则可以大大节省实验的时间成本。

2. 足够真实：模拟环境不应当退化为一系列 OS 知识点的整理，而应当是一个协调、完整的系统，能够正确地运行简单的“应用程序”。

   - OSTEP：OSTEP 即 《Operating Systems: Three Easy Pieces》是 [Remzi H. Arpaci-Dusseau](http://www.cs.wisc.edu/~remzi) and [Andrea C. Arpaci-Dusseau](http://www.cs.wisc.edu/~dusseau) 编写的关于操作系统原理的书籍，其配套代码利用简单的模拟代码介绍 OS 原理。之前已有人实现基于 Python 和 Rust 的 OSTEP 实现，但仍然是多个知识点的整合，并不能展示真实系统协调工作的效果。

3. 逻辑清晰：模拟环境应当遵循尽可能清晰的逻辑，揭示硬件、操作系统、软件是如何协调工作的。具体来说，要能展示 OS 处理异常，分配资源的能力。例如应用程序的一次系统调用可能要经历以下过程：

   - 应用程序使用标准库中的函数
   - 标准库调用系统调用函数，通过指令发出 SYSCALL 软中断
   - 中断处理例程响应，提供相应的服务
   - 中断处理结束，返回用户态
   - 标准库返回给调用者结果

   只有能够真实展示这些过程，才能完整展示 OS 在提供系统调用这一机制中的作用。

#### 实验描述

基于这些目标，我们提出了 Hyper OS：一个运行在用户态，利用线程之间的通信和协调模拟 OS 调度的模拟操作系统。为了实现这个目标，我们解决的主要问题包括：

1. 如何提供中断的支持：中断需要被正确的发送给“处理器”，并启动相应的中断处理例程，处理结束后需要返回用户态程序。
2. 如何提供进程的支持：进程是用户态程序，用户态程序的格式是怎样的？如何从用户态程序的手中抢夺控制权？
3. 如何提供内存的支持：内存要能和“处理器”通信，要能利用物理地址寻址，实现基本的读、写功能。
4. 如何提供外部设备支持：外部设备如何在 OS 的调度下与进程协调工作？

#### 小组分工

- 杨天祺同学实现了中断机制和外部设备的支持

- 李嘉图同学实现了操作系统内核和用户态程序

## 设计实现

### 基本方式

Hyper OS 基于线程之间的通信和同步。在 Hyper OS 中，在任何时候，具有独立控制流的设备都拥有一个处于运行状态的线程作为模拟。这些设备包括：

- 外部设备：例如时钟运行在 clock 线程，作用是每隔一段时间向中断控制器发送一次时钟中断。
- 中断控制器：包括 IO-APIC 和每个 CPU 的 LAPIC，负责中断的接收、调度和分发。
- 处理器（正在占用处理器的用户进程）：正在占用处理器的用户进程有一个线程作为模拟。
- 物理内存：物理内存运行在 memory 线程，负责接收读/写操作，并返回结果。

通过进程间同步的机制，我们可以保证具有独立控制流的设备有且只有一个线程作为模拟正在运行，从而可以实现处理器的分时复用、中断控制等机制。

### 中断机制

#### 中断控制器

中断控制器是控制和调度内外部中断的硬件。在多处理系统中，中断控制器分为两种：

- IOAPIC：IOAPIC 通常在南桥上，处理外部设备产生的中断。
- LAPIC：LAPIC 每个处理器拥有一个，处理当前处理器的内部中断，和 IOAPIC 发来的外部中断。

Hyper OS 中也依照这种机制进行了模拟。每个 IOAPIC 拥有一个线程，每个 LAPIC 拥有一个线程，他们都不断的从一个事件队列中取出未被处理的中断并分发之。

#### 外部中断

外部中断来自外部设备，例如时钟、键盘、打印机等。外部中断是外部设备通知 CPU 某个事件发生的机制，其流程如下图所示：

```sequence
Device-> IO_APIC: 外部中断
IO_APIC-> LAPIC of CPU0: 分发给某个CPU
note over CPU0: 当前进程
LAPIC of CPU0-> CPU0: 跳转到中断处理例程
note over CPU0: 中断处理例程
```

在 Hyper OS 中，外部中断的实现分为四个步骤

- 设备是单独的线程，其通过提供的接口，利用线程安全队列（`src/utils/thead_safe_queue.h`）与 IO_APIC 通信。IO_APIC 是管理外部中断的设备，所有的外部中断都要经过 IO_APIC 进一步发送给 LAPIC；
- IO_APIC 不断的从队列中取出一个未被分配的中断、优先级最高的中断，并将其分配给一些 CPU（`class core in src/core/core.h`） 的 LAPIC；
- LAPIC 设置 CPU 的中断标识 `core::set_interrupt_waiting_flag()`，等待信号以启动中断处理例程（`interrupt_t::process`）
- 正在占用 CPU 的程序定期检查标记，发现中断时，则通过`check_interrupt()`主动放弃 CPU 使用权，将自己阻塞起来的同时原子地释放其占用 CPU 的锁 （`core::release()`），并向 LAPIC 发送可以开始处理的信号，这时中断处理例程线程启动并获取 CPU 锁开始执行。

#### 内部中断

与外部中断不同，内部中断是由于某种原因，由 CPU 内部主动发出的中断，例如页错误（由 MMU）发出和系统调用中断（利用 INT 80 指令发出）。内部中断是同步的，其流程如下图所示：

```sequence
note over CPU0: 当前进程
CPU0 -> LAPIC of CPU0: 发出内部中断
LAPIC of CPU0 -> CPU0: 跳转到中断处理程序
note over CPU0: 中断处理例程
```

内部中断的实现相对简单，占用 CPU 的进程或 CPU 内部组件（如 MMU）可以通过 `interrupt(interrupt_t*)` 触发一个软中断，并立刻阻塞自己并原子地释放 CPU 锁，直到其再次被唤醒。LAPIC 立刻启动处理例程响应软中断。值得指出的是，一些软中断（例如系统调用`yield`）可能会导致进程调度，因此软中断可能发生进程切换。

#### 中断的优先级

由于中断会打断控制流，为了避免系统陷入混乱，有必要为中断设置不同的优先级。在实验中发现的由于未正确设置优先级引起的一个bug如下：

- 程序使用系统调用，但由于未设置优先级，系统调用被迫等待一个未被处理的时钟中断执行
- 时钟中断处理过程中，由于当前进程时间片用完，发生进程切换，之后的系统调用无法被正确的相应。

```sequence
clock->apic: interrupt
process 1 -> apic: syscall
note over apic: 先处理时钟中断
note over apic: 发生进程切换
apic -> process 2: 启动另一个进程
apic -> process 2: syscall作用于了新的进程
```

因此，有必要约定各种中断的优先级。基本的原则有以下两条：

1. 内部中断高于外部中断。内部中断是同步的，在发出后立刻执行，外部中断不能打断内部中断。
2. 外部中断按照中断号分配优先级，中断号越低的优先级越高。

由于任何时候，一个 CPU 上运行的进程只有一个，因此同时出现的内部中断只有一个，而外部中断并不关心当前执行的进程。另一方面，优先级确保了高优先级的外部中断总会优先执行。

那么，上面问题的正确处理逻辑入下图所示：

```sequence
clock -> io_apic: interrupt
process 1 -> lapic: syscall
note over lapic: 内部中断优先处理
io_apic -> lapic: 内部中断处理结束，开始处理外部中断
note over lapic: 发生进程切换
lapic -> process 2: 启动另一个进程
note over process 2: 进程正确执行
```

#### 中断的完成
在中断完成后，应该由 CPU 向 Local APIC 写入一个 EOI 寄存器 (End Of Interrupt)，代表中断处理完成，并由 Local APIC 决定如何处理这个信息，例如再发送一个中断等等。但是 Hyper OS 中的 Local APIC 不是忙等待的，而是通过等待在事件队列上获取下一个事件。因此在 Hyper OS 中，中断完成后 ISR 会向 Local APIC 发送一个信号事件，表示中断处理完成。Local APIC 通过处理这个信号事件来完成对中断结束的处理。

而 Local APIC 在进行完中断结束后，还需要通知发送中断的线程中断完成。这里有异于 schedule 中的通知，这里通知的一定是发送中断的线程，让该线程获取中断的返回值，并销毁这个 interrupt_t 类的实例，避免内存泄漏。

因此在处理完中断后，还应当有如下过程：

```sequence
ISR -> LAPIC: 发送 EOI 信号模拟写入 EOI 寄存器
LAPIC -> thread which sent the interrupt: 通知中断来源中断结束
note over thread which sent the interrupt: 获取中断结果，回收清理内存
```

#### 中断嵌套

Hyper OS 中断控制器通过实现一个中断栈，支持了中断的嵌套。例如在打印字符的系统调用中调用设备接口，结果设备不存在发出一个中断，中断控制器可以继续调用相应的中断处理例程进行处理。

这里的中断只有在收到的是一个内部中断，且目前中断栈上也是内部中断的时候才会嵌套。因为此情况一定为当前拥有 CPU 锁的代码发出这一中断，而由于目前中断栈上有内部中断在处理，因此一定为这个中断处理例程发出。即为出现了在中断处理例程中出现中断的情况，需要进行嵌套处理。

### 用户态程序和进程

#### 用户态程序

Hyper OS 实现的另外一个难点是如何打断用户态进程的执行，进行进程调度和中断处理。为了解决这一点，Hyper OS 约定了用户态程序的格式，具体来说有以下两点：

1. 任何程序是一个继承自 `program` 的子类，要实现两个虚函数：

   - `static_init`：完成静态空间的设置，包括分配静态区 `.data` 和 bss 区 `.bss` 的内存，设置静态区内容等操作。
   - `main`：应用程序的入口，被加载到进程中后，执行其中的代码。

2. 程序必须通过给定的接口 `handle<T>` 来分配和访问内存，Hyper OS 应用程序具有非常简单的内存布局：

   分配内存的方式有四种，分别是：

   - 在静态区分配：在 `static_init` 中使用 `alloc_static<T>()` 分配，使用 `modify_in_compile()` 修改；
   - 在 bss 区分配：在 `static_init` 中使用 `alloc_bss<T>()` 函数分配；
   - 在栈上分配：在  `main` 中使用 `alloc_stack<T>()` 函数分配； 
   - 在堆上分配：在  `main` 中使用 `alloc_heap<T>()` 函数分配，使用 `free_heap` 进行释放。

   由于重载了构造、赋值和类型转化函数，可以直接使用 `handle<T>` 进行运算。`handle` 访存操作会经过 MMU 被翻译到物理地址，利用物理内存的访存指令进行访存。
   
   `handle<T>` 是从应用程序抢夺控制权的基础，通过模拟真实系统，在访存操作结束后检查是否有中断，并利用进程同步操作放弃控制权，实现了操作系统对程序的控制。

#### 进程

进程是正在执行的程序的抽象，包括程序和程序的运行上下文。进程信息由 `process_t` 类管理，包括进程名，进程号，进程状态以及进程所执行的程序。进程上下文主要包括两点：

1. 进程的页表：关于页表的实现将在下一节详细描述。
2. 进程执行到的位置

由于 Hyper OS 是语言层面的，并不能像真实 OS 一样通过记录 `eip` 的方式记录运行的位置。那么，如何打断进程的执行，使得多个进程可以协调工作，就是 Hyper OS 进程设计中的难点。

在 Hyper OS 中，每一个进程都对应真实系统上的一个线程，而处于等待状态的线程将自己阻塞在其拥有的一个条件变量上。事实上，这个条件变量承担了打断和记录执行位置的任务。我们用下图来展示进程切换时 Hyper OS 发生的事件：

```sequence
note over process 1: 发现时钟中断
process 1 -> ISR : 阻塞自己并放弃CPU
note over ISR: 调度器切换到process 2
ISR -> process 2: 给条件变量一个信号
note over process 2: 收到信号并启动继续执行

```

#### 标准库和系统调用

Hyper OS 像应用程序提供了系统调用的接口，并在之上封装了一些常用的函数。

- 系统调用接口是 `program` 类的成员 `sys_t *sys`，应用程序可以使用 `sys->syscall()` 调用对应的系统调用
- 标准库接口是 `program` 类的成员 `hos_std_t *hos_std`，应用程序可以使用 `hos_std->fun()` 调用相应的标准库函数。

因此，Hyper OS 可以完整展现应用程序使用 `print` 输出时的完整过程：

```sequence
application -> hos_std: print('h')
hos_std -> sys: sys_write('h')
sys -> ISR: interrupt #80
ISR -> device: output command
note over ISR: trap exit
ISR -> sys: return 0
sys -> hos_std: return 0
hos_std -> application: return 0
```

#### 程序管理器

由于程序不再是一个文件，而是 `program` 某个子类，因此程序管理器 `src/program/program_manager` 负责根据程序名生成程序的实例。

### 虚拟内存

#### 页表

像真实的系统一样，Hyper OS 具有页机制，进程拥有完整的 4G 虚拟地址空间，线性地址经过 MMU 的翻译称为物理地址，并通过线程间通信进行物理内存的读/写操作。

为了简单起见 Hyper OS 仅使用单级页表，每个进程在其 `process_t::context` 中存有页表，当进程被调度器调度要占用 CPU 执行时，对应 CPU 的页表基址指针 `core::context` 被指向进程的页表，供 MMU 查找。

#### MMU

MMU 的实现在 `src/mmu/mmu.cpp` 中，主要功能是实现地址翻译，将线性地址翻译到物理地址，当地址不存在时触发页错误。由于 MMU 是 CPU 的一部分，其并不运行在一个独立的线程上，而是由当前运行进程所在的线程直接调用并提供服务。当线性地址不合法时，MMU 会不断触发缺页错，直到错误被消除（这模拟 CPU 执行错误处理时，运行处理例程后重新执行产生错误的指令）。

#### 进程内存布局

Hyper OS 中的进程具有很简单的内存布局：

```
---------------- 0x00000000
|    .text     |
|--------------| text_size
|    .data     |
|--------------| text_size + data_size
|     .bss     |
|--------------| text_size + data_size + bss_size
|    .stack    |
|   heap(up)   |  
|              |
|  stack(down) |
|--------------| VM_SIZE
```

在执行 `static_init` 之后，所有指向 `.bss` 的指针需要被重定向到 `data + text + bss` 上。

当进程被使用 `exec_program` 系统调用，加载到内存中执行时，操作系统为 `.data` 区和 `.bss` 区申请页面，将 `.data` 区赋值，将 `.bss` 区清空，并设置堆栈指针。

## 实验演示

### shell

`shell` 是仅使用 `hos` 标准库和标准库编写的简单终端，其中执行程序的逻辑可以大致表述为： 

```C++
read_string(str);
int pid = create_process();
exec_program(pid, str);
wait(pid);
```

其中 `wait` 语句阻塞自己，直到某个子进程 `pid` 执行结束，完成回收后继续执行。尝试在 `shell` 中启动另一个 `shell`，仍可以正常工作；若删去 `wait` 语句，两个中断的打印则交错出现，程序无法正常工作。

调用程序 `lp`，可以答应所有加载的程序。

### elephant

`elephant` 输出字符串 `Hyper OS` 的字符画：

```
--------------------------------------------------------------------
    //    / /                                      //   ) ) //   )  
   //___ / /         ___      ___      __         //   / / ((       
  / ___   //   / / //   ) ) //___) ) //  ) )     //   / /    \     
 //    / ((___/ / //___/ / //       //          //   / /       ))   
//    / /    / / //       ((____   //          ((___/ / ((___ //    
--------------------------------------------------------------------
```

如果在程序末尾加入指令：

```c++
handle<int> pid = sys->create_process();
sys->exec_program();
while (1) {
    hos_std->println("tick");
    sys->yield();
}
```

可以观察到，每隔一段时间，一个新的进程被创建出来并打印一个字符画；每当一个进程被创建出来，打印 `tick` 的个数就加 1。这说明 `yield` 成功地释放了 CPU 使用权。由于没有实现同步互斥，可能存在字符画打印一半后出现一个 `tick`，这也符合预测。

### demo-interrupt

`demo-interrupt` 展示了中断处理相关的处理过程。这个例程分为三个部分。我们可以通过这三个部分的例子完整看出系统处理不同中断的时候的过程。

#### 内部中断
第一部分中例程向系统发送了 DOUBLE INTERRUPT TEST 中断。这个中断的中断处理例程会发送 INTERRUPT TEST 中断，而 INTERRUPT TEST 中断为一个简单的测试用中断。因此这个过程展示了内部中断嵌套的情况。这部分的过程如下：

```
hd Local APIC (0)    : Sending an interrupt request to LAPIC on core #0 : id : 30
hd Local APIC (0)    : LAPIC received new interrupt request : id : 30
hd Local APIC (0)    : Local APIC scheduled ISR of interrupt to be run : id : 30, this is #0 ISR on stack
kern trap (0)        : (switch to kernel mode) trap entry of interrupt : id : 30
hd Local APIC (0)    : Sending an interrupt request to LAPIC on core #0 : id : 31
hd Local APIC (0)    : LAPIC received new interrupt request : id : 31
hd Local APIC (0)    : Local APIC scheduled ISR of interrupt to be run : id : 31, this is #1 ISR on stack
kern trap (0)        : (switch to kernel mode) trap entry of interrupt : id : 31
kern trap (0)        : (switch to user mode) trap exit, restore context of current process
hd Local APIC (0)    : Sending EOI to LAPIC
hd Local APIC (0)    : LAPIC received EOI from ISR : id : 31, return the interrupt
kern trap (0)        : (switch to user mode) trap exit, restore context of current process
hd Local APIC (0)    : Sending EOI to LAPIC
hd Local APIC (0)    : LAPIC received EOI from ISR : id : 30, return the interrupt
```

可以发现中断从 LAPIC 一路传到 CPU，并执行中断处理例程，并在中断处理例程中再次发送中断，产生嵌套。在中断结束后，通过向 LAPIC 发送 EOI 返回中断。

#### 系统调用
第二部分展示了最简单的系统调用：打印一个字符。这部分过程如下：

```
user stdlib (0)      : call function 'println' in standard library
user syscall (0)     : syscall function trigger INTR #80 using 'INT 80' instruction
hd Local APIC (0)    : Sending an interrupt request to LAPIC on core #0 : id : 80
hd Local APIC (0)    : LAPIC received new interrupt request : id : 80
hd Local APIC (0)    : Local APIC scheduled ISR of interrupt to be run : id : 80, this is #0 ISR on stack
kern trap (0)        : (switch to kernel mode) trap entry of interrupt : id : 80
kern syscall (0)     : Interrupt Service Routine start to serve syscall
kern trap (0)        : (switch to user mode) trap exit, restore context of current process
hd Local APIC (0)    : Sending EOI to LAPIC
hd Local APIC (0)    : LAPIC received EOI from ISR : id : 80, return the interrupt
hd Local APIC (0)    : Sending an interrupt request to LAPIC on core #0 : #PF : P--
hd Local APIC (0)    : LAPIC received new interrupt request : #PF : P--
hd Local APIC (0)    : Local APIC scheduled ISR of interrupt to be run : #PF : P--, this is #0 ISR on stack
kern trap (0)        : (switch to kernel mode) trap entry of interrupt : #PF : P--
kern trap (0)        : (switch to user mode) trap exit, restore context of current process
hd Local APIC (0)    : Sending EOI to LAPIC
hd Local APIC (0)    : LAPIC received EOI from ISR : #PF : P--, return the interrupt
// 这个中断是由于在栈上申请了空间传递参数，导致的缺页
user syscall (0)     : syscall function trigger INTR #80 using 'INT 80' instruction
hd Local APIC (0)    : Sending an interrupt request to LAPIC on core #0 : id : 80
hd Local APIC (0)    : LAPIC received new interrupt request : id : 80
hd Local APIC (0)    : Local APIC scheduled ISR of interrupt to be run : id : 80, this is #0 ISR on stack
kern trap (0)        : (switch to kernel mode) trap entry of interrupt : id : 80
kern syscall (0)     : Interrupt Service Routine start to serve syscall
kern trap (0)        : (switch to user mode) trap exit, restore context of current process
hd Local APIC (0)    : Sending EOI to LAPIC
hd Local APIC (0)    : LAPIC received EOI from ISR : id : 80, return the interrupt
```

#### 外部中断
第三部分展示了时钟中断的过程。这部分由 dev clock 定时发出，并通过 I/O APIC 传递到 LAPIC，过程如下：

```
dev clock            : Device clock sending a tick
hd I/O APIC          : Sending an interrupt request to I/O APIC : id : 32
hd I/O APIC          : I/O APIC received interrupt request : id : 32, sending to core #0
hd Local APIC (0)    : Sending an interrupt request to LAPIC on core #0 : id : 32
hd Local APIC (0)    : LAPIC received new interrupt request : id : 32
hd Local APIC (0)    : Local APIC scheduled ISR of interrupt to be run : id : 32, this is #0 ISR on stack
kern trap (0)        : (switch to kernel mode) trap entry of interrupt : id : 32
kern trap (0)        : (switch to user mode) trap exit, restore context of current process
hd Local APIC (0)    : Sending EOI to LAPIC
hd Local APIC (0)    : LAPIC received EOI from ISR : id : 32, return the interrupt
```

可以发现中断 device $\to$ I/O APIC $\to$ Local APIC $\to$ CPU 的过程，并在 I/O APIC 到 Local APIC 的过程中决定被发送到哪个核的 Local APIC。

### demo-pf

`demo-pf` 是缺页处理的例子，Hyper OS 不会为堆栈预先分配空间，而是按需分配，因此在堆栈上申请内存会导致缺页错并开始处理。

```
user program (0)     : write operation in stack
hd MMU (0)           : memory write operation at linear address : 2147483647
hd MMU (0)           : (write) mmu check fail, page is not present, unavailable for user and unwritable
kern ISR(#PF) (0)    : swap-in or create page for linear address 2147483647, page 524287 is now in physical page frame [12288, 16384)
hd MMU (0)           : page fault ISR finish, run the memory access instruction again
hd MMU (0)           : (write) mmu check pass
hd MMU (0)           : MMU translate linear address : 2147483647 to physical address (page frame address)12288 + (offset)4095 = 16383
```

值得演示的例子是，如果再一次调用 `demo-pf`，最后一行算出的物理地址不会变化，这是由于上一次调用退出后其占用的页面被释放；如果调用 `shell` 再调用 `demo-pf`，物理地址则会发生变化。

### demo-proc

`demo-proc` 是进程调度的基本实例，其基本逻辑如下：

```
int pid = create_process();
exec_program(pid, "elephant");
wait(pid);
println("elephant");
```

输出的消息如下：

```
kern scheduler (0)   : process <default>(pid = 6) is created, state : uninit
kern scheduler (0)   : process elephant(pid = 6) is assigned to CPU #0
kern scheduler (0)   : process elephant(pid = 6) state change : uninit --> runable
kern scheduler (0)   : time slice of process demo-proc(pid = 5) is running out, switch to process elephant(pid = 6)
kern scheduler (0)   : time slice of process elephant(pid = 6) is running out, switch to process demo-proc(pid = 5)
kern scheduler (0)   : process demo-proc(pid = 5) will wait process 6
kern scheduler (0)   : process demo-proc(pid = 5) state change : runable --> sleeping
kern scheduler (0)   : time slice of process <null> is running out, switch to process elephant(pid = 6)
--------------------------------------------------------------------
    //    / /                                      //   ) ) //   )  
   //___ / /         ___      ___      __         //   / / ((       
  / ___   //   / / //   ) ) //___) ) //  ) )     //   / /    \     
 //    / ((___/ / //___/ / //       //          //   / /       ))   
//    / /    / / //       ((____   //          ((___/ / ((___ //    
--------------------------------------------------------------------
kern scheduler (0)   : process elephant(pid = 6) exit
kern scheduler (0)   : process elephant(pid = 6) exit, waiting process demo-proc(pid = 5) will be waken up
kern scheduler (0)   : process demo-proc(pid = 5) state change : sleeping --> runable
kern scheduler (0)   : time slice of process <null> is running out, switch to process demo-proc(pid = 5)
elephant
```

可以看到，在这次调用中，shell, demo-proc, elephant 三个进程的生命周期变化如下图所示。

```sequence
shell -> demo_proc: exec_program
note over shell: (wait)runable->sleeping
note over demo_proc: uninit->runable
demo_proc -> elephant: exec_program
note over demo_proc: (wait)runable->sleeping
note over elephant: uninit->runable
note over elephant: (exit)runable->zombie
elephant -> demo_proc: (wait finish)
note over demo_proc: sleeping->runable
note over demo_proc: (exit)runable->zombie
demo_proc -> shell: (wait finish)
note over shell: sleeping->runable
```

### matrix

`matrix` 是矩阵乘法的例子，用于测试利用 `handle` 访问内存使用二维数组（`handle<handle<int>>`）。在这样的例子中，二维数组事实上是指向一维数组指针的数组。

## 实验总结

#### 实验结论

Hyper OS 验证了在用户态利用线程间同步，模拟操作系统逻辑的可行性。Hyper OS 以软件模拟方式展示了操作系统调度进程协同工作的基本方法，在更抽象的层次上说明了 OS 的基本原理。我们认为，完整实现 Hyper OS 并解决遇到的问题大大加深了我们对中断机制、虚拟内存、进程调度的理解和认识，将实现 Hyper OS 这样的模拟平台作为学习 OS 的途径是行之有效的。

另外，Hyper OS 也可以作为分析 OS 中实现的实验平台。由于逻辑简单、与平台无关的同时保证了一定的模拟程度和系统完整度，在 Hyper OS 上做实验是方便而有效的。

#### 后续工作计划

由于现在 Hyper OS 还相对简陋，我们仍然希望在 16 周最终答辩之前进行更多的工作。

- 实现更加方便的 screen。优化用户体验的同时，也更好模拟了输入输出设备与 OS 的协同工作；
- 实现简易的文件系统。文件系统是 OS 重要的知识点，实现文件系统会让 Hyper OS 的完整性大大增强。
- 编写更详细的文档。由于 Hyper OS 为了模拟硬件使用了一些特殊的同步技巧，有必要编写文档来提升代码的可读性。

#### 未来展望

我们认为，对于硬件基础比较差（如外系对 OS 感兴趣的同学，今后的高三同学或本科低年级同学）或没有时间独立完成 OS 编写的同学，相比于完成 ucore 实验来说，完整的实现类似 Hyper OS 这样的模拟环境会对理解 OS 原理更有帮助。如果能够将任务划分更精细，配合更加详细的说明文档，也许可以作为替代 ucore lab 的一种新的实验选项。

## 参考文献

1. Remzi H. Arpaci-Dusseau and Andrea C. Arpaci-Dusseau. Operating Systems: Three Easy Pieces. 2018. 
2. Randal E. Bryant and David R. O'Hallaron. Computer Systems: A Programmer's Perspective. 2016.
