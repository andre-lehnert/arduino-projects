# Version 1:
# Press "1"-"9" to choose a stepper motor
# Press "Up"/ "Down" to accelerate the choosen stepper (default stepper: 1)
# Press "q" to Quit

# Version 2:
# Press "f" to activate the full length mode (14 rounds, 12 mm pitch)

import serial
import time
import turtle

# Number of Steppers drawn on screen
maxSteppers = 9
# Number of rounds
numberOfRounds = 14

ser = serial.Serial('/dev/ttyACM0', 9600)
stepper = 0
mode = "single"

turtle.setup(400,400) 
wn = turtle.Screen()  
wn.title("Visual Stepper - Serial Control")  
wn.bgcolor("white")  

tlist = list()
colorlist = ["red", "green", "black", "blue", "brown", "yellow", "grey", "cyan", "magenta", "orange"]
for i in xrange(maxSteppers):
    tlist.append(turtle.Turtle(shape="square"))
    tlist[i].color(colorlist[i%10])
    tlist[i].penup()
    tlist[i].speed(10)    
    tlist[i].setpos(-150.0,0.0)
    tlist[i].forward((i*25))
    tlist[i].left(90)
    tlist[i].pendown()
    


def stepper1():
   global stepper
   stepper = 0
   tlist[stepper].shape("arrow")
   for i in xrange(maxSteppers):
      if i == stepper:
         tlist[i].shape("arrow")
      else:
         if mode == "s": # single mode
            tlist[i].shape("square")
         else:
            tlist[i].shape("circle")

def stepper2():
   global stepper
   stepper = 1   
   for i in xrange(maxSteppers):
      if i == stepper:
         tlist[i].shape("arrow")
      else:
         if mode == "s": # single mode
            tlist[i].shape("square")
         else:
            tlist[i].shape("circle")

def stepper3():
   global stepper
   stepper = 2
   for i in xrange(maxSteppers):
      if i == stepper:
         tlist[i].shape("arrow")
      else:
         if mode == "s": # single mode
            tlist[i].shape("square")
         else:
            tlist[i].shape("circle")

def stepper4():
   global stepper
   stepper = 3
   for i in xrange(maxSteppers):
      if i == stepper:
         tlist[i].shape("arrow")
      else:
         if mode == "s": # single mode
            tlist[i].shape("square")
         else:
            tlist[i].shape("circle")

def stepper5():
   global stepper
   stepper = 4
   for i in xrange(maxSteppers):
      if i == stepper:
         tlist[i].shape("arrow")
      else:
         if mode == "s": # single mode
            tlist[i].shape("square")
         else:
            tlist[i].shape("circle")

def stepper6():
   global stepper
   stepper = 5
   for i in xrange(maxSteppers):
      if i == stepper:
         tlist[i].shape("arrow")
      else:
         if mode == "s": # single mode
            tlist[i].shape("square")
         else:
            tlist[i].shape("circle")

def stepper7():
   global stepper
   stepper = 6
   for i in xrange(maxSteppers):
      if i == stepper:
         tlist[i].shape("arrow")
      else:
         if mode == "s": # single mode
            tlist[i].shape("square")
         else:
            tlist[i].shape("circle")

def stepper8():
   global stepper
   stepper = 7
   for i in xrange(maxSteppers):
      if i == stepper:
         tlist[i].shape("arrow")
      else:
         if mode == "s": # single mode
            tlist[i].shape("square")
         else:
            tlist[i].shape("circle")

def stepper9():
   global stepper
   stepper = 8
   for i in xrange(maxSteppers):
      if i == stepper:
         tlist[i].shape("arrow")
      else:
         if mode == "s": # single mode
            tlist[i].shape("square")
         else:
            tlist[i].shape("circle")

def up():
   global numberOfRounds
   ser.write(str(stepper) + ';u;' + mode) # up
   print ser.readline()	   
   moveUp = True
   if mode == "s": # single mode
      tlist[stepper].forward(10);
   else: 
      tlist[stepper].forward(10*numberOfRounds);

def down():
   global numberOfRounds
   ser.write(str(stepper) + ';d;' + mode) # down	
   print ser.readline()   
   if mode == "s": # single mode
      tlist[stepper].backward(10);
   else: 
      tlist[stepper].backward(10*numberOfRounds);

def checkSerial():
   print ser.readline() 

# do 14 rounds (12x12)
def fullLengthMode():
   global mode
   if mode == "s": # single mode
      mode = "f" # full length mode
      for i in xrange(maxSteppers):          
         tlist[i].shape("circle")
     
   else:
      mode = "s" # single mode
      for i in xrange(maxSteppers):          
         tlist[i].shape("square")

def close():
    wn.bye()                    

# These lines "wire up" keypresses to the handlers we've defined.
wn.onkey(up, "Up")
wn.onkey(down, "Down")
wn.onkey(stepper1, "1")
wn.onkey(stepper2, "2")
wn.onkey(stepper3, "3")
wn.onkey(stepper4, "4")
wn.onkey(stepper5, "5")
wn.onkey(stepper6, "6")
wn.onkey(stepper7, "7")
wn.onkey(stepper8, "8")
wn.onkey(stepper9, "9")
wn.onkey(fullLengthMode, "f")
wn.onkey(close, "q")


wn.listen()
turtle.mainloop()



