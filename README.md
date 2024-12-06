**Email for TA**
Trevor Jacobs: jacobstr@msu.edu
Gregory Lis: lisgrego@msu.edu
Julia Bishop: bisho121@msu.edu
Aidan Baird: bairdaid@msu.edu

**Morphing**

The program can perform image morphing, where two images are blended based on an alpha value. This blending can create a gradual transition between the two images, showing an effect of one image morphing into the other.
This feature blends the Julia and Trevor image together. To use this you click morph in edit, and if you use the mouse mode with Julia, she's more apparent; the mouse mode with Trevor has him more apparent.


**Wave Warp Effect**

The ApplyWaveEffect function adds a wave distortion to the image. The effect distorts the pixels based on sine and cosine functions, creating a wavy, fluid-like appearance.
The effect can be toggled on or off via a menu option, and it works by shifting pixel positions in both X and Y directions to create the wave effect.

**Green Screen and Blue Screen**

The DrawImage function includes a CChromakey object that applies chroma keying (green or blue screen effect). If enabled, the image is checked for green or blue pixels and replaces them with a background image.
The program allows the user to select a background image (such as background.png) and apply it to the areas where the green or blue screen is detected. It also applies an exterior garbage mask to help apply the background to areas along the outside not blue or green.

**Recoloring (RecolorRedToBlue)**

The RecolorRedToBlue function selectively changes the red pixels in an image to blue. It identifies pixels that fall within a certain red range and replaces them by decreasing the red component and increasing the blue component. This effect is applied when the "Recolor" mode is activated. We chose to use this specifically for the tie and for the firework so that we can show that we have control of the color, even when it isnt that way in the original video.

**Fireworks Effect**

The DrawFireworks function is where a firework that first starts at the bottom of the screen and shoots up towards the top of the screen to make it look like a firework.

**Paint Effect**

The Paint effect is an option in mouse mode and essentially acts as a paint brush. You can modify the color and brush size in paint settings in the edit menu

**Movable Arm**

A dynamic, interactive arm that can be repositioned within the scene for animations or effects.. 

**Outlined Drawings**

Create outlined drawings of characters or objects in the video.
These outlines appear as line drawings that are dynamically associated with the elements in the video.


