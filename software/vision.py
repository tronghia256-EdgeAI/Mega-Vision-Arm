from ultralytics import YOLO
import cv2
import time

model = YOLO(r"C:\Users\ASUS\Desktop\arm-robot-controller\models\best.pt") 
cap = cv2.VideoCapture(0)
cap.set(cv2.CAP_PROP_FRAME_WIDTH, 640)
cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 480)
prev_time = 0

while True:
    ret, frame = cap.read()
    if not ret:
        break
    current_time = time.time()
    fps = 1 / (current_time - prev_time) if prev_time != 0 else 0
    prev_time = current_time
    frame_resized = cv2.resize(frame, (480,480))
    results = model(frame_resized)
    annotated = results[0].plot()
    cv2.putText(
        annotated,
        f"FPS: {fps:.2f}",
        (20, 40),
        cv2.FONT_HERSHEY_SIMPLEX,
        1,
        (0, 255, 0),
        2
    )
    cv2.imshow("YOLOv8 ONNX - CPU - FPS", annotated)
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break
cap.release()
cv2.destroyAllWindows()
