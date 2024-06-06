# SHIMMER_EFFECT
In this project we will explore and implement a basic shimmer reverb effect. This effect has been used by a lot of artists such as Brian Eno, Daniel Lanois, U2, etc... The result of this effect is a very long decay, big reverb effect that can transform a dry guitar sound to an angelic, ambient pad (for example). 

You can check [here](https://youtu.be/KI7by4NOwV0?si=jw2CNXyi32flTsZP) a famous example of this effect used by U2 on the song *"4th of July"*.

There's obviously many ways to get this effect, but the basic fondation of the shimmer is a combination of pitch-shifting, long decay reverb and for most of the time, a feedback loop. 

Too choose a design for our a shimmer, we can take as example the very powerful plugin **ValhallaShimmer** made by ValhallaDSP and try to in a way reverse-engineer the effect : 

<img width="800" style="float: center; margin: 10px 10px 0px 85px;" src="ValhallaShimmerGUI-960x338.jpg">

>*Here we can see the GUI of the effect composed by the different parameters that we can change to control the effect, [here is the list](https://valhalladsp.com/2010/11/27/valhallashimmer-the-controls/) of all the differents controls parameters*

The project will then consists of two big parts : 
1. Creating a Proof-Of-Concept (in a jupyter notebook) of the effect by choosing the design, the implementation of the differents processing blocks, the parameters of the design, etc...
2. Implementing the effect with C to be able to use it in real-time
   
Our chance is that the founder of ValhallaDSP, Sean Costello, offers a variety of posts about the designs of his plugins on the [main page](https://valhalladsp.com/category/learn/plugin-design-learn/) of the company. We have then a lot of insights to help us during the design process.

## HOW TO USE THE JUPYTER NOTEBOOK
The notebook represent the research process of this project. We start from scratch and try to understand how a shimmer works by exploring each components. You can either read it or run it if you have jupyter and the required libraries installed on you machine. 

## HOW TO USE THE C IMPLEMENTATION
The C implementation has been built as a **windows** application, so it's not compatible with other operating systems. The libraries are linked statically, so you don't need anything to run the .exe file.

If you want to play more with the effect and build your own version of the project, don't hesitate to adapt the *MAKEFILE* and all the templates variables in the file *utils.h*. 

The application offers two mode : 

1. *Play-back* : This mode just take and mono 32-bit float .wav file and apply the effect to 32-bit float .wav output file. It's useful if you want to test the effect without connecting a microphone or speaker.
   
   It also offers the possibility to emulate a stereo effect by sending a delayed version of the mono channel on the left channel of the output file. 
   You can add your own files in the *./audio* folder.

4. *Real-time* : This mode apply the effect as a real-time process. [Portaudio library](https://www.portaudio.com) is used to get data from given input and output channels on the machine.
   
   By running *shimmer.exe* you can use the real-time process with the WMME host API. It will then take the default input and output device of your computer as input and output channel of the effect. Sadly, the WMME is not very efficient and you will ear a lot of delay between input and output. 
   
   Because it's a real-time process, you may ear some underruns during the process, depending of the performance of your machine. Don't hesitate to adapt the buffer size to evict the problem (a bigger buffer size induce more delay between input and output). 
   
   If you have an ASIO compatible audio interface, you can also find the executable *shimmer_asio.exe* which use ASIO as host API. ASIO is much more efficient, you will be able to ear the effect with almost no delay. 
   
   To build the ASIO version of the effect, you just have to adapt the *ASIO* flag in the *MAKEFILE*

> NOTE : If you're executing the program on the native Windows console, you will probably not see thrown error, the program will just crash (for example, if you did not connected any ASIO interface when using *shimmer_asio.exe*). 
