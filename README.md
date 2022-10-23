
# Zombie Slayer Simulator

2022 Spring Semester System Programming Lecture Homework

## Overview

You and some of your friends want to clear a building from the zombie invasion. However, you only have 3 weapons, so **only 3 people can kill zombies**.

After some thought, you decide to implement a plan like this: 
In order to clear a room with multiple entrances from zombies, each friend of yours keeps watch at a door **(each a doorman)**, takes the zombies in one by one and keeps the number of zombies taken in.
Since you have 3 weapons in your hand, you and your 2 friends **(slayer)** kill the zombies that enter. And you keep count of how many zombies you have killed in total and how many zombies each slayer has killed. 
 - **If the number of zombies in the room is 100 or more then everyone dies.**
- **If you kill 100 zombies in total, then you and your friends are safe.**

The purpose of this assignment is to find the maximum number of doormen you can have so that you and your friends can kill 100 zombies without dying.

## Details

You are requested to write a multi-thread program that simulates the zombie removal plan above. In this program, you need to create 3 slayer threads and a doorman thread for each of your friends who control the door. And you need to implement the following functions with thread synchronization:

* **``zombieEntered()``**: Keeps track of number of zombies entered
* **``zombiesKilled()``**: Keeps track of number of zombies killed
* **``tooManyZombiesInTheRoom()``**: Returns true, If the number of zombies in the room are greater than or equal to 100
* **``killed100Zombies()``**: Returns true, If more than 100 zombies have been killed
* **``zombiesExist()``**: Returns true, If there are at least one zombie in the room
* **``getKilledCount()``**: Returns the number of zombies killed
* **``getInTheRoomCount()``**: Returns the number of zombies in the room
* **``void *doorMan(void *)``**: Each doorman thread has a 50% chance of allowing a zombie to enter every 2ms. When the zombie enters, you need to update the zombie_counter by calling the necessary functions.If there are too many zombies, that is 100 or more, the doorman thread stops running.
* **``void *slayer(void *)``**: Slayer thread kills a zombie every 2ms. It updates the number of zombie_killed_counter and zombie_counter by calling the appropriate functions. If there are too many zombies, this thread ends its work.
* **``int main()``**: This function creates n Doorman threads (**n command line argument**) and 3 slayer threads. When the execution of all threads is finished whether 100 zombies were killed or not and it prints how many zombies each slayer has killed

## To Build and Run

You need linux environment and the packages.
> In Debian/Ubuntu, To install the packages type  ``sudo apt install build-essential`` 

Then change directory to the folder where you download this project and type ``make`` then ``./zombieSlayer [number of doorman]`` and  you're ready.

