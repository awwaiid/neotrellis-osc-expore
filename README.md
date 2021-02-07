# Neotrellis OSC Exploration

There's lots of cool grid things (like monome) used for various music projects, a natural thing to try for the Neotrellis. Here we are sending OSC commands meant for Zynaddsubfx.

You'll need to get your Zynaddsubfx all set up, and then start it with a specific port like `zynaddsubfx -P 8888`. The synth does all the work of making actual sound, this sketch lights up buttons and sends the messages.

Future exploration:
* Record/playback a loop
* Hook into a sampler instead of zynaddsubfx
* Set up a step sequencer
