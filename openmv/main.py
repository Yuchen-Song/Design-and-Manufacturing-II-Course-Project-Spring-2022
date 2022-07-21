THRESHOLD = (0, 40) # 灰度阈值，40以下为黑色，以上为白色
BINARY_VISIBLE = True

import sensor, image, time
from pyb import UART
from pid import PID

rho_pid = PID(p=0.1, i=0.5)
theta_pid = PID(p=0.001, i=0)

uart = UART(3, 256000)


sensor.reset()
sensor.set_pixformat(sensor.GRAYSCALE)
sensor.set_framesize(sensor.QQQVGA) # 80x60 (4,800 pixels) - O(N^2) max = 2,3040,000.
sensor.skip_frames(time = 2000)     # WARNING: If you use QQVGA it may take seconds

clock = time.clock()
x=0
y=0
i_counter=0
arr=[0x0A,0x0A,0,0,0,0,0,0]
stra="left"


def passing_num(left,right):
    arr[2]=int(left/100)
    arr[3]=int(left%100/10)
    arr[4]=int(left%10)
    arr[5]=int(right/100)
    arr[6]=int(right%100/10)
    arr[7]=int(right%10)
    uart.write('A')
    uart.write('B')
    uart.write(str(arr[2]))
    uart.write(str(arr[3]))
    uart.write(str(arr[4]))
    uart.write(str(arr[5]))
    uart.write(str(arr[6]))
    uart.write(str(arr[7]))

    delta=1/3*abs(left-right)
    if ((left-right)>=delta):
        stra="right"
    elif (((left-right)<delta)and((left-right)>=0)):
         stra="straight"
    else:
        stra="left"
    print(left,right,stra)


while(True):
    clock.tick()
    # 二值化图像
    img = sensor.snapshot().binary([THRESHOLD]) if BINARY_VISIBLE else sensor.snapshot()
    # 拟合直线
    line = img.get_regression([(255,255) if BINARY_VISIBLE else THRESHOLD], robust = True)
    # 计算直线误差
    if (line):
        rho_err = abs(line.rho())-img.width()/2
        if line.theta()>90:
            theta_err = line.theta()-180
        else:
            theta_err = line.theta()
        # 绘制直线
        img.draw_line(line.line(), color = 127)
        print("rho_err",rho_err,"rho",line.rho(),"magnitude",line.magnitude(),"theta",theta_err)
        if line.magnitude()>8:
            rho_output = rho_pid.get_pid(rho_err,1)
            theta_output = theta_pid.get_pid(theta_err,1)
            output = rho_output+theta_output
            passing_num(400+10*output, 400-10*output)
        else:
            passing_num(1000,1000)
    else:
        passing_num(600,600)
        pass



