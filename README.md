# Neotrellis OSC Exploration

There's lots of cool grid things (like [Monome Grid](https://monome.org/docs/grid/)) used for various music projects, a natural thing to try for the Neotrellis. Here we are sending [OSC (Open Sound Control)](https://en.wikipedia.org/wiki/Open_Sound_Control) commands meant for [Zynaddsubfx](https://zynaddsubfx.sourceforge.io/zyn-ports/).

You'll need to get your Zynaddsubfx all set up, and then start it with a specific port like `zynaddsubfx -P 8888`. The synth does all the work of making actual sound, this sketch lights up buttons and sends the messages.

Things this uses:
* [Neotrellis 8x8 kit](https://www.adafruit.com/product/1929)
* [Adafruit HUZZAH32 – ESP32 Feather Board](https://www.adafruit.com/product/3405)
* [OSC Arduino Library](https://github.com/CNMAT/OSC)

Future exploration:
* Record/playback a loop
* Hook into a sampler instead of zynaddsubfx
* Set up a step sequencer

