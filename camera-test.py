"""Prueba la camara para ser usada con el beaglebone"""
import cv 																		#Importa la libreria de Open CV

#Aqui la variable capture toma el valor de la funcion para capturar video.
CAPTURE = cv.CaptureFromCAM(0) 													#el 0 es el indice del device
#Funciones para retornar las propiedades de captura de video.
cv.SetCaptureProperty(CAPTURE, cv.CV_CAP_PROP_FRAME_WIDTH, 960) 				#El ancho es de 960 pixeles
cv.SetCaptureProperty(CAPTURE, cv.CV_CAP_PROP_FRAME_HEIGHT, 720)				#El alto es de 720 pixeles
#Aqui la variable toma el valor de la proxima imagen decodificado
IM = cv.QueryFrame(CAPTURE)													
cv.SaveImage("/var/lib/cloud9/Dish-Detector/camera-test.jpg", IM)				#Guarda la imagen guardada en im			
#Pensado para ser usado en cloud9 IDE
