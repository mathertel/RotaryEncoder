# BounceRotaryEncoder library

This is a fork of the RotaryEncoder library by @mathertel which includes the debouncing functionallity for the push button.
The debounce functionallity is based on the original Bounce library by Thomas Frederick.

* Original RotaryEncoder library: https://github.com/mathertel/RotaryEncoder
* Original Bounce library: https://playground.arduino.cc/Code/Bounce/

# Why?

If you are working on a project with multiple push buttons, it makes sense to use any other library to manage the debouncing of said buttons. However if you are working on a small project in which you only need the push button of the encoder, you can use this library to manage that.

# How it works

The best way to see how it works is to install the library and have a look at the examples. They are quite stright forward.
The fork library is retro-compatible with the original RotaryEncoder library.
