# Unreal-Engine-5-Multiplayer-Networked-Custom-Movement-Component

This repository contains a template for a basic Movement Component and Character written in C++ that allows the Character to sprint in multiplayer sessions with proper networking so that it still functions well even on high ping.

The default Movement Component in Unreal Engine 5 supports multiplayer networked walking and jumping however it does not support sprinting.

This template inherits all of the functionality of the default Movement Component, all it does is also support sprinting.

Properly implementing multiplayer networked sprinting can be difficult so this custom Movement Component is a good place to start if you know your game will be Multiplayer and include sprinting.

To use this template, just replace "MyCharacterMovementComponent" with whatever name you'd like to give your custom Movement Component.  Replace "SphereCharacter" with the name of the Character that you are using.  And finally, replace "SPHERECUBED" with your project's name.

I wanted a Multiplayer game where my Character could sprint, this took me a while to figure out and get working, hopefully people who run into this same hill can use this to continue developing their game without spending as much time figuring out this solution.

I hope this repository proves helpful!

-- JrMJr
