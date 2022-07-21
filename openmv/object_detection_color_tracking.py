# 修改逻辑：希望每张图中只识别到一种物体，每种物体只有一个，然后连续识别到n个物体才说明真正识别到，停车，
# k表示识别到物体次数（用于滤除噪声），n表示n次坐标求平均
import sensor, image, time, os, tf, math, uos, gc
from pyb import UART, delay
sensor.reset()                         # Reset and initialize the sensor.
sensor.set_pixformat(sensor.RGB565)    # Set pixel format to RGB565 (or GRAYSCALE)
sensor.set_framesize(sensor.QVGA)      # Set frame size to QVGA (320x240)
sensor.set_windowing((240, 240))       # Set 240x240 window.
sensor.skip_frames(time=2000)          # Let the camera adjust.
uart = UART(3, 256000) #PA2-P5  PA9-P5

net = None
labels = None
min_confidence = 0.5
arr=[0x41,0x42,0,0,0,0,0,0,0,0,0,0,0,0]
z = {'bottle':20,'orange':20,'eraser':6,'pen':6,'marker':6}
gripper ={'bottle':60,'orange':130,'eraser':80,'pen':60,'marker':60}
flag = 0 #识别到物体坐标的次数
center_x = 0
center_y = 0
k = 0
n = 10

try:
    # Load built in model
    labels, net = tf.load_builtin_model('trained')
except Exception as e:
    raise Exception(e)

colors = [ # Add more colors if you are detecting more than 7 types of classes at once.
    (255,   0,   0),
    (  0, 255,   0),
    (255, 255,   0),
    (  0,   0, 255),
    (255,   0, 255),
    (  0, 255, 255),
    (255, 255, 255),
]

#clock = time.clock()
while(True):
    #clock.tick()

    img = sensor.snapshot().lens_corr(1.8)
    objects = 0 #表示有几个物体

    # detect() returns all objects found in the image (splitted out per class already)
    # we skip class index 0, as that is the background, and then draw circles of the center
    # of our objects

    detect = net.detect(img, thresholds=[(math.ceil(min_confidence * 255), 255)])
    #print(len(detect))
    #print(detect)
    for i, detection_list in enumerate(detect):
        if (i == 0): continue # background class
        if (len(detection_list) != 0): objects += 1 #统计物体个数
    for i, detection_list in enumerate(detect):
        #外层对i遍历
        if (objects>1): break #跳过画面中出现两个物体的情况
        if (i == 0): continue # background class
        if (len(detection_list) != 1): continue # no detections for this class or detections larger than one

        #内层对detection_list遍历
        for d in detection_list: #符合以上要求的识别
            if (flag<k):#排除前两个误差
                flag += 1
                break
            #TODO: 发送数据进行停车
            if (flag==k):
                uart.write('A')
                uart.write('C')
                for index in range (12):
                    uart.write(str(arr[2+index]))
                flag += 1
                print("Stopped!")
                delay(1000)
                break
            print(f"********** %s{flag-k} **********" % labels[i]) #表示识别到若干个第i个物体
            if (flag>k and flag<k+n):
                [x, y, w, h] = d.rect()
                center_x += math.floor(x + (w / 2))
                center_y += math.floor(y + (h / 2))
                print('x %d\ty %d' % (center_x, center_y))
                img.draw_circle((center_x, center_y, 12), color=colors[i], thickness=2)
                flag += 1
                break
            else:
                center_x = center_x/n #n次坐标求平均
                center_y = center_y/n
                x1 = 120-center_y
                y1 = center_x-160
                x2 = 1.9*x1 #1.38表示mm与px的比例
                y2 = 1.9*y1
                X = int(x2+200) # 摄像头和基座的沿X轴线的距离
                Y = int(y2)+300
                Z = z[labels[i]]
                G = gripper[labels[i]]
                arr[2]=int(X/100)
                arr[3]=int(X%100/10)
                arr[4]=X%10
                arr[5]=int(Y/100)
                arr[6]=int(Y%100/10)
                arr[7]=Y%10
                arr[8]=int(Z/100)
                arr[9]=int(Z%100/10)
                arr[10]=int(Z%10)
                arr[11]=int(G/100)
                arr[12]=int(G%100/10)
                arr[13]=int(G%10)
                uart.write('A')
                uart.write('B')
                for index in range (12):
                    uart.write(str(arr[2+index]))
                print(f'X={X},Y={Y},Z={Z},G={G}')
                print('Transmission finished!')
                flag = 0
                center_x = 0
                center_y = 0
                delay(1000)#传输完成，等待车走
    #print("While End!\t\n")
