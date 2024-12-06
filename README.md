**Email for TA**
Trevor Jacobs: jacobstr@msu.edu

**Morphing**

The program can perform image morphing, where two images are blended based on an alpha value. This blending can create a gradual transition between the two images, showing an effect of one image morphing into the other.

**Wave Warp Effect**

The ApplyWaveEffect function adds a wave distortion to the image. The effect distorts the pixels based on sine and cosine functions, creating a wavy, fluid-like appearance.
The effect can be toggled on or off via a menu option, and it works by shifting pixel positions in both X and Y directions to create the wave effect.

**Green Screen and Blue Screen**

The DrawImage function includes a CChromakey object that applies chroma keying (green or blue screen effect). If enabled, the image is checked for green or blue pixels and replaces them with a background image.
The program allows the user to select a background image (such as background.png) and apply it to the areas where the green or blue screen is detected.

**Recoloring (RecolorRedToBlue)**

The RecolorRedToBlue function selectively changes the red pixels in an image to blue. It identifies pixels that fall within a certain red range and replaces them by decreasing the red component and increasing the blue component. This effect is applied when the "Recolor" mode is activated. We chose to use this specifically for the tie and for the firework so that we can show that we have control of the color, even when it isnt that way in the original video.

**Fireworks Effect**

The DrawFireworks function is where a firework that first starts at the bottom of the screen and shoots up towards the top of the screen to make it look like a firework.

