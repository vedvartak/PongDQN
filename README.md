# PongDQN
## Introduction
Experimental implementaion of the Double DQN algorithm in C++ from scratch to train it on custom Pong environment. DQN agent learns to play Pong by playing against computer opponents using inputs from RAM. Pong environment is rendered using OpenGL with Win32 API for window and context creation. DQN library uses a simple neural network library implimented from scratch, a custom math library supporting vectors and matrices.

DQN agent learns to hold its own against chaser AI even if max vertical speed of paddle is less than max vertical speed of ball in about 400 episodes (approx 1 min on i5 11th gen at 3 GHz).
## Results
<table>
  <tr> <td><img src="images/training_graph.png"/></td>
  <td><img src="images/test_graph.png"/></td>
  </tr>
  <tr><th>Training Log</th><th>Testing Log</th></tr>
</table>

## Modules
This repository contains 6 libraries that are implemented from scratch in C++:
<ol type="i">
  <li>Mathlib:</li>
  A simple math library containing implementation of vectors and matrix which is used to implement neural networks.
  <li>Neurallib:</li>
  A simple neural network library containing implementation of deep neural networks, activation functions and loss functions which is used to implement DQN agent.
  <li>Gymlab:</li>
  A simple gym environment library for RL agents containing implementation of PONG environment which is used alongside DQN agent to train it.
  <li>Uilib:</li>
  A simple UI library(windows) for creating windows which can be used for rendering.
  <li>Renderlib:</li>
  A simple rendering library for rendering gym environment for human understanding. It provides way to draw simple 2d shapes using OpenGL API.
  <li>Rllib:</li>
  A simple reinforcement learning library containing implementation of DQN algorithm.
</ol>

## Screenshots

<table>
  <tr> <td align="center"><img src="images/pong_env.png" width="50%" height="50%" /></td></tr>
  <tr><th>Pong environment</th></tr>
  <tr> <td align="center"><img src="images/training_log.png" width="50%" height="50%"/></td></tr>
  <tr><th>Training log</th></tr>
</table>
