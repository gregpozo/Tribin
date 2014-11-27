"""Tira una foto para determinar el centro del circulo de un fregadero"""
import cv 																									#Importa la libreria de Open CV

#Aqui la variable capture toma el valor de la funcion para capturar video.
CAPTURE = cv.CaptureFromCAM(0)																				#el 0 es el indice del device
#			--------------------Funciones para retornar las propiedades de captura de video-------------
cv.SetCaptureProperty(CAPTURE, cv.CV_CAP_PROP_FRAME_WIDTH, 960)												#El ancho es de 960 pixeles
cv.SetCaptureProperty(CAPTURE, cv.CV_CAP_PROP_FRAME_HEIGHT, 720)											#El alto es de 720 pixeles
#Aqui la variable toma el valor de la proxima imagen decodificado
IM = cv.QueryFrame(CAPTURE)	
cv.SaveImage("/var/lib/cloud9/Dish-Detector/camera-test.jpg", IM)											#Guarda la imagen guardada en IM

#Variable que crea imagen con un size, cantidad de bits, y canales
EDGES = cv.CreateImage(cv.GetSize(IM), cv.IPL_DEPTH_8U, 1)													#Crea imagen de mismo size, 8 bits
#Used to convert Images in one color space to another
cv.CvtColor(IM, EDGES, cv.CV_BGR2GRAY)																		#Convierte la im original a graysc.
THRESH = 100																								#Threshold de la imagen
#Used to perform edge detection with the Canny Operator
cv.Canny(EDGES, EDGES, THRESH, THRESH / 2, 3)																#Se guarda en edges, con kernel 3x3
#Aplica filtro a la imagen deseada. El usado sera el gaussiano, con param 3x3
cv.Smooth(EDGES, EDGES, cv.CV_GAUSSIAN, 3, 3)																#Se guarda en edges, con kernel 3x3
#Creates a matrix header and allocates the matrix data
STORAGE = cv.CreateMat(640, 1, cv.CV_32FC3)																	#640 filas, 1 col, 32 bit float
#The function finds circles in a grayscale image using a modification of the Hough transform.
cv.HoughCircles(EDGES, STORAGE, cv.CV_HOUGH_GRADIENT, 2, EDGES.width / 10, THRESH, 350, 0, 0)
#Se escriben los valores en un archivo txt
F = open("/var/lib/cloud9/Dish-Detector/sink-empty.txt", "w")
for i in range(STORAGE.rows):
    val = STORAGE[i, 0]
    radius = int(val[2])
    center = (int(val[0]), int(val[1]))
    F.write(str(center[0]) + "," + str(center[1]) + "," + str(radius) + "\n")
    #The function circle draws a simple or filled circle with a given center and radius.
    cv.Circle(IM, center, radius, (0, 255, 0), thickness=2)													#Circulo rojo de 2mm de grueso
cv.SaveImage("/var/lib/cloud9/Dish-Detector/sink-empty.jpg", IM)											#Guarda la imagen
