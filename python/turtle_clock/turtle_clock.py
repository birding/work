# coding=utf-8
 
import turtle
from datetime import *
 
 
# ̧�𻭱ʣ���ǰ�˶�һ�ξ������
def Skip(step):
    turtle.penup()
    turtle.forward(step)
    turtle.pendown()
 
 
def mkHand(name, length):
    # ע��Turtle��״����������Turtle
    turtle.reset()
    Skip(-length * 0.1)
    # ��ʼ��¼����εĶ��㡣��ǰ���ڹ�λ���Ƕ���εĵ�һ�����㡣
    turtle.begin_poly()
    turtle.forward(length * 1.1)
    # ֹͣ��¼����εĶ��㡣��ǰ���ڹ�λ���Ƕ���ε����һ�����㡣�����һ������������
    turtle.end_poly()
    # ��������¼�Ķ���Ρ�
    handForm = turtle.get_poly()
    turtle.register_shape(name, handForm)
 
 
def Init():
    global secHand, minHand, hurHand, printer
    # ����Turtleָ��
    turtle.mode("logo")
    # ������������Turtle����ʼ��
    mkHand("secHand", 135)
    mkHand("minHand", 125)
    mkHand("hurHand", 90)
    secHand = turtle.Turtle()
    secHand.shape("secHand")
    minHand = turtle.Turtle()
    minHand.shape("minHand")
    hurHand = turtle.Turtle()
    hurHand.shape("hurHand")
 
    for hand in secHand, minHand, hurHand:
        hand.shapesize(1, 1, 3)
        hand.speed(0)
 
        # �����������Turtle
    printer = turtle.Turtle()
 
    # ���ػ��ʵ�turtle��״
    printer.hideturtle()
    printer.penup()
 
 
def SetupClock(radius):
    # �����������
    turtle.reset()
    turtle.pensize(7)
    turtle.pencolor("#ff5500")
    turtle.fillcolor("green")
 
    for i in range(60):
        Skip(radius)
        if i % 5 == 0:
            turtle.forward(20)
            Skip(-radius - 20)
 
            Skip(radius + 20)
            if i == 0:
                turtle.write(int(12), align="center", font=("Courier", 14, "bold"))
            elif i == 30:
                Skip(25)
                turtle.write(int(i / 5), align="center", font=("Courier", 14, "bold"))
                Skip(-25)
            elif (i == 25 or i == 35):
                Skip(20)
                turtle.write(int(i / 5), align="center", font=("Courier", 14, "bold"))
                Skip(-20)
            else:
                turtle.write(int(i / 5), align="center", font=("Courier", 14, "bold"))
            Skip(-radius - 20)
        else:
            turtle.dot(5)
            Skip(-radius)
        turtle.right(6)
 
 
def Week(t):
    week = ["����һ", "���ڶ�", "������",
            "������", "������", "������", "������"]
    return week[t.weekday()]
 
 
def Date(t):
    y = t.year
    m = t.month
    d = t.day
    return "%s-%d-%d" % (y, m, d)
 
 
def Tick():
    # ���Ʊ���Ķ�̬��ʾ
    t = datetime.today()
    second = t.second + t.microsecond * 0.000001
    minute = t.minute + second / 60.0
    hour = t.hour + minute / 60.0
    secHand.setheading(6 * second)
    minHand.setheading(6 * minute)
    hurHand.setheading(30 * hour)
 
    turtle.tracer(False)
 
    printer.forward(65)
    printer.write(Week(t), align="center",
                  font=("Courier", 14, "bold"))
    printer.back(130)
    printer.write(Date(t), align="center",
                  font=("Courier", 14, "bold"))
    printer.home()
    turtle.tracer(True)
 
    # 100ms���������tick
    turtle.ontimer(Tick, 100)
 
 
def main():
    # ��/�رչ궯������Ϊ����ͼֽ�����ӳ١�
    turtle.tracer(False)
    Init()
    SetupClock(160)
    turtle.tracer(True)
    Tick()
    turtle.mainloop()
 
 
if __name__ == "__main__":
    main()