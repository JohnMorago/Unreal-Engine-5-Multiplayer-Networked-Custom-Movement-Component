# Unreal-Engine-5-Multiplayer-Networked-Custom-Movement-Component

This repository contains a template for a basic Movement Component and Character written in C++ that allows the Character to sprint in multiplayer sessions with proper networking so that it still functions well even on high ping.

The default Movement Component in Unreal Engine 5 supports multiplayer networked walking and jumping however it does not support sprinting.

This template inherits all of the functionality of the default Movement Component, all it does is also support sprinting.

Properly implementing multiplayer networked sprinting can be difficult so this custom Movement Component is a good place to start if you know your game will be Multiplayer and include sprinting.

To use this template, just replace "MyCharacterMovementComponent" with whatever name you'd like to give your custom Movement Component.  Replace "SphereCharacter" with the name of the Character that you are using.  And finally, replace "SPHERECUBED" with your project's name.

The C++ classes should be fully setup, there's just a few last things to do before you have a fully functional character.

Create a Blueprint in Unreal Engine and select the Parent class to be your Character C++ Class you made.  The C++ class handles all the functionality but the Blueprint is where we will select a visible Mesh and Input mappings.

Then, you'll need to create an InputMappingContext in Unreal Engine to setup the keybinds as well as InputActions for Jump, Move, Look, and Sprint.  You can assign these to your Character in the Character's Blueprint. Select the (self) component inside your Character's Blueprint and search for the 'input' section. You should see drop downs where you can select the InputMappingContext and InputActions you've created and assign them to the character.

This next seciton will go over the settings for the InputMappingContext and InputActions.

InputActions Settings:  

Jump -> Value Type: Digital (bool)  
Move -> Value Type: Axis2D (Vector2D)  
Look -> Value Type: Axis2D (Vector2D)  
Sprint -> Value Type: Digital (bool)  


InputMappingContext Settings:  

Add mappings to the Default Key Mappings tab. Select your InputActions via the drop down.  
The following settings are set up INSIDE the InputMappingContext, NOT inside each individual InputAction.  
The only information provided by the InputAction is it's Value Type.  

InputAction_Jump    
Space Bar  

InputAction_Move    
W - Modifiers: Swizzle Input Axis Values  
A - Modifiers: Negate  
S - Modifiers: Swizzle Input Axis Values | Negate  
D  

InputAction_Look    
Mouse X  
Mouse Y - Modifiers: Swizzle Input Axis Values | Negate  

InputAction_Sprint    
Left Shift  


After you've set up these Input Actions and your InputMappingContext and attatched them to the Character via the Blueprint editor, you should be good to go!  

I hope this repository proves helpful!  Have fun coding your Multiplayer game in Unreal Engine!  

-- JrMJr
