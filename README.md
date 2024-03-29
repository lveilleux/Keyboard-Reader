###Keyboard-Reader  
An application built for Razer Chroma (works only with Razer keyboards) that will take in a text file, and print out the contents of the file onto the keyboard, lighting up the key corresponding to each letter in the word one by one, before going onto the next word.  
  
_This program was built for my English 215 - Text and Code class in the Fall of 2016 at RIT._
  
**Videos of the program running through the different texts taken from the ENGL-215 course can be found at this link <https://www.youtube.com/playlist?list=PLIkYbqfuemk8V9DQi3i6DoLI8PtenSBNB>**
  
The program can be run, with a Razer keyboard attached by running the .exe in the release folder of this project and giving the program a file name of a text file in the text/ folder within release/.  
  
For my final project, I am going to be building an application that will take in some text from a given file, into a C++ application that I'd designed to work specifically for keyboards that have and support individual lighting of keys with each key having its own LED (Razer Chroma). The program will take in the text found in a file, and go word by word flashing the letters of each word onto the keyboard. This will be done on either the keyboard on my desktop or laptop which use 3rd party libraries, Razer Chroma SDK, to make this possible. I have a small amount of work with the SDK already so getting this program up and running shouldn’t be a monster task.

The idea behind this is a different way for users to read and in a sense, interact with a text file or web page. This goes back to what Bootz was talking about with making the users work to read the entire piece in the same way that the user must pay attention to the keyboard not the screen to get the text from the source. In class, we often come back to the idea of pieces that change as they go and can only be read once before deleting themselves, like Taroko Gorge or Agrippa, while this program will be the same every time, if run with the same given inputs, there is no way to go back and reread the last line of the program as each word flashes just long enough for the reader to decipher the word, before moving quickly on for the next word to appear.
