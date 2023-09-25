# Rhythm Game in C Language
[![Video](http://img.youtube.com/vi/flk778sG75g/0.jpg)](https://www.youtube.com/watch?v=flk778sG75g)

---

# Introduction
> This is a rhythm game I created as a 1st-year term project assignment.

While creating the rhythm game itself didn't take too long, I remember struggling to sync everything properly.

---

# How to Play

Use the keys D, F, J, and K to input notes.

If you input the notes accurately, you'll receive an "Excellent" judgment. If you input them a bit slowly, you'll get a "Good" judgment.

Inputting notes continuously will stack combos.

Your score accumulates based on your combos and judgments. The score calculation formula is as follows:
```c
score += (combo / 10 + 1) * 50
```

In the settings menu, you can enable or disable two options:

- Turning on "Note Recording Mode" activates the editor mode. The song will play, but notes won't descend. Instead, when you input notes to match the song, they get saved in the 'note.txt' file.

- Enabling "Debugging Mode" activates debugging. You can check the screen refresh delay.

---

# Explanation
- The simple text animation at the beginning is implemented using a for loop. I stored the text in a two-dimensional char array and repeated the process of printing and erasing one line at a time.

- When you choose a song, the notes are received from the text file where they're stored as a char array buffer. 0 indicates no note, and 1 indicates a note.

- The songs are played using the `PlaySound` function.

- The notes change the text color and are implemented to appear like rhythm game notes by outputting spaces. The `SetConsoleTextAttribute` function is used to change the text's color.

- The delay for each screen is adjusted according to the song's BPM. To ensure that notes are displayed quickly, there's inevitably some deviation in the delay. I measured time at the beginning and end of the loop and adjusted the delay uniformly.

- Double buffering is applied, where only the part where the front buffer and back buffer switch is cleared with spaces before outputting.

- Key input is received using the `GetAsyncKeyState` function. For multithreading, the `_beginthreadex` function is used, allowing multiple keys to be input simultaneously.

- When a note enters the judgment line, it distinguishes note input judgments by comparing the screen buffer and key input status. It also differentiates between long notes and short notes by comparing the front and back buffer states. If you press the note correctly, it's "Excellent"; if you press it quickly, it's "Good"; if you miss it, it's "Miss." The `key_state` variable is created to prevent repeatedly holding down keys to gain points.

- The game keeps track of Excellent, Good, and Miss judgments during gameplay. Based on the ratio of these judgments, it assigns a rank. When the game ends, it displays the judgments and draws a graph to show your performance visually.

---

# We recommend changing console properties and playing the game with the following settings:

- Options - Check "Use Legacy Console"
- Font - Size 10 x 22
- Font - Raster Fonts
- Layout - Screen Buffer Size: Width 100, Height 40
- Layout - Window Size: Width 100, Height 40

If you have any questions or need further assistance, feel free to ask.
