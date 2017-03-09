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
-ทำงานแบบ callback รันแบบมัลติเทรด 
-สั่ง .loopstart() คือ ให้เริ่มรัน backgroud process  
-ไม่กินแรงเครื่อง  
-ต้องอ่านให้เข้าใจว่า มันสั่ง callback อะไรได้บ้างก่อน
-แล้วออกแรง เลือกเซ็ตฟังก์ชั่นกับตัวแปร calback ตอนเริ่มต้น   
-เหมือนยากตอนแรก แต่สบายตอนหลัง  
  
*publish*  
-ส่งอย่างเดียว คำสั่งเดียว สั้นๆ ง่ายๆ  

*subscribe*  
-รับอย่างเดียว สองคำสั่ง รันสองแบบ  
-สั่งรับแล้ว มันค้างที่สองคำสั่งนี้ไม่ไปไหน ต้องรอจนกว่า broker จะส่งกลับ  
-ง่ายตอนเริ่ม แต่เขียนโปรแกรมไปต่อเรื่องราวอื่นๆ ยาก  
  
  




