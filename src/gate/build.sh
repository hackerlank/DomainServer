#!/usr/bin/bash

g++ GateTest.cpp gate.cpp ../threadpool/threadpool.cpp -I ../threadpool -I ../logger -I ../hub -pthread --std=c++11
