# QLLib
#### C++ Library to Learn Behaviours using Off Policy Q-Learning Reinforcement Learning

QLLib is a Reinforcement Learning that implements several common RL algorithms. It is designed to be simple, fast and extremely flexible. In particular, QLLib currently features:
* Temporal Difference Learning (Q-Learning algorithm)
* Greedy, Epsilon-Greedy, Random and SoftMax policies
* Optimizations for running simulations on high-performance servers
* Additional flexibility achieved by giving you the right layer of separation between the core library and your custom simulations
* C++11, header-only, no external dependencies, ...

### Prerequisites
Because QLLib is focused on performance, it uses many C++11 features, so you'll need a modern compiler and standard C++ library. **QLLib requires at least GCC 4.8**. QLLib is developed on GNU/Linux, however it should also work on OS X and Windows. Note however that Windows support is experimental.

It may be useful to point out that QLLib has been specifically designed to run Reinforcement Learning simulations on high-performance machines, as the library focuses on performance. For this reason, QLLib may not be the best choice if you want to run RL simulations on embedded devices, like robots. In that case, a library like RLLib may be more suitable.

### Installation
QLLib is a header-only library, so there is nothing to build for the library itself. The header files are located in the `src` directory, so you simply need to include this directory from your projects to access the library.

### Usage
