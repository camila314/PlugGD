# PlugGD

Plug-in system and python interpreter embedded into the game Geometry Dash, designed for helping people automate tasks for creating. Allows the user to run plug-ins by using customizable keybinds. Works for both mac and windows.

## Example Script

```python
e = Editor()

for i in range(180):
	e.selection.rotate(2)
	e.selection.incrementZ() # stops visual glitches, increments Z order
	e.duplicate()

```

## Documentation

comig soon

## License
This project is licensed with GPLv2 (i'm sorry ik, gpl bad mit good). Don't take it personal just, I put a lot of work in and don't want it to go unrecognized by means of someone else embedding it with no credit into a more popular program.
