# test-paho-mqtt
test python paho mqtt module  
ทดสอบตามลิงค์นี้  
https://pypi.python.org/pypi/paho-mqtt  
  
โมดูลนี้ใช้งานได้ สามแบบ สามคลาส
```python
import paho.mqqt.client as client  
  
import paho.mqtt.publish as publish  
  
import paho.mqtt.subscribe as subscribe  
  
```  
*client*  
-คลาสสำหรับรับและส่งควบกัน  
-ทำงานแบบ callback และรันแบบ backgroud process  
-ไม่กินแรงเครื่อง  
-ออกแรงเซ็ตตัวแปร calback ตอนเริ่มต้น   
-เหมือนยากตอนแรก แต่สบายตอนหลัง  
  
*publish*  
-ส่งอย่างเดียว คำสั่งเดียว สั้นๆ ง่ายๆ  

*subscribe*  
-รับอย่างเดียว สองคำสั่ง รันสองแบบ  
-สั่งรับ แล้วต้องรอจนกว่า broker จะส่งกลับ  
-ง่ายตอนเริ่ม แต่เขียนโปรแกรมไปต่อยาก  
  
  




