"""Programa de prueba utiliza cascade classifier y open CV para detectar caras en BB, version 2"""
import cv2
import cv
FACE_CASCADE = cv2.CascadeClassifier('haarcascade_frontalface_default.xml')

CAPTURE = cv.CaptureFromCAM(0)                                                                              

cv.SetCaptureProperty(CAPTURE, cv.CV_CAP_PROP_FRAME_WIDTH, 320)                                             
cv.SetCaptureProperty(CAPTURE, cv.CV_CAP_PROP_FRAME_HEIGHT, 240)                                            
#Aqui la variable toma el valor de la proxima imagen decodificado
IM = cv.QueryFrame(CAPTURE) 
cv.SaveImage("/var/lib/cloud9/Dish-Detector/sample.jpg", IM)

IMAGE = cv2.imread("/var/lib/cloud9/Dish-Detector/pudin.jpg")
GRAY = cv2.cvtColor(IMAGE, cv2.COLOR_BGR2GRAY)

FACES = FACE_CASCADE.detectMultiScale(
    GRAY,
    scaleFactor=1.1,
    minNeighbors=5,
    minSize=(30, 30),
    flags=cv2.cv.CV_HAAR_SCALE_IMAGE
    )

print "Found {0} faces!".format(len(FACES))
# Draw a rectangle around the faces
for (x, y, w, h) in FACES:
    cv2.rectangle(IMAGE, (x, y), (x+w, y+h), (0, 255, 0), 2)

# Display the resulting frame
#cv2.imshow("Faces Found", IMAGE)
#cv2.waitKey(0)
cv2.imwrite("/var/lib/cloud9/Dish-Detector/facedet.jpg", IMAGE)
